# FITKAIAssistant

## 1. 模块简介

FITKAIAssistant 是 APPRadioss 中的 AI 助手组件模块，提供以下能力：

- 在主界面中提供 AI 对话窗口（上方输出、下方输入与提交）。
- 启动并管理 Python 侧 FastCAEAssistant 进程。
- 通过 stdio 协议进行 C++ 与 Python 双向通信。
- 解析并执行由 MCP/skill 生成的 HOSTPY 命令。
- 将宿主执行结果回传给 agent，形成多轮闭环决策。

该模块以动态库形式构建，目标名称为 FITKAIAssistant。

## 2. 目录与核心文件

- FITKAIAssistantAPI.h
  - 导出宏声明。
- FITKAIAssistantInterface.h/.cpp
  - 组件接口实现，向框架暴露组件名 FITKAIAssistant。
- FITKAIAssistantWindow.h/.cpp
  - 组件 UI 窗口，负责用户输入、输出展示、桥接信号连接。
- FITKAIPythonAgentLauncher.h/.cpp
  - Python 进程启动与桥接核心，负责协议解析、HOSTPY 执行与回传。
- FITKPythonExecutor.h/.cpp
  - 宿主 Python 执行器，基于 FITKPythonInterface 执行命令并产出结果文本。
- CMakeLists.txt
  - 本模块构建与链接配置。

## 3. 架构关系

整体调用链如下：

1. UI 层
   - FITKAIAssistantWindow 接收用户输入。
2. 桥接层
   - FITKAIPythonAgentLauncher 启动 FastCAEAssistant.py 并通过 stdio 通信。
3. Agent/MCP 层
   - Python agent 调用 MCP 工具，必要时产出签名 HOSTPY 命令。
4. 宿主执行层
   - FITKAIPythonAgentLauncher 解析 HOSTPY 命令后调用 FITKPythonExecutor。
   - FITKPythonExecutor 通过 FITKPythonInterface 执行宿主 Python 命令。
5. 结果回传
   - 执行结果以 HOSTPY_RESULT 返回给 Python agent，继续后续决策。

## 4. 协议说明

### 4.1 C++ -> Python 命令

- ASK\t<query>
  - 请求 agent 处理用户问题。
- CLEAR
  - 清空 agent 历史。
- STOP
  - 结束会话。

### 4.2 Python -> C++ 命令

- HOSTPY\t<python_command>
   - 表示需要宿主执行 Python 命令。
   - command 为工具生成并透传的单行脚本文本。

### 4.3 C++ -> Python 执行回传

- HOSTPY_RESULT\t<result_text>
  - 宿主执行结果回传给 agent。
  - 若失败，回传 ERROR: ...。

## 5. 安全与防臆造机制

为避免桥接层误处理，本模块采用固定前缀与回传约定：

1. 固定协议前缀
   - 仅处理以 HOSTPY\t 开头的整行输出。
2. 命令文本清洗
   - 过滤 [BRIDGE]、HOSTPY_RESULT 等桥接噪声后执行。
3. 统一结果回写
   - 通过 HOSTPY_RESULT\t<result_text> 回传执行结果，失败回传 ERROR: ...。

## 6. 结果可用性策略

为确保 agent 可以继续下一步决策：

- FITKPythonExecutor 使用 submit(command, QVariant&) 获取返回值。
- 若脚本是语句型且无返回值，回传非空兜底文本：
  - OK: Executed <command>

这样可避免出现仅显示 Script executed 但 HOSTPY_RESULT 为空导致流程中断。

## 7. 构建说明

本模块由 CMakeLists.txt 构建为共享库，关键依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- FITKAppFramework
- FITKPython

输出目录：

- Debug: output/bin_d
- Release: output/bin

## 8. 运行时要求

运行时需要以下资产可用：

1. Python 可执行文件
   - Windows: <appDir>/python.exe
   - Linux: <appDir>/python
2. Agent 脚本
   - <appDir>/FastCAEAssistantPy/FastCAEAssistant.py
3. .FastCAEAI 目录
   - 含 MCP、agents、skills 等配置与脚本。

## 9. 常见问题排查

1. 提示 Python executable not found
   - 检查应用目录下 python 可执行文件是否存在。
2. 提示 Assistant script not found
   - 检查 FastCAEAssistant.py 路径是否正确。
3. 出现 Script executed 但 agent 不继续
   - 确认 HOSTPY_RESULT 是否为非空文本。

## 10. 接入约定

- 组件名称固定为 FITKAIAssistant。
- 应用层通过 ComponentFactory 注册该接口实例。
- UI 层建议以右侧 dock 方式集成，避免破坏主工作区布局。

---

最后更新：2026-05-15
