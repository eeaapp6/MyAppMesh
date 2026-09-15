"""FastCAEAssistant 工具调用流程的核心编排逻辑。

该模块负责协同多 MCP 服务会话、本地 skill 调用与模型多轮工具调用流程。
"""

from __future__ import annotations

import json
import re
from contextlib import AsyncExitStack
from collections.abc import Callable
from dataclasses import dataclass
from pathlib import Path
from typing import Any

from fastcaeassistant.fastcae_context import SkillSpec
from fastcaeassistant.llm_client import DeepSeekChatClient
from fastcaeassistant.mcp_registry import McpServerSpec, discover_mcp_servers

DEFAULT_SYSTEM_PROMPT = (
    "You are an assistant that can use MCP tools when needed. "
    "Always decide by yourself whether to call tools."
)
LOCAL_SKILL_TOOL_NAME = "invoke_skill"


@dataclass(frozen=True)
class AssistantRunResult:
    """单轮助手执行结果的结构化输出。"""

    #: 返回给调用方的最终文本答案。
    final_answer: str
    #: 本轮调用过的工具名称（按调用顺序）。
    called_tools: list[str]
    #: 工具调用追踪明细（参数与结果文本）。
    tool_traces: list[dict[str, Any]]
    #: 当前轮次需要持久化到后续上下文的消息增量。
    history_messages: list[dict[str, Any]]


def _build_local_skill_tool_schema(skills: list[SkillSpec]) -> dict[str, Any] | None:
    """构建用于调用本地 skill 的 OpenAI 风格工具 schema。"""
    if not skills:
        return None

    skill_names = [skill.name for skill in skills]
    return {
        "type": "function",
        "function": {
            "name": LOCAL_SKILL_TOOL_NAME,
            "description": (
                "Execute a loaded FastCAE skill from .FastCAEAI/skills. "
                "Use this when a skill spec is relevant to the user task."
            ),
            "parameters": {
                "type": "object",
                "properties": {
                    "skill_name": {
                        "type": "string",
                        "enum": skill_names,
                        "description": "The skill to execute.",
                    },
                    "task": {
                        "type": "string",
                        "description": "The concrete subtask to perform with the selected skill.",
                    },
                },
                "required": ["skill_name", "task"],
            },
        },
    }


def _run_local_skill(
    client: DeepSeekChatClient,
    skills_by_name: dict[str, SkillSpec],
    skill_name: str,
    task: str,
) -> str:
    """通过一次受约束的模型调用执行本地 FastCAE skill。"""
    selected = skills_by_name.get(skill_name.lower())
    if selected is None:
        return f"Skill '{skill_name}' not found in .FastCAEAI/skills."

    try:
        response = client.request_chat_completion(
            messages=[
                {
                    "role": "system",
                    "content": (
                        "You are executing one FastCAE skill. Follow the skill specification strictly. "
                        "Return practical output only.\n\n"
                        f"Skill name: {selected.name}\n"
                        f"Skill spec:\n{selected.content}"
                    ),
                },
                {"role": "user", "content": task},
            ],
            temperature=client.default_temperature,
        )
    except Exception as exc:
        return f"Skill execution failed: {exc}"

    choices = getattr(response, "choices", None)
    if not choices:
        return "Skill execution returned empty response."
    message = getattr(choices[0], "message", None)
    if message is None:
        return "Skill execution returned invalid message."

    return getattr(message, "content", "") or ""


def _mcp_tool_schema(tool: Any) -> dict[str, Any]:
    """将 MCP 工具元数据转换为 OpenAI 工具 schema 格式。"""
    parameters = getattr(tool, "inputSchema", None) or {
        "type": "object",
        "properties": {},
    }

    return {
        "type": "function",
        "function": {
            "name": tool.name,
            "description": getattr(tool, "description", "") or "MCP tool",
            "parameters": parameters,
        },
    }


def _safe_server_name(name: str) -> str:
    """将服务名清洗为可用于工具前缀的安全标识。"""
    safe = re.sub(r"[^a-zA-Z0-9_]", "_", name)
    return safe or "server"


def _exposed_tool_name(server_name: str, raw_tool_name: str) -> str:
    """通过服务名前缀构造全局唯一的暴露工具名。"""
    return f"{_safe_server_name(server_name)}__{raw_tool_name}"


def _extract_tool_result_text(result: Any) -> str:
    """从 MCP 调用结果对象中提取可读文本。"""
    content = getattr(result, "content", None)
    if not content:
        return str(result)

    texts: list[str] = []
    for item in content:
        text = getattr(item, "text", None)
        if text:
            texts.append(text)

    if texts:
        return "\n".join(texts)

    structured = getattr(result, "structuredContent", None)
    if structured is not None:
        return json.dumps(structured, ensure_ascii=False)

    return str(result)


