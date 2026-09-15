"""网格相关 MCP 工具服务。

该服务提供 OpenRadioss 网格相关的功能服务。
"""
import json
from pathlib import Path

from mcp.server.fastmcp import FastMCP

#: 当前脚本导出的 MCP 服务实例名称。
server = FastMCP("Radioss Mesh")


def _build_hostpy_command(command: str) -> str:
    return f"HOSTPY\t{command}"


@server.tool()
def editMeshName(ori_name: str, new_name: str) -> str:
    """修改网格名称。

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
    command = f"Global.Meshs.GetMesh({ori_name}).setName({new_name})"
    return _build_hostpy_command(command)


@server.tool()
def setMeshProperty(mesh_name: str, property_name: str) -> str:
    """设置网格属性。

    Args:
        mesh_name: 网格名称
        property_name: 属性名称
    """

    if not mesh_name:
        return "ERROR: mesh_name is empty."
    if not property_name:
        return "ERROR: property_name is empty."

    mesh_name = json.dumps(mesh_name, ensure_ascii=False)
    property_name = json.dumps(property_name, ensure_ascii=False)
    command = f"Global.Meshs.GetMesh({mesh_name}).setProperty({property_name})"
    return _build_hostpy_command(command)


@server.tool()
def setMeshMaterial(mesh_name: str, material_name: str) -> str:
    """设置网格材料。

    Args:
        mesh_name: 网格名称
        material_name: 材料名称
    """

    if not mesh_name:
        return "ERROR: mesh_name is empty."
    if not material_name:
        return "ERROR: material_name is empty."

    mesh_name = json.dumps(mesh_name, ensure_ascii=False)
    material_name = json.dumps(material_name, ensure_ascii=False)
    command = f"Global.Meshs.GetMesh({mesh_name}).setMaterial({material_name})"
    return _build_hostpy_command(command)


@server.tool()
def meshToSPHParticleConversion(mesh_name: str) -> str:
    """网格转化 SPH 粒子。

    Args:
        mesh_name: 网格名称
    """

    if not mesh_name:
        return "ERROR: mesh_name is empty."

    mesh_name = json.dumps(mesh_name, ensure_ascii=False)
    command = f"Global.Meshs.MeshToSPHParticleConversion({mesh_name})"
    return _build_hostpy_command(command)


def run_server() -> None:
    """以 stdio 传输方式启动当前 MCP 服务。"""
    server.run(transport="stdio")


if __name__ == "__main__":
    run_server()