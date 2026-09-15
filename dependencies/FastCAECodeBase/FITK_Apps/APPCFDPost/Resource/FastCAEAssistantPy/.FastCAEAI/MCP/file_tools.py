"""文件相关 MCP 工具服务。

该服务文件相关的功能服务。

"""
import json
from pathlib import Path

from mcp.server.fastmcp import FastMCP

#: 当前脚本导出的 MCP 服务实例名称。
server = FastMCP("MCP Files")
def _build_hostpy_command(command: str) -> str:
    return f"HOSTPY\t{command}"
    
@server.tool()
def importVTKFile(file: str) -> str:
    """导入 单个VTK文件 或 包含vtk文件的文件夹。"""
    raw = (file or "").strip()
    if not raw:
        return "ERROR: file path is empty."
    resolved = Path(raw).expanduser().resolve(strict=False)
    if not resolved.exists():
        return f"ERROR: file not found: {resolved}"
    if resolved.is_file() and resolved.suffix.lower() != ".vtk":
        return f"ERROR: unsupported file type: {resolved}"
    if resolved.is_dir() and not any(child.suffix.lower() == ".vtk" for child in resolved.iterdir() if child.is_file()):
        return f"ERROR: no vtk files found in directory: {resolved}"
    # 使用 JSON 字符串字面量生成双引号包裹路径，兼容 Windows 反斜杠。
    path_literal = json.dumps(str(resolved), ensure_ascii=False)
    command = f"Global.Files.OpenVTKFiles({path_literal})"
    return _build_hostpy_command(command)
    
    
def run_server() -> None:
    """以 stdio 传输方式启动当前 MCP 服务。"""
    server.run(transport="stdio")
    
if __name__ == "__main__":
    run_server()