"""Top-level launcher script for FastCAEAssistant CLI."""

import sys
import threading

from fastcaeassistant import create_fastcaeassistant_session
from fastcaeassistant import run_fastcaeassistant_cli

_SESSION = None


def _safe_print(text: str) -> None:
    payload = "" if text is None else str(text)
    try:
        print(payload, flush=True)
    except UnicodeEncodeError:
        encoding = getattr(sys.stdout, "encoding", None) or "utf-8"
        sanitized = payload.encode(encoding, errors="replace").decode(encoding, errors="replace")
        print(sanitized, flush=True)


def _flush_stdio() -> None:
    try:
        sys.stdout.flush()
    except Exception:
        pass
    try:
        sys.stderr.flush()
    except Exception:
        pass


def start(interactive: bool | None = None, project_root: str | None = None):
    """启动助手。PythonQt 中默认创建可复用会话并返回。"""
    global _SESSION

    if interactive is None:
        interactive = bool(sys.stdin) and bool(getattr(sys.stdin, "isatty", lambda: False)())

    try:
        if interactive:
            run_fastcaeassistant_cli(interactive=True)
            return None

        _SESSION = create_fastcaeassistant_session(stream_output=False, project_root=project_root)
        _safe_print("FastCAEAssistant 会话已就绪。")
        _safe_print(f"项目根目录: {_SESSION.project_root}")
        _safe_print(f"已加载 Agent({ _SESSION.loaded_agent_count }):")
        for name in _SESSION.loaded_agent_names:
            _safe_print(f" {name}")
        _safe_print(f"已加载 Skill({ _SESSION.loaded_skill_count }):")
        for name in _SESSION.loaded_skill_names:
            _safe_print(f" {name}")
        _safe_print(f"已加载 MCP({ _SESSION.loaded_mcp_server_count }):")
        for name in _SESSION.loaded_mcp_server_names:
            _safe_print(f" {name}")
        if _SESSION.loaded_mcp_server_count == 0 and _SESSION.mcp_diagnostics:
            _safe_print(f"MCP 诊断: {_SESSION.mcp_diagnostics}")
        _safe_print("可调用 clear() 清空历史，stop() 关闭会话。")
        return _SESSION
    except Exception as exc:
        _safe_print(f"运行 demo 失败：{exc}")
        assert False, "run_fastcaeassistant_cli 启动异常"

 
def ask(query: str, stream_output: bool = False, cancel_event: threading.Event | None = None) -> str:
    """在当前会话中执行一轮问答。"""
    global _SESSION
    if _SESSION is None:
        _SESSION = create_fastcaeassistant_session(stream_output=False)

    result = _SESSION.ask(query, stream_output=stream_output, cancel_event=cancel_event)
    if result.final_answer and not stream_output:
        _safe_print(result.final_answer)
    return result.final_answer


def clear() -> None:
    """清空会话历史。"""
    global _SESSION
    if _SESSION is not None:
        _SESSION.clear_history()
    _safe_print("对话历史已清空。")


def stop() -> None:
    """关闭当前会话。"""
    global _SESSION
    _SESSION = None
    _safe_print("FastCAEAssistant 会话已关闭。")


def run_stdio_bridge() -> None:
    """通过标准输入输出与 C++ 端通信。"""
    if hasattr(sys.stdout, "reconfigure"):
        try:
            sys.stdout.reconfigure(encoding="utf-8", errors="replace", line_buffering=True, write_through=True)
        except Exception:
            pass
    if hasattr(sys.stderr, "reconfigure"):
        try:
            sys.stderr.reconfigure(encoding="utf-8", errors="replace", line_buffering=True, write_through=True)
        except Exception:
            pass

    start(interactive=False)
    _safe_print("[BRIDGE] READY")
    _flush_stdio()

    hostpy_protocol_hint = (
        "如果需要宿主执行Python命令，必须先通过 MCP/skill 工具获取命令字符串。" 
        "HOSTPY 命令由工具生成并必须原样透传（例如 HOSTPY\\t<python_command>）。"
        "若没有可用工具结果，请直接说明无法执行，不要输出 HOSTPY。"
        "输出宿主命令时，请严格输出单独一行: HOSTPY\\t<python_command>。"
        "如宿主命令执行错误，可以按照错误提示进行修改后再次尝试。"
        "拿到宿主返回的 HOSTPY_RESULT 后，评估是继续输出下一条 HOSTPY 命令，还是输出最终答案。"
        "全部命令执行完毕后，请输出最终总结后结束本轮问答。"
    )

    demo_math_query = (
        "请做一个最小闭环验证：先调用 demo_math_server 的 add 工具计算 2+3。"
        "然后按顺序执行两条宿主Python命令并评估结果："
        "第1条输出 HOSTPY\\tx=2+3；第2条输出 HOSTPY\\tx*10；"
        "最后给出最终结论。每次只输出一条 HOSTPY 命令或最终答案。"
    )

    active_worker: threading.Thread | None = None
    stop_event = threading.Event()

    def _is_running() -> bool:
        return active_worker is not None and active_worker.is_alive()

    def _run_query(query: str) -> None:
        try:
            ask(f"{query}\\n\\n{hostpy_protocol_hint}", stream_output=True, cancel_event=stop_event)
        except Exception as exc:
            _safe_print(f"[BRIDGE][ERROR] {exc}")
        finally:
            if stop_event.is_set():
                _safe_print("[BRIDGE] STOPPED")
            else:
                _safe_print("[BRIDGE] END")
            _flush_stdio()

    for raw in sys.stdin:
        line = raw.strip()
        if not line:
            continue

        try:
            if line == "STOP":
                if _is_running():
                    stop_event.set()
                    _safe_print("[BRIDGE] STOPPING")
                else:
                    _safe_print("[BRIDGE] STOPPED")
                _flush_stdio()
                continue

            if line == "CLEAR":
                clear()
                _safe_print("[BRIDGE] CLEARED")
                _flush_stdio()
                continue

            if line.startswith("ASK\t"):
                query = line.split("\t", 1)[1].strip()
            else:
                query = line

            if line.startswith("HOSTPY_RESULT\t"):
                hostpy_result = line.split("\t", 1)[1].strip()
                query = (
                    f"宿主Python命令执行结果: {hostpy_result}\\n"
                    "请判断是否需要修正命令或执行下一条命令。"
                    "若需要继续执行宿主命令，必须先调用 MCP/skill 获取命令字符串，"
                    "并且只输出一行工具原样返回的签名 HOSTPY 命令；"
                    "若已完成任务，请直接给出最终答案。"
                )

            if query == "@demo_hostpy_math":
                query = demo_math_query

            if not query:
                _safe_print("[BRIDGE] EMPTY_QUERY")
                _flush_stdio()
                continue

            if _is_running():
                _safe_print("[BRIDGE] BUSY")
                _flush_stdio()
                continue

            stop_event.clear()
            active_worker = threading.Thread(target=_run_query, args=(query,), daemon=True)
            active_worker.start()
        except Exception as exc:
            _safe_print(f"[BRIDGE][ERROR] {exc}")
            _flush_stdio()

if __name__ == "__main__":
    if "--bridge-stdio" in sys.argv:
        run_stdio_bridge()
    else:
        run_fastcaeassistant_cli()
