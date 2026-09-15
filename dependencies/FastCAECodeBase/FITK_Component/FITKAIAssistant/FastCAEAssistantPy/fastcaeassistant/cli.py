"""FastCAEAssistant 交互式命令行入口。

CLI 在启动时初始化运行上下文，并提供类似 REPL 的多轮对话交互能力。
"""

from __future__ import annotations

import asyncio
import os
import sys
import threading
from pathlib import Path
from typing import Any

if __package__ is None or __package__ == "":
    project_root = Path(__file__).resolve().parent.parent
    if str(project_root) not in sys.path:
        sys.path.insert(0, str(project_root))

from fastcaeassistant.fastcae_context import build_fastcae_spec_prompt, load_fastcae_workspace_context
from fastcaeassistant.llm_client import LlmChatClient
from fastcaeassistant.mcp_registry import discover_mcp_servers_with_diagnostics
from fastcaeassistant.orchestrator import AssistantRunResult
from fastcaeassistant.orchestrator import run_assistant_turn_with_history
from fastcaeassistant.settings import load_assistant_settings


def _sanitize_text(text: str) -> str:
    """清洗文本中的非法代理字符，避免编码阶段抛异常。"""
    return text.encode("utf-8", errors="replace").decode("utf-8", errors="replace")


def _build_system_prompt(loaded_spec_prompt: str) -> str:
    """构建系统提示词，并按需拼接已加载的 FastCAE 规范片段。"""
    base = (
        "You are FastCAEAssistant, a proactive execution agent. "
        "Load and follow available .FastCAEAI agent/skill specs. "
        "When a loaded skill can help, call invoke_skill by yourself. "
        "Use MCP tools when external facts or calculations are required. "
        "Use concise Chinese output unless user asks otherwise."
    )
    if not loaded_spec_prompt:
        return base
    return base + "\n\n" + loaded_spec_prompt


def _print_tool_traces(tool_traces: list[dict[str, Any]]) -> None:
    """以简洁可读格式打印工具调用轨迹。"""
    if not tool_traces:
        print("Tools called: (none)")
        return

    print("Tools called:")
    for trace in tool_traces:
        print(f"- {trace['name']}({trace['args']}) => {trace['result']}")


def _resolve_project_root(explicit_project_root: str | Path | None = None) -> Path:
    """解析项目根目录，优先使用显式参数与环境变量，其次自动探测 .FastCAEAI。"""
    candidates: list[Path] = []

    if explicit_project_root:
        candidates.append(Path(explicit_project_root).expanduser().resolve())

    env_root = os.environ.get("FASTCAE_PROJECT_ROOT", "").strip()
    if env_root:
        candidates.append(Path(env_root).expanduser().resolve())

    module_root = Path(__file__).resolve().parent.parent
    candidates.append(module_root)

    cwd = Path.cwd().resolve()
    candidates.append(cwd)
    candidates.extend(cwd.parents)

    seen: set[Path] = set()
    for base in candidates:
        if base in seen:
            continue
        seen.add(base)
        if (base / ".FastCAEAI").exists():
            return base

    return module_root


def _run_coroutine_sync(coro: Any) -> Any:
    """同步运行协程；若当前线程已有事件循环，则在子线程执行。"""
    try:
        asyncio.get_running_loop()
    except RuntimeError:
        return asyncio.run(coro)

    result_box: dict[str, Any] = {}
    error_box: dict[str, BaseException] = {}

    def _runner() -> None:
        try:
            result_box["result"] = asyncio.run(coro)
        except BaseException as exc:
            error_box["error"] = exc

    worker = threading.Thread(target=_runner, daemon=True)
    worker.start()
    worker.join()

    if "error" in error_box:
        raise error_box["error"]
    return result_box.get("result")


