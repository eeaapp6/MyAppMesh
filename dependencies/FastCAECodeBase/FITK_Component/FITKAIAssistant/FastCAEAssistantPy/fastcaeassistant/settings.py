"""FastCAEAssistant 运行时配置加载模块。

该模块负责读取环境变量并生成类型化配置对象，供其他模块统一使用。
"""

from __future__ import annotations

import os
import re
from dataclasses import dataclass
import math
from pathlib import Path
from typing import Literal

from dotenv import dotenv_values, load_dotenv

_ENV_KEY_PATTERN = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


@dataclass(frozen=True)
class AssistantSettings:
    """多协议 LLM 连接与模型选择的不可变运行时配置。"""

    #: LLM 协议类型，当前支持 openai 与 anthropic。
    llm_protocol: Literal["openai", "anthropic"]
    #: LLM 访问密钥。
    llm_api_key: str
    #: LLM 模型名称。
    llm_model: str
    #: LLM 服务基础地址（支持私有网关或代理）。
    llm_base_url: str
    #: LLM 采样温度，用于控制生成结果的随机性。
    llm_temperature: float


def load_assistant_settings(project_root: Path | None = None) -> AssistantSettings:
    """从 .env 与进程环境变量中加载并校验配置。

    Args:
        project_root: 可选的项目根目录，用于定位 .env 文件。

    Returns:
        AssistantSettings: 解析并校验后的运行时配置。

    Raises:
        RuntimeError: 缺少关键配置项或协议值非法时抛出。
    """
    root = project_root or Path(__file__).resolve().parent.parent
    env_path = root / ".env"

    if not env_path.exists():
        raise RuntimeError("Missing .env file. Please create it from .env.example.")

    # 使用 override=True，确保 .env 中的值能覆盖当前进程里可能存在的旧值。
    load_dotenv(dotenv_path=env_path, override=True)

    # 兼容带 BOM 的 .env 文件键名（例如 "\ufeffDEEPSEEK_API_KEY"）。
    dotenv_data = dotenv_values(env_path)
    for raw_key, raw_value in dotenv_data.items():
        if not raw_key:
            continue
        normalized_key = raw_key.lstrip("\ufeff")
        if normalized_key != raw_key and normalized_key and _ENV_KEY_PATTERN.match(normalized_key):
            os.environ[normalized_key] = raw_value or ""

    missing_keys: list[str] = []

    def _required_env(key_name: str) -> str:
        value = os.getenv(key_name, "").strip()
        if not value:
            missing_keys.append(key_name)
        return value

    def _optional_temperature(key_name: str, default_value: float) -> float:
        raw_value = os.getenv(key_name, "").strip()
        if not raw_value:
            return default_value

        try:
            parsed_value = float(raw_value)
        except ValueError as exc:
            raise RuntimeError(f"Invalid {key_name}. Expected a number in range [0, 2].") from exc

        if not math.isfinite(parsed_value) or parsed_value < 0.0 or parsed_value > 2.0:
            raise RuntimeError(f"Invalid {key_name}. Expected a number in range [0, 2].")

        return parsed_value

    protocol = _required_env("LLM_PROTOCOL").lower()
    api_key = _required_env("LLM_API_KEY")
    model = _required_env("LLM_MODEL")
    base_url = _required_env("LLM_BASE_URL")
    temperature = _optional_temperature("LLM_TEMPERATURE", 0.0)

    if missing_keys:
        raise RuntimeError(
            "Invalid .env format. Missing required fields: " + ", ".join(missing_keys) + "."
        )

    if protocol not in {"openai", "anthropic"}:
        raise RuntimeError("Invalid LLM_PROTOCOL. Supported values: openai, anthropic.")

    return AssistantSettings(
        llm_protocol=protocol,
        llm_api_key=api_key,
        llm_model=model,
        llm_base_url=base_url,
        llm_temperature=temperature,
    )
