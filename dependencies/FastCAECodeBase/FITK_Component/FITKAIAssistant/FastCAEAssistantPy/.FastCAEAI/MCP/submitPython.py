 
from mcp.server.fastmcp import FastMCP

#: 当前脚本导出的 MCP 服务实例名称。
server = FastMCP("submit_python_server")


def _build_hostpy_command(command: str) -> str:
    return f"HOSTPY\t{command}"

 
 

@server.tool()
def submitPythonCommand(command: str) -> str:
    """返回可由宿主执行的 Python 命令字符串（HOSTPY 协议）。"""
    cmd = command.strip().replace("\n", ";").replace("\r", ";")
    return _build_hostpy_command(cmd)
 

def run_server() -> None:
    """以 stdio 传输方式启动当前 MCP 服务。"""
    server.run(transport="stdio")

if __name__ == "__main__":
    run_server()
