"""文件相关 MCP 工具服务。

该服务OpenRadioss相关的功能服务。
"""
import json
from pathlib import Path

from mcp.server.fastmcp import FastMCP

#: 当前脚本导出的 MCP 服务实例名称。
server = FastMCP("Radioss Tools")


def _build_hostpy_command(command: str) -> str:
    return f"HOSTPY\t{command}"


@server.tool()
def ImportRadFile(file: str) -> str:
    """导入 rad 文件。"""
    raw = (file or "").strip()
    if not raw:
        return "ERROR: file path is empty."

    resolved = Path(raw).expanduser().resolve(strict=False)
    if not resolved.exists() or not resolved.is_file():
        return f"ERROR: file not found: {resolved}"

    # 使用 JSON 字符串字面量生成双引号包裹路径，兼容 Windows 反斜杠。
    path_literal = json.dumps(str(resolved), ensure_ascii=False)
    command = f"Global.Files.ImportRadFile({path_literal})"
    return _build_hostpy_command(command)

@server.tool()
def ImportMeshFile(file: str) -> str:
    """导入 网格（*.inp; *.bdf; *.nas） 文件。"""
    raw = (file or "").strip()
    if not raw:
        return "ERROR: file path is empty."

    resolved = Path(raw).expanduser().resolve(strict=False)
    if not resolved.exists() or not resolved.is_file():
        return f"ERROR: file not found: {resolved}"

    # 使用 JSON 字符串字面量生成双引号包裹路径，兼容 Windows 反斜杠。
    path_literal = json.dumps(str(resolved), ensure_ascii=False)
    command = f"Global.Files.ImportMeshFile({path_literal})"
    return _build_hostpy_command(command)

@server.tool()
def SaveProject(file: str) -> str:
    """保存项目文件。"""
    raw = (file or "").strip()
    if not raw:
        return "ERROR: file path is empty."

    resolved = Path(raw).expanduser().resolve(strict=False)

    # 使用 JSON 字符串字面量生成双引号包裹路径，兼容 Windows 反斜杠。
    path_literal = json.dumps(str(resolved), ensure_ascii=False)
    command = f"Global.Files.SaveProject({path_literal})"
    return _build_hostpy_command(command)


@server.tool()
def GetVTKResultFilePath() -> str:
    """获取当前项目的 VTK 结果文件路径。"""
    command = "Global.Files.GetVTKResultFilePath()"
    return _build_hostpy_command(command)


def run_server() -> None:
    """以 stdio 传输方式启动当前 MCP 服务。"""
    server.run(transport="stdio")




if __name__ == "__main__":
    run_server()
