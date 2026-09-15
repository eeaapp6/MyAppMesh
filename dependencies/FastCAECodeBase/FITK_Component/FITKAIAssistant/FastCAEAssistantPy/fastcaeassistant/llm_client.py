"""FastCAEAssistant 的多协议 LLM 客户端封装。

该模块统一封装 OpenAI 协议与 Anthropic 协议，向上层提供一致调用接口。
"""

from __future__ import annotations

from collections.abc import Iterator
import json
from types import SimpleNamespace
from typing import Any

from openai import OpenAI

from fastcaeassistant.settings import AssistantSettings

try:
    from anthropic import Anthropic
except ImportError:
    Anthropic = None


class LlmChatClient:
    """统一封装 openai/anthropic 两种协议的对话客户端。"""

    @property
    def default_temperature(self) -> float:
        """返回当前会话默认使用的采样温度。"""
        return self._settings.llm_temperature

    @staticmethod
    def _serialize_anthropic_block(block: Any) -> dict[str, Any]:
        """将 Anthropic 内容块序列化为可回传的普通字典。"""
        if isinstance(block, dict):
            return dict(block)

        block_type = getattr(block, "type", None)
        serialized: dict[str, Any] = {"type": block_type}

        for field_name in ("text", "thinking", "signature", "id", "name", "input", "tool_use_id", "content"):
            value = getattr(block, field_name, None)
            if value is not None:
                serialized[field_name] = value

        return serialized

    def __init__(self, settings: AssistantSettings) -> None:
        #: 构建请求载荷时使用的运行时配置。
        self._settings = settings
        #: 当前启用的协议类型。
        self._protocol = settings.llm_protocol
        #: 底层协议客户端实例（OpenAI 或 Anthropic）。
        if self._protocol == "openai":
            self._client = OpenAI(
                api_key=settings.llm_api_key,
                base_url=settings.llm_base_url,
            )
        else:
            if Anthropic is None:
                raise RuntimeError(
                    "Anthropic protocol selected but anthropic package is not installed. "
                    "Please install with: pip install anthropic"
                )
            self._client = Anthropic(
                api_key=settings.llm_api_key,
                base_url=settings.llm_base_url,
            )

    def should_preserve_reasoning(self) -> bool:
        """判断当前模型是否需要保留 reasoning/thinking 内容。"""
        if "deepseek" not in self._settings.llm_base_url.lower():
            return False

        model_name = self._settings.llm_model.lower()
        return model_name.startswith("deepseek-v4") or model_name.startswith("deepseek-reasoner")

    def _openai_tools_to_anthropic_tools(self, tools: list[dict[str, Any]] | None) -> list[dict[str, Any]] | None:
        """将 OpenAI 风格工具定义转换为 Anthropic 工具定义。"""
        if not tools:
            return None
        converted: list[dict[str, Any]] = []
        for item in tools:
            function_def = item.get("function", {})
            converted.append(
                {
                    "name": function_def.get("name", "tool"),
                    "description": function_def.get("description", ""),
                    "input_schema": function_def.get("parameters", {"type": "object", "properties": {}}),
                }
            )
        return converted

    def _normalize_anthropic_messages(
        self, messages: list[dict[str, Any]]
    ) -> tuple[str | None, list[dict[str, Any]]]:
        """将 OpenAI 对话消息转换为 Anthropic messages.create 入参。"""
        system_parts: list[str] = []
        normalized: list[dict[str, Any]] = []

        for message in messages:
            role = message.get("role")
            if role == "system":
                content = str(message.get("content", "")).strip()
                if content:
                    system_parts.append(content)
                continue

            if role == "user":
                normalized.append({"role": "user", "content": str(message.get("content", ""))})
                continue

            if role == "assistant":
                raw_blocks = message.get("anthropic_content")
                if isinstance(raw_blocks, list) and raw_blocks:
                    normalized.append(
                        {
                            "role": "assistant",
                            "content": [self._serialize_anthropic_block(block) for block in raw_blocks],
                        }
                    )
                    continue

                assistant_content: list[dict[str, Any]] = []
                text = str(message.get("content", "") or "")
                if text:
                    assistant_content.append({"type": "text", "text": text})

                for tool_call in message.get("tool_calls", []) or []:
                    function_call = tool_call.get("function", {})
                    raw_args = function_call.get("arguments", "{}")
                    try:
                        parsed_args = json.loads(raw_args) if isinstance(raw_args, str) else raw_args
                    except json.JSONDecodeError:
                        parsed_args = {}
                    assistant_content.append(
                        {
                            "type": "tool_use",
                            "id": tool_call.get("id", ""),
                            "name": function_call.get("name", "tool"),
                            "input": parsed_args or {},
                        }
                    )

                normalized.append({"role": "assistant", "content": assistant_content or [{"type": "text", "text": ""}]})
                continue

            if role == "tool":
                normalized.append(
                    {
                        "role": "user",
                        "content": [
                            {
                                "type": "tool_result",
                                "tool_use_id": message.get("tool_call_id", ""),
                                "content": str(message.get("content", "")),
                            }
                        ],
                    }
                )

        system = "\n\n".join(system_parts).strip() or None
        return system, normalized

    def _normalize_anthropic_response(self, response: Any) -> Any:
        """将 Anthropic 响应转换为编排层可复用的 OpenAI 兼容结构。"""
        content_blocks = getattr(response, "content", None) or []
        text_parts: list[str] = []
        tool_calls: list[Any] = []
        serialized_blocks: list[dict[str, Any]] = []

        for block in content_blocks:
            block_type = getattr(block, "type", None)
            serialized_blocks.append(self._serialize_anthropic_block(block))
            if block_type == "text":
                text_parts.append(getattr(block, "text", "") or "")
                continue
            if block_type == "thinking":
                continue
            if block_type == "tool_use":
                args = getattr(block, "input", None) or {}
                tool_calls.append(
                    SimpleNamespace(
                        id=getattr(block, "id", ""),
                        function=SimpleNamespace(
                            name=getattr(block, "name", ""),
                            arguments=json.dumps(args, ensure_ascii=False),
                        ),
                    )
                )

        message = SimpleNamespace(
            content="".join(text_parts),
            tool_calls=tool_calls,
            anthropic_content=serialized_blocks,
        )
        return SimpleNamespace(choices=[SimpleNamespace(message=message)])

    def _create_openai_chat(
        self,
        messages: list[dict[str, Any]],
        tools: list[dict[str, Any]] | None,
        tool_choice: str | dict[str, Any] | None,
        temperature: float,
        stream: bool,
    ) -> Any:
        """执行 OpenAI 协议请求。"""
        payload: dict[str, Any] = {
            "model": self._settings.llm_model,
            "messages": messages,
            "temperature": temperature,
        }
        if self.should_preserve_reasoning():
            payload["reasoning_effort"] = "high"
            payload["extra_body"] = {"thinking": {"type": "enabled"}}
        if tools:
            payload["tools"] = tools
        if tool_choice is not None:
            payload["tool_choice"] = tool_choice
        if stream:
            payload["stream"] = True
        return self._client.chat.completions.create(**payload)

    def _create_anthropic_chat(
        self,
        messages: list[dict[str, Any]],
        tools: list[dict[str, Any]] | None,
        tool_choice: str | dict[str, Any] | None,
        temperature: float,
    ) -> Any:
        """执行 Anthropic 协议请求，并返回标准化结果。"""
        system, anthropic_messages = self._normalize_anthropic_messages(messages)
        payload: dict[str, Any] = {
            "model": self._settings.llm_model,
            "messages": anthropic_messages,
            "temperature": temperature,
            "max_tokens": 2048,
        }
        if system:
            payload["system"] = system

        converted_tools = self._openai_tools_to_anthropic_tools(tools)
        if converted_tools:
            payload["tools"] = converted_tools
            if tool_choice is not None:
                if tool_choice == "auto":
                    payload["tool_choice"] = {"type": "auto"}
                elif isinstance(tool_choice, dict):
                    payload["tool_choice"] = tool_choice

        response = self._client.messages.create(**payload)
        return self._normalize_anthropic_response(response)

    def request_chat_completion(
        self,
        messages: list[dict[str, Any]],
        tools: list[dict[str, Any]] | None = None,
        tool_choice: str | dict[str, Any] | None = None,
        temperature: float | None = None,
    ) -> Any:
        """发送一次对话请求（自动根据协议分发）。

        Args:
            messages: OpenAI Chat 格式的会话消息列表。
            tools: 可选的工具 schema 列表。
            tool_choice: 可选的工具选择策略（例如 "auto"）。
            temperature: 采样温度参数；为空时使用 .env 中的默认配置。

        Returns:
            Any: 编排层可复用的标准响应对象。
        """
        effective_temperature = self.default_temperature if temperature is None else temperature
        if self._protocol == "openai":
            return self._create_openai_chat(messages, tools, tool_choice, effective_temperature, stream=False)
        return self._create_anthropic_chat(messages, tools, tool_choice, effective_temperature)

    def request_chat_completion_stream(
        self,
        messages: list[dict[str, Any]],
        tools: list[dict[str, Any]] | None = None,
        tool_choice: str | dict[str, Any] | None = None,
        temperature: float | None = None,
    ) -> Iterator[Any]:
        """发送流式对话请求（OpenAI 真流式，Anthropic 兼容回退）。

        Args:
            messages: OpenAI Chat 格式的会话消息列表。
            tools: 可选的工具 schema 列表。
            tool_choice: 可选的工具选择策略（例如 "auto"）。
            temperature: 采样温度参数；为空时使用 .env 中的默认配置。

        Returns:
            Iterator[Any]: 按增量返回的流式响应迭代器。
        """
        effective_temperature = self.default_temperature if temperature is None else temperature
        if self._protocol == "openai":
            return self._create_openai_chat(messages, tools, tool_choice, effective_temperature, stream=True)

        # Anthropic 在当前实现中回退为单次响应并包装为一个“伪流”分片，
        # 以兼容现有编排层的流式消费逻辑。
        normalized = self._create_anthropic_chat(messages, tools, tool_choice, effective_temperature)
        message = normalized.choices[0].message

        delta_tool_calls: list[Any] = []
        for index, tc in enumerate(message.tool_calls or []):
            delta_tool_calls.append(
                SimpleNamespace(
                    index=index,
                    id=tc.id,
                    function=SimpleNamespace(
                        name=tc.function.name,
                        arguments=tc.function.arguments,
                    ),
                )
            )

        chunk = SimpleNamespace(
            choices=[
                SimpleNamespace(
                    delta=SimpleNamespace(
                        content=message.content,
                        tool_calls=delta_tool_calls,
                    )
                )
            ]
        )
        return iter([chunk])


# 兼容旧名称，避免外部导入路径失效。
DeepSeekChatClient = LlmChatClient