def _tool_call_to_dict(tool_call: Any) -> dict[str, Any]:
    """将供应商工具调用对象标准化为可序列化字典。"""
    if hasattr(tool_call, "model_dump"):
        return tool_call.model_dump()

    return {
        "id": tool_call.id,
        "type": "function",
        "function": {
            "name": tool_call.function.name,
            "arguments": tool_call.function.arguments,
        },
    }


def _assistant_message(response: Any) -> Any | None:
    """从原始响应中安全提取首个 assistant 消息。"""
    if response is None:
        return None
    choices = getattr(response, "choices", None)
    if not choices:
        return None
    return getattr(choices[0], "message", None)


def _stream_assistant_response(
    stream: Any,
    on_output_chunk: Callable[[str], None] | None = None,
    should_cancel: Callable[[], bool] | None = None,
) -> tuple[str, list[dict[str, str]], str]:
    """聚合流式响应内容，并提取工具调用信息。"""
    content_parts: list[str] = []
    tool_calls: dict[int, dict[str, str]] = {}
    reasoning_parts: list[str] = []

    for chunk in stream:
        if should_cancel is not None and should_cancel():
            raise RuntimeError("USER_CANCELLED")

        choices = getattr(chunk, "choices", None)
        if not choices:
            continue

        delta = getattr(choices[0], "delta", None)
        if delta is None:
            continue

        text = getattr(delta, "content", None)
        if text:
            content_parts.append(text)
            if on_output_chunk is not None:
                on_output_chunk(text)

        reasoning_text = getattr(delta, "reasoning_content", None)
        if reasoning_text:
            reasoning_parts.append(reasoning_text)

        delta_tool_calls = getattr(delta, "tool_calls", None) or []
        for tc in delta_tool_calls:
            idx = getattr(tc, "index", 0) or 0
            item = tool_calls.setdefault(idx, {"id": "", "name": "", "arguments": ""})

            tc_id = getattr(tc, "id", None)
            if tc_id:
                item["id"] = tc_id

            func = getattr(tc, "function", None)
            if func is not None:
                fn_name = getattr(func, "name", None)
                if fn_name:
                    item["name"] = fn_name
                fn_args = getattr(func, "arguments", None)
                if fn_args:
                    item["arguments"] += fn_args

    ordered_calls = [tool_calls[idx] for idx in sorted(tool_calls.keys())]
    return "".join(content_parts), ordered_calls, "".join(reasoning_parts)


async def run_assistant_turn(
    client: DeepSeekChatClient,
    user_query: str,
    system_prompt: str | None = None,
    skills: list[SkillSpec] | None = None,
    mcp_servers: list[McpServerSpec] | None = None,
    project_root: Path | None = None,
    max_rounds: int = 6,
    stream_output: bool = False,
    on_output_chunk: Callable[[str], None] | None = None,
    should_cancel: Callable[[], bool] | None = None,
) -> AssistantRunResult:
    """执行一轮不含历史上下文的助手对话。"""
    return await run_assistant_turn_with_history(
        client=client,
        conversation_history=[],
        user_query=user_query,
        system_prompt=system_prompt,
        skills=skills,
        mcp_servers=mcp_servers,
        project_root=project_root,
        max_rounds=max_rounds,
        stream_output=stream_output,
        on_output_chunk=on_output_chunk,
        should_cancel=should_cancel,
    )


