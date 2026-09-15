"""加载并渲染 .FastCAEAI 工作区规范。

该模块负责发现 agent/skill 的 Markdown 文件，并转为可注入系统提示词的
类型化结构。
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class AgentSpec:
    """已加载的 agent 规范文档。"""

    #: 由文件名推导得到的 agent 标识。
    name: str
    #: 源 Markdown 文件的完整路径。
    path: Path
    #: 从磁盘读取的完整文档内容。
    content: str


@dataclass(frozen=True)
class SkillSpec:
    """已加载的 skill 规范文档。"""

    #: 技能标识，通常为目录名或文件名。
    name: str
    #: 源 Markdown 文件的完整路径。
    path: Path
    #: 从磁盘读取的完整文档内容。
    content: str
    #: 从首个非空行提取的简短摘要。
    summary: str


@dataclass(frozen=True)
class FastCAEWorkspaceContext:
    """从 .FastCAEAI 目录聚合得到的上下文。"""

    #: 已加载的 agent 规范列表。
    agents: list[AgentSpec]
    #: 已加载的 skill 规范列表。
    skills: list[SkillSpec]


def _read_text(path: Path) -> str:
    """读取 UTF-8 文本，遇到坏字节时自动降级容错。"""
    try:
        return path.read_text(encoding="utf-8").strip()
    except UnicodeDecodeError:
        return path.read_text(encoding="utf-8", errors="ignore").strip()


def _first_non_empty_line(text: str) -> str:
    """返回首个非空行，并裁剪长度用于摘要展示。"""
    for line in text.splitlines():
        stripped = line.strip()
        if stripped:
            return stripped[:160]
    return ""


def _load_agents(base_dir: Path) -> list[AgentSpec]:
    """从 .FastCAEAI/agents 目录加载全部 agent 文档。"""
    agents_dir = base_dir / "agents"
    if not agents_dir.exists():
        return []

    loaded: list[AgentSpec] = []
    for md_file in sorted(agents_dir.rglob("*.md")):
        content = _read_text(md_file)
        if not content:
            continue
        loaded.append(AgentSpec(name=md_file.stem, path=md_file, content=content))
    return loaded


def _load_skills(base_dir: Path) -> list[SkillSpec]:
    """从 .FastCAEAI/skills 目录加载 skill 文档。

    优先读取每个技能目录中的 SKILL.md；其余 Markdown 文件作为补充来源。
    """
    skills_dir = base_dir / "skills"
    if not skills_dir.exists():
        return []

    by_key: dict[str, SkillSpec] = {}

    for skill_file in sorted(skills_dir.rglob("*.md")):
        if skill_file.name.lower() != "skill.md":
            continue
        if skill_file.parent == skills_dir:
            # 跳过根 skills 目录下的聚合文档，避免其作为一个 skill 被加载。
            continue
        skill_name = skill_file.parent.name
        content = _read_text(skill_file)
        if not content:
            continue
        by_key[skill_name.lower()] = SkillSpec(
            name=skill_name,
            path=skill_file,
            content=content,
            summary=_first_non_empty_line(content),
        )

    for md_file in sorted(skills_dir.rglob("*.md")):
        if md_file.name.lower() == "skill.md":
            continue
        skill_name = md_file.stem
        key = skill_name.lower()
        if key in by_key:
            continue
        content = _read_text(md_file)
        if not content:
            continue
        by_key[key] = SkillSpec(
            name=skill_name,
            path=md_file,
            content=content,
            summary=_first_non_empty_line(content),
        )

    return sorted(by_key.values(), key=lambda x: x.name.lower())


def load_fastcae_workspace_context(project_root: Path) -> FastCAEWorkspaceContext:
    """从给定项目根目录加载全部 FastCAE agent/skill 文档。"""
    fastcae_root = project_root / ".FastCAEAI"
    if not fastcae_root.exists():
        return FastCAEWorkspaceContext(agents=[], skills=[])

    return FastCAEWorkspaceContext(
        agents=_load_agents(fastcae_root),
        skills=_load_skills(fastcae_root),
    )


def build_fastcae_spec_prompt(context: FastCAEWorkspaceContext, max_chars_per_doc: int = 1200) -> str:
    """将已加载规范渲染为可拼接到系统提示词的文本块。"""
    lines: list[str] = []

    if context.agents:
        lines.append("Loaded agent specs from .FastCAEAI/agents:")
        for agent in context.agents:
            lines.append(f"- Agent: {agent.name}")
            lines.append(f"  Source: {agent.path.as_posix()}")
            lines.append(f"  Spec snippet:\n{agent.content[:max_chars_per_doc]}")

    if context.skills:
        lines.append("Loaded skill specs from .FastCAEAI/skills:")
        for skill in context.skills:
            lines.append(f"- Skill: {skill.name}")
            lines.append(f"  Source: {skill.path.as_posix()}")
            lines.append(f"  Summary: {skill.summary or '(no summary line)'}")

    return "\n".join(lines).strip()
