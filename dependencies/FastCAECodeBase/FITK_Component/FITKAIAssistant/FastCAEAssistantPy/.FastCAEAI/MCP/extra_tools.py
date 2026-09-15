"""示例 MCP 通用工具服务。

该服务暴露时间、回显与字符串处理工具，用于验证非数学类工具调用链路。
"""

from datetime import datetime

from mcp.server.fastmcp import FastMCP

#: 当前脚本导出的 MCP 服务实例名称。
server = FastMCP("demo_utility_server")


@server.tool()
def current_time() -> str:
    """返回当前本地时间（ISO 格式，精确到秒）。"""
    return datetime.now().isoformat(timespec="seconds")


@server.tool()
def echo(text: str) -> str:
    """回显输入文本，用于快速连通性检查。"""
    return f"echo: {text}"


@server.tool()
def reverse_text(text: str) -> str:
    """返回输入字符串的反转结果。"""
    return text[::-1]


def run_server() -> None:
    """以 stdio 传输方式启动当前 MCP 服务。"""
    server.run(transport="stdio")


if __name__ == "__main__":
    run_server()
