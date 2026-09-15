"""示例 MCP 数学工具服务。

该服务通过 FastMCP 暴露基础数学运算工具，用于演示多 MCP 服务自动加载与调用。
"""

from mcp.server.fastmcp import FastMCP

#: 当前脚本导出的 MCP 服务实例名称。
server = FastMCP("demo_math_server")


def _build_hostpy_command(command: str) -> str:
    return f"HOSTPY\t{command}"

@server.tool()
def add(a: int, b: int) -> int:
    """返回两个整数之和。"""
    return a + b


@server.tool()
def multiply(a: int, b: int) -> int:
    """返回两个整数之积。"""
    return a * b


@server.tool()
def subtract(a: int, b: int) -> int:
    """返回两个整数之差。"""
    return a - b


@server.tool()
def square(n: int) -> int:
    """返回整数 n 的平方。"""
    return n * n

@server.tool()
def hostpy_assign(name: str, value: str) -> str:
    """返回一个变量赋值命令字符串（HOSTPY 协议）。"""
    var_name = name.strip().replace(" ", "")
    rhs = value.strip().replace("\n", " ").replace("\r", " ")
    return _build_hostpy_command(f"{var_name} = {rhs}")


def run_server() -> None:
    """以 stdio 传输方式启动当前 MCP 服务。"""
    server.run(transport="stdio")


if __name__ == "__main__":
    run_server()
