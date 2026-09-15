"""几何相关 MCP 工具服务。

该服务提供 OpenRadioss 几何相关的功能服务。
"""
import json
from pathlib import Path

from mcp.server.fastmcp import FastMCP

#: 当前脚本导出的 MCP 服务实例名称。
server = FastMCP("Radioss Geometry")


def _build_hostpy_command(command: str) -> str:
    return f"HOSTPY\t{command}"


@server.tool()
def createBox(x_p: float, y_p: float, z_p: float, x_l: float, y_l: float, z_l: float) -> str:
    """创建长方体。

    Args:
        x_p: 基准点X坐标（float）
        y_p: 基准点Y坐标（float）
        z_p: 基准点Z坐标（float）
        x_l: X方向长度（float）
        y_l: Y方向长度（float）
        z_l: Z方向长度（float）
    """

    command = f"Global.Geometrys().createBox({x_p}, {y_p}, {z_p}, {x_l}, {y_l}, {z_l})"
    return _build_hostpy_command(command)


@server.tool()
def createSphere(x_c: float, y_c: float, z_c: float, radius: float) -> str:
    """创建球体。

    Args:
        x_c: 中心点X坐标（float）
        y_c: 中心点Y坐标（float）
        z_c: 中心点Z坐标（float）
        radius: 球体半径（float）
    """

    command = f"Global.Geometrys().createSphere({x_c}, {y_c}, {z_c}, {radius})"
    return _build_hostpy_command(command)


@server.tool()
def createCylinder(x_c: float, y_c: float, z_c: float, x_d: float, y_d: float, z_d: float, radius: float, length: float) -> str:
    """创建圆柱体。

    Args:
        x_c: 基准点X坐标（float）
        y_c: 基准点Y坐标（float）
        z_c: 基准点Z坐标（float）
        x_d: 方向X分量（float）
        y_d: 方向Y分量（float）
        z_d: 方向Z分量（float）
        radius: 圆柱体半径（float）
        length: 圆柱体长度（float）
    """

    command = f"Global.Geometrys().createCylinder({x_c}, {y_c}, {z_c}, {x_d}, {y_d}, {z_d}, {radius}, {length})"
    return _build_hostpy_command(command)


@server.tool()
def editGeomertyName(ori_name: str, new_name: str) -> str:
    """修改几何名称。

    Args:
        ori_name: 原名称
        new_name: 新名称 
    """

    if not ori_name:
        return "ERROR: ori_name is empty."
    if not new_name:
        return "ERROR: new_name is empty."

    ori_name = json.dumps(ori_name, ensure_ascii=False)
    new_name = json.dumps(new_name, ensure_ascii=False)
    command = f"Global.Geometrys.GetGeometry({ori_name}).setName({new_name})"
    return _build_hostpy_command(command)


def run_server() -> None:
    """以 stdio 传输方式启动当前 MCP 服务。"""
    server.run(transport="stdio")


if __name__ == "__main__":
    run_server()
