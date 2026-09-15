"""MCP 连通性冒烟检查工具。

该模块对已发现的 MCP 服务执行轻量端到端检查，并验证代表性工具可调用。
"""

from __future__ import annotations

from contextlib import AsyncExitStack
from dataclasses import dataclass
from pathlib import Path
from typing import Any

from mcp import ClientSession
from mcp.client.stdio import stdio_client

from fastcaeassistant.mcp_registry import McpServerSpec, discover_mcp_servers


@dataclass(frozen=True)
class McpSmokeResult:
    """冒烟检查结果汇总。"""

    #: 在全部 MCP 服务中发现到的工具名并集。
    tools: list[str]
    #: 调用 add 工具的原始结果对象。
    add_result: Any
    #: 调用 current_time 工具的原始结果对象。
    time_result: Any


def _find_tool_session(
    tool_name: str,
    index: list[tuple[McpServerSpec, ClientSession, list[str]]],
) -> ClientSession | None:
    """在已连接会话中查找包含目标工具的会话。"""
    for _, session, names in index:
        if tool_name in names:
            return session
    return None


async def run_mcp_smoke_check(project_root: Path | None = None) -> McpSmokeResult:
    """对已发现服务执行端到端 MCP 冒烟检查。

    该检查会验证服务发现结果，并调用 add 与 current_time 两个工具，
    确认基础计算与通用工具链路可用。
    """
    root = project_root or Path(__file__).resolve().parent.parent
    servers = discover_mcp_servers(root)
    if not servers:
        raise RuntimeError("No MCP servers found in .FastCAEAI/MCP")

    async with AsyncExitStack() as stack:
        index: list[tuple[McpServerSpec, ClientSession, list[str]]] = []
        all_tools: set[str] = set()

        for server in servers:
            read_stream, write_stream = await stack.enter_async_context(stdio_client(server.params))
            session = await stack.enter_async_context(ClientSession(read_stream, write_stream))
            await session.initialize()

            tools_resp = await session.list_tools()
            names = [tool.name for tool in tools_resp.tools]
            index.append((server, session, names))
            all_tools.update(names)

        add_session = _find_tool_session("add", index)
        if add_session is None:
            raise RuntimeError("Tool 'add' not found in discovered MCP servers.")
        add_result = await add_session.call_tool("add", {"a": 7, "b": 5})

        time_session = _find_tool_session("current_time", index)
        if time_session is None:
            raise RuntimeError("Tool 'current_time' not found in discovered MCP servers.")
        time_result = await time_session.call_tool("current_time", {})

        return McpSmokeResult(
            tools=sorted(all_tools),
            add_result=add_result,
            time_result=time_result,
        )