class FastCAEAssistantSession:
    """可复用的会话对象，适配 PythonQt 等无 stdin 环境。"""

    def __init__(self, stream_output: bool = False, project_root: str | Path | None = None) -> None:
        try:
            settings = load_assistant_settings()
            self._chat_client = LlmChatClient(settings)
        except Exception as exc:
            raise RuntimeError(f"启动失败：配置或模型客户端初始化异常。{exc}") from exc

        self._project_root = _resolve_project_root(project_root)
        self._context = load_fastcae_workspace_context(self._project_root)
        self._mcp_servers, self._mcp_diag = discover_mcp_servers_with_diagnostics(self._project_root)
        self._system_prompt = _build_system_prompt(build_fastcae_spec_prompt(self._context))
        self._conversation_history: list[dict[str, Any]] = []
        self._stream_output = stream_output

    @property
    def project_root(self) -> Path:
        return self._project_root

    @property
    def loaded_agent_count(self) -> int:
        return len(self._context.agents)

    @property
    def loaded_agent_names(self) -> list[str]:
        return [item.name for item in self._context.agents]

    @property
    def loaded_skill_count(self) -> int:
        return len(self._context.skills)

    @property
    def loaded_skill_names(self) -> list[str]:
        return [item.name for item in self._context.skills]

    @property
    def loaded_mcp_server_count(self) -> int:
        return len(self._mcp_servers)

    @property
    def loaded_mcp_server_names(self) -> list[str]:
        return [item.name for item in self._mcp_servers]

    @property
    def mcp_diagnostics(self) -> str | None:
        return self._mcp_diag

    def clear_history(self) -> None:
        """清空当前会话历史。"""
        self._conversation_history.clear()

    def ask(
        self,
        user_input: str,
        stream_output: bool | None = None,
        cancel_event: threading.Event | None = None,
    ) -> AssistantRunResult:
        """执行一轮问答并自动维护历史。"""
        sanitized = _sanitize_text((user_input or "").strip())
        if not sanitized:
            return AssistantRunResult(final_answer="", called_tools=[], tool_traces=[], history_messages=[])

        normalized = sanitized.lower()
        if normalized == "clear":
            self.clear_history()
            return AssistantRunResult(final_answer="对话历史已清空。", called_tools=[], tool_traces=[], history_messages=[])

        if sanitized.startswith("/agent"):
            parts = sanitized.split(maxsplit=1)
            if len(parts) < 2 or not parts[1].strip():
                return AssistantRunResult(
                    final_answer="请在 /agent 后输入任务内容，例如：/agent 帮我计算 17*9 并给出当前时间",
                    called_tools=[],
                    tool_traces=[],
                    history_messages=[],
                )
            effective_query = parts[1].strip()
        else:
            effective_query = sanitized

        streamed_chars = 0

        def _on_output_chunk(chunk: str) -> None:
            nonlocal streamed_chars
            if cancel_event is not None and cancel_event.is_set():
                raise RuntimeError("USER_CANCELLED")
            safe_chunk = _sanitize_text(chunk)
            streamed_chars += len(safe_chunk)
            print(safe_chunk, end="", flush=True)

        use_stream = self._stream_output if stream_output is None else stream_output

        result = _run_coroutine_sync(
            run_assistant_turn_with_history(
                client=self._chat_client,
                conversation_history=self._conversation_history,
                user_query=effective_query,
                system_prompt=self._system_prompt,
                skills=self._context.skills,
                mcp_servers=self._mcp_servers,
                project_root=self._project_root,
                stream_output=use_stream,
                on_output_chunk=_on_output_chunk if use_stream else None,
                should_cancel=(lambda: bool(cancel_event is not None and cancel_event.is_set())),
            )
        )

        if cancel_event is not None and cancel_event.is_set():
            return AssistantRunResult(
                final_answer="对话已停止。",
                called_tools=result.called_tools,
                tool_traces=result.tool_traces,
                history_messages=result.history_messages,
            )

        result_answer = _sanitize_text(result.final_answer)
        for message in result.history_messages:
            cloned = dict(message)
            if cloned.get("role") in {"assistant", "tool"}:
                cloned["content"] = _sanitize_text(str(cloned.get("content", "") or ""))
            self._conversation_history.append(cloned)

        if use_stream and streamed_chars == 0 and result_answer:
            print(result_answer)

        return AssistantRunResult(
            final_answer=result_answer,
            called_tools=result.called_tools,
            tool_traces=result.tool_traces,
            history_messages=result.history_messages,
        )


def create_fastcaeassistant_session(
    stream_output: bool = False,
    project_root: str | Path | None = None,
) -> FastCAEAssistantSession:
    """创建可供外部宿主复用的会话对象。"""
    return FastCAEAssistantSession(stream_output=stream_output, project_root=project_root)


def run_fastcaeassistant_cli(interactive: bool = True) -> None:
    """运行 FastCAEAssistant 终端会话。"""
    print("正在启动 FastCAEAssistant CLI...")
    try:
        session = create_fastcaeassistant_session(stream_output=True)
    except Exception as exc:
        print(exc)
        print("请检查 .env 的 LLM_PROTOCOL / LLM_API_KEY / LLM_MODEL / LLM_BASE_URL / LLM_TEMPERATURE。")
        return

    print("=== FastCAEAssistant Interactive Chat ===")
    print(f"项目根目录: {session.project_root}")
    print(f"已加载 agent: {session.loaded_agent_count}")
    for name in session.loaded_agent_names:
        print(f"- {name}")
    print(f"已加载 skill: {session.loaded_skill_count}")
    for name in session.loaded_skill_names:
        print(f"- {name}")
    print(f"已加载 mcp server: {session.loaded_mcp_server_count}")
    for name in session.loaded_mcp_server_names:
        print(f"- {name}")
    if session.mcp_diagnostics and session.loaded_mcp_server_count == 0:
        print(f"MCP 诊断: {session.mcp_diagnostics}")
    print("\n输入你的问题开始对话，输入 'exit' 或 'quit' 结束会话。\n")
    print("输入 'clear' 清空对话历史。\n")
    print("输入 '/agent 任务描述' 启用主动 Agent 模式。\n")

    if not interactive:
        print("当前为非交互模式：已完成初始化检查。")
        return

    while True:
        try:
            user_input = input("You> ").strip()
        except (EOFError, RuntimeError):
            print("检测到 stdin 不可用，请改用 create_fastcaeassistant_session().ask(...) 交互。")
            break

        normalized = user_input.lower()
        if normalized in {"exit", "quit"}:
            print("Bye.")
            break

        if not user_input:
            continue

        print("Assistant>")
        try:
            result = session.ask(user_input, stream_output=True)
        except Exception as exc:
            print()
            print(f"本轮执行失败：{exc}")
            print("你可以继续输入下一个问题，或输入 quit 退出。")
            continue

        print()
        _print_tool_traces(result.tool_traces)
        print()


if __name__ == "__main__":
    run_fastcaeassistant_cli()
