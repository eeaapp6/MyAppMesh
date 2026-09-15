"""FastCAEAssistant 的 MCP 服务发现工具。

该模块将 .FastCAEAI/MCP 下的每个 Python 脚本视为独立 stdio MCP 服务，
并转换为统一的启动描述。
"""

from __future__ import annotations

from dataclasses import dataclass
import os
from pathlib import Path
import sys
from typing import Any


_LAST_MCP_IMPORT_ERROR: str | None = None


@dataclass(frozen=True)
class McpServerSpec:
    """单个可发现 MCP 服务脚本的描述对象。"""

    #: 由脚本文件名推导得到的服务名称。
    name: str
    #: 承载 MCP 服务的 Python 脚本绝对路径。
    script_path: Path
    #: MCP 客户端连接该服务时使用的 stdio 启动参数。
    params: Any


def _python_command(project_root: Path) -> str:
    """返回启动 MCP 脚本优先使用的 Python 可执行路径。"""
    # 允许外部显式指定 MCP 子进程解释器（例如 PythonQt 宿主解释器）。
    configured = os.environ.get("FASTCAE_MCP_PYTHON", "").strip()
    if configured:
        return configured

    python_exec = project_root / ".venv" / "Scripts" / "python.exe"
    if python_exec.exists():
        return str(python_exec)

    # 在嵌入式宿主中，sys.executable 可能指向主程序 exe，
    # 若直接用于 MCP 子进程会触发“再启动一个主程序实例”。
    current_python = (sys.executable or "").strip()
    if current_python:
        exe_name = Path(current_python).name.lower()
        if exe_name.startswith("python"):
            return current_python

    return "python"


def _build_stdio_server_parameters(command: str, script: Path) -> Any | None:
    """延迟导入 MCP 类型，避免在缺少 pywintypes 时模块加载失败。"""
    global _LAST_MCP_IMPORT_ERROR
    _LAST_MCP_IMPORT_ERROR = None

    _ensure_pywin32_runtime()

    try:
        from mcp import StdioServerParameters
    except ModuleNotFoundError as exc:
        _LAST_MCP_IMPORT_ERROR = str(exc)
        return None

    return StdioServerParameters(command=command, args=[str(script)])


def _ensure_pywin32_runtime() -> None:
    """在嵌入式 Python 环境中补齐 pywin32 运行时路径。"""
    if os.name != "nt":
        return

    try:
        import pywintypes  # noqa: F401
        return
    except ModuleNotFoundError:
        pass

    # 优先尝试 pywin32 自带 bootstrap（若 .pth 未执行，此处可手动触发）。
    try:
        import pywin32_bootstrap  # type: ignore # noqa: F401
    except ModuleNotFoundError:
        pass

    try:
        import pywintypes  # noqa: F401
        return
    except ModuleNotFoundError:
        pass

    # 回退：手动将 pywin32 常用路径注入 sys.path。
    added_paths: list[str] = []
    for entry in list(sys.path):
        base = Path(entry)
        if not base.name.lower().endswith("site-packages"):
            continue
        for rel in ("win32", "win32/lib", "pywin32_system32"):
            candidate = base / rel
            if candidate.exists():
                candidate_str = str(candidate)
                if candidate_str not in sys.path:
                    sys.path.append(candidate_str)
                    added_paths.append(candidate_str)

    # 注入后再次尝试 bootstrap + import。
    try:
        import pywin32_bootstrap  # type: ignore # noqa: F401
    except ModuleNotFoundError:
        pass

    try:
        import pywintypes  # noqa: F401
    except ModuleNotFoundError:
        # 保持静默，由调用方通过 MCP 诊断信息提示用户。
        _ = added_paths


def discover_mcp_servers(project_root: Path) -> list[McpServerSpec]:
    """发现 .FastCAEAI/MCP 目录下的独立 MCP 服务脚本。"""
    servers, _ = discover_mcp_servers_with_diagnostics(project_root)
    return servers


def discover_mcp_servers_with_diagnostics(project_root: Path) -> tuple[list[McpServerSpec], str | None]:
    """发现 MCP 服务并返回诊断信息（无问题时为 None）。"""
    mcp_root = project_root / ".FastCAEAI" / "MCP"
    if not mcp_root.exists():
        return [], f"MCP 目录不存在: {mcp_root}"

    command = _python_command(project_root)
    discovered: list[McpServerSpec] = []

    for script in sorted(mcp_root.rglob("*.py")):
        if script.name == "__init__.py":
            continue

        params = _build_stdio_server_parameters(command, script)
        if params is None:
            # MCP 运行时不可用时，降级为不加载 MCP 服务，避免启动失败。
            detail = _LAST_MCP_IMPORT_ERROR or "unknown"
            return [], f"MCP 运行时不可用：{detail}"

        discovered.append(
            McpServerSpec(
                name=script.stem,
                script_path=script,
                params=params,
            )
        )

    return discovered, None
