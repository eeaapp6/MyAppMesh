"""文件相关 MCP 工具服务。

该服务文件相关的功能服务。
"""
import json
from pathlib import Path

from mcp.server.fastmcp import FastMCP

#: 当前脚本导出的 MCP 服务实例名称。
server = FastMCP("File Tools")


def _build_hostpy_command(command: str) -> str:
    return f"HOSTPY\t{command}"

@server.tool()
def exportGeoFile(file: str) -> str:
    """导出 几何文件 """
    raw = (file or "").strip()
    if not raw:
        return "ERROR: file path is empty."
    resolved = Path(raw).expanduser().resolve(strict=False)
    parent = resolved.parent
    if not parent.exists():
        return f"ERROR: parent directory not found: {parent}"
    # 使用 JSON 字符串字面量生成双引号包裹路径，兼容 Windows 反斜杠。
    path_literal = json.dumps(str(resolved), ensure_ascii=False)
    command = f"Global.Files.ExportGeoFile({path_literal})"
    return _build_hostpy_command(command)
    





def run_server() -> None:
    """以 stdio 传输方式启动当前 MCP 服务。"""
    server.run(transport="stdio")




if __name__ == "__main__":
    run_server()