async def run_assistant_turn_with_history(
    client: DeepSeekChatClient,
    conversation_history: list[dict[str, Any]],
    user_query: str,
    system_prompt: str | None = None,
    skills: list[SkillSpec] | None = None,
    mcp_servers: list[McpServerSpec] | None = None,
    project_root: Path | None = None,
    max_rounds: int = 6,
    stream_output: bool = False,
    on_output_chunk: Callable[[str], None] | None = None,
    should_cancel: Callable[[], bool] | None = None,
) -> AssistantRunResult:
    """执行一轮带历史上下文的助手对话。

    函数会打开全部已发现的 MCP 服务会话，把工具暴露给模型，执行工具调用，
    最终返回答案与调用轨迹。
    """
    root = project_root or Path(__file__).resolve().parent.parent
    discovered_servers = mcp_servers or discover_mcp_servers(root)

    mcp_client_session_cls: Any | None = None
    mcp_stdio_client: Any | None = None
    if discovered_servers:
        try:
            from mcp import ClientSession as _ClientSession
            from mcp.client.stdio import stdio_client as _stdio_client

            mcp_client_session_cls = _ClientSession
            mcp_stdio_client = _stdio_client
        except ModuleNotFoundError:
            # 例如 Windows 缺少 pywintypes 时，自动禁用 MCP 工具会话。
            discovered_servers = []

    async with AsyncExitStack() as stack:
        active_sessions: list[tuple[McpServerSpec, Any]] = []

        for server in discovered_servers:
            try:
                # PythonQt 等宿主可能将 sys.stderr 重定向为无 fileno 对象。
                # 传入 errlog=None 可避免子进程创建时继承不可用 stderr。
                try:
                    stdio_context = mcp_stdio_client(server.params, errlog=None)
                except TypeError:
                    stdio_context = mcp_stdio_client(server.params)

                read_stream, write_stream = await stack.enter_async_context(stdio_context)
                session = await stack.enter_async_context(mcp_client_session_cls(read_stream, write_stream))
                await session.initialize()
                active_sessions.append((server, session))
            except Exception as exc:
                # 单个 MCP 服务异常时降级跳过，避免整个 ask() 失败。
                print(f"[MCP 跳过] server={server.name}, reason={exc}")
                continue

        openai_tools: list[dict[str, Any]] = []
        tool_routes: dict[str, tuple[Any, str]] = {}

        for server, session in active_sessions:
            tools_response = await session.list_tools()
            for tool in tools_response.tools:
                exposed_name = _exposed_tool_name(server.name, tool.name)
                schema = _mcp_tool_schema(tool)
                schema["function"]["name"] = exposed_name
                schema["function"]["description"] = (
                    f"Server '{server.name}': " + schema["function"]["description"]
                )
                openai_tools.append(schema)
                tool_routes[exposed_name] = (session, tool.name)

        loaded_skills = skills or []
        local_skill_tool = _build_local_skill_tool_schema(loaded_skills)
        if local_skill_tool:
            openai_tools.append(local_skill_tool)
        skills_by_name = {skill.name.lower(): skill for skill in loaded_skills}

        messages: list[dict[str, Any]] = [
            {
                "role": "system",
                "content": system_prompt or DEFAULT_SYSTEM_PROMPT,
            }
        ]
        turn_history_messages: list[dict[str, Any]] = []
        for item in conversation_history:
            role = item.get("role", "")
            if role == "user":
                content = item.get("content", "")
                if isinstance(content, str):
                    messages.append({"role": "user", "content": content})
            elif role == "assistant":
                assistant_message: dict[str, Any] = {
                    "role": "assistant",
                    "content": item.get("content", "") or "",
                }
                if item.get("tool_calls"):
                    assistant_message["tool_calls"] = item["tool_calls"]
                if item.get("reasoning_content"):
                    assistant_message["reasoning_content"] = item["reasoning_content"]
                if item.get("anthropic_content"):
                    assistant_message["anthropic_content"] = item["anthropic_content"]
                messages.append(assistant_message)
            elif role == "tool":
                content = item.get("content", "")
                tool_call_id = item.get("tool_call_id", "")
                if isinstance(content, str) and isinstance(tool_call_id, str):
                    messages.append({"role": "tool", "tool_call_id": tool_call_id, "content": content})
        user_message = {"role": "user", "content": user_query}
        messages.append(user_message)
        turn_history_messages.append(user_message)

        called_tools: list[str] = []
        tool_traces: list[dict[str, Any]] = []

        for _ in range(max_rounds):
            if should_cancel is not None and should_cancel():
                return AssistantRunResult(
                    final_answer="对话已停止。",
                    called_tools=called_tools,
                    tool_traces=tool_traces,
                    history_messages=turn_history_messages,
                )

            assistant_anthropic_content = None
            assistant_reasoning_content = None
            try:
                if stream_output and (
                    getattr(client, "_protocol", None) == "openai"
                    and not client.should_preserve_reasoning()
                ):
                    streamed_text, streamed_tool_calls, streamed_reasoning = _stream_assistant_response(
                        client.request_chat_completion_stream(
                            messages=messages,
                            tools=openai_tools,
                            tool_choice="auto",
                            temperature=client.default_temperature,
                        ),
                        on_output_chunk=on_output_chunk,
                        should_cancel=should_cancel,
                    )
                    assistant_content = streamed_text or ""
                    assistant_tool_calls = streamed_tool_calls
                    assistant_reasoning_content = streamed_reasoning or None
                else:
                    response = client.request_chat_completion(
                        messages=messages,
                        tools=openai_tools,
                        tool_choice="auto",
                        temperature=client.default_temperature,
                    )
                    assistant = _assistant_message(response)
                    if assistant is None:
                        return AssistantRunResult(
                            final_answer="LLM returned empty or invalid response.",
                            called_tools=called_tools,
                            tool_traces=tool_traces,
                            history_messages=turn_history_messages,
                        )

                    assistant_content = assistant.content or ""
                    assistant_anthropic_content = getattr(assistant, "anthropic_content", None)
                    assistant_reasoning_content = getattr(assistant, "reasoning_content", None)
                    assistant_tool_calls = [
                        {
                            "id": tc.id,
                            "name": tc.function.name,
                            "arguments": tc.function.arguments or "{}",
                        }
                        for tc in (assistant.tool_calls or [])
                    ]
            except Exception as exc:
                if should_cancel is not None and should_cancel():
                    return AssistantRunResult(
                        final_answer="对话已停止。",
                        called_tools=called_tools,
                        tool_traces=tool_traces,
                        history_messages=turn_history_messages,
                    )
                return AssistantRunResult(
                    final_answer=f"LLM call failed: {exc}",
                    called_tools=called_tools,
                    tool_traces=tool_traces,
                    history_messages=turn_history_messages,
                )

            if assistant_tool_calls:
                assistant_message: dict[str, Any] = {
                    "role": "assistant",
                    "content": assistant_content,
                    "tool_calls": [
                        {
                            "id": tc["id"],
                            "type": "function",
                            "function": {
                                "name": tc["name"],
                                "arguments": tc["arguments"] or "{}",
                            },
                        }
                        for tc in assistant_tool_calls
                    ],
                }
                if assistant_anthropic_content:
                    assistant_message["anthropic_content"] = assistant_anthropic_content
                if assistant_reasoning_content:
                    assistant_message["reasoning_content"] = assistant_reasoning_content
                messages.append(assistant_message)
                turn_history_messages.append(assistant_message)

                for tool_call in assistant_tool_calls:
                    if should_cancel is not None and should_cancel():
                        return AssistantRunResult(
                            final_answer="对话已停止。",
                            called_tools=called_tools,
                            tool_traces=tool_traces,
                            history_messages=turn_history_messages,
                        )

                    tool_name = tool_call["name"]
                    called_tools.append(tool_name)

                    raw_args = tool_call["arguments"] or "{}"
                    try:
                        tool_args = json.loads(raw_args)
                    except json.JSONDecodeError:
                        tool_args = {}

                    # 实时输出调用过程
                    if tool_name == LOCAL_SKILL_TOOL_NAME:
                        skill_name = str(tool_args.get("skill_name", "")).strip()
                        task = str(tool_args.get("task", "")).strip()
                        print(f"[调用 skill] skill_name: {skill_name}, task: {task}")
                        tool_text = _run_local_skill(client, skills_by_name, skill_name, task)
                    else:
                        print(f"[调用 MCP 工具] tool_name: {tool_name}, args: {tool_args}")
                        route = tool_routes.get(tool_name)
                        if route is None:
                            tool_text = f"Unknown tool: {tool_name}"
                        else:
                            session, raw_tool_name = route
                            try:
                                tool_result = await session.call_tool(raw_tool_name, tool_args)
                                tool_text = _extract_tool_result_text(tool_result)
                            except Exception as exc:
                                tool_text = f"Tool call failed: {exc}"

                    tool_traces.append(
                        {
                            "name": tool_name,
                            "args": tool_args,
                            "result": tool_text,
                        }
                    )
                    tool_message = {
                        "role": "tool",
                        "tool_call_id": tool_call["id"],
                        "content": tool_text,
                    }
                    messages.append(tool_message)
                    turn_history_messages.append(tool_message)
                continue

            final_assistant_message: dict[str, Any] = {
                "role": "assistant",
                "content": assistant_content,
            }
            if assistant_anthropic_content:
                final_assistant_message["anthropic_content"] = assistant_anthropic_content
            if assistant_reasoning_content:
                final_assistant_message["reasoning_content"] = assistant_reasoning_content
            turn_history_messages.append(final_assistant_message)
            return AssistantRunResult(
                final_answer=assistant_content,
                called_tools=called_tools,
                tool_traces=tool_traces,
                history_messages=turn_history_messages,
            )

        return AssistantRunResult(
            final_answer="Model did not finish within max tool-call rounds.",
            called_tools=called_tools,
            tool_traces=tool_traces,
            history_messages=turn_history_messages,
        )
