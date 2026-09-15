"""FastCAEAssistant 包导出入口。

该模块统一暴露主 CLI 启动函数，外部调用方无需依赖内部模块路径。
"""

from fastcaeassistant.cli import FastCAEAssistantSession
from fastcaeassistant.cli import create_fastcaeassistant_session
from fastcaeassistant.cli import run_fastcaeassistant_cli

__all__ = [
	"run_fastcaeassistant_cli",
	"create_fastcaeassistant_session",
	"FastCAEAssistantSession",
]
