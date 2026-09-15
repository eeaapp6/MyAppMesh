# APPCFDPost-master - 基于 FITK 的 CFD 后处理应用仓库

## 简介

APPCFDPost-master 是一个基于 FITK 体系构建的 CFD 后处理桌面应用仓库，用于组织后处理数据对象、渲染窗口、参数对话框、业务操作器和应用程序入口，最终形成可运行的 CFD 结果可视化与分析程序。

从当前仓库结构和顶层 CMake 看，它不是单一模块工程，而是一个完整的应用程序集成仓库：

- `FITK_Kernel` 提供底座与应用框架。
- `FITK_Interface` 提供 CFD 后处理对象接口和 VTK 算法封装。
- `FITK_Component` 提供渲染窗口、二维图表和通用组件。
- `PostGraphAdaptor`、`OperatorsModel`、`OperatorsGUI`、`OperatorsInterface` 负责后处理对象到界面行为的桥接。
- `GUIFrame`、`GUIWidget`、`GUIDialog` 负责图形界面承载、控件复用和参数输入。
- `CFDPostAPP` 负责主程序入口和运行时装配。

如果把仓库整体看作一个应用，那么它解决的是“如何把 CFD 结果导入、组织、显示、交互和分析整合为一个桌面程序”的问题。

## 仓库定位

该仓库位于 FastCAE/FITK 应用程序集成层，主要承担以下职责：

- 提供完整的 CFD 后处理桌面程序源码。
- 统一管理底座、接口、组件、图形适配和操作器模块。
- 提供基于 Qt 与 VTK 的图形界面与渲染能力。
- 提供导入、裁剪、切片、流线、阈值、变形、探针、灯光和色库等后处理功能。
- 通过 CMake、qmake 和 Visual Studio 工程文件兼容多种工程组织方式。

从顶层 `CMakeLists.txt` 可确认，当前仓库以 `project(CFDPost VERSION 1.0.0 LANGUAGES CXX)` 组织，并通过 `add_subdirectory(...)` 纳入所有子模块共同构建。

## 整体架构

可以把当前仓库理解为六层结构：

1. 底座层：`FITK_Kernel`
2. 接口与算法层：`FITK_Interface`
3. 通用组件层：`FITK_Component`
4. 图形适配与操作器层：`PostGraphAdaptor`、`OperatorsInterface`、`OperatorsGUI`、`OperatorsModel`
5. 界面层：`GUIFrame`、`GUIWidget`、`GUIDialog`
6. 应用入口层：`CFDPostAPP`

这六层的核心关系是：

- `FITKCore` 定义对象模型、仓库、图形抽象和线程能力。
- `FITKAppFramework` 把底座能力组织成可运行应用。
- `FITKInterfaceCFDPost` 定义 CFD 后处理数据对象体系。
- `FITKVTKAlgorithm` 提供 VTK 算法与图元辅助。
- `FITKRenderWindowVTK` 和 `FITKPlotWindow` 提供三维与二维显示能力。
- `PostGraphAdaptor` 把后处理对象映射为渲染对象与交互部件。
- `OperatorsModel` 执行业务逻辑，`OperatorsGUI` 处理界面交互，`OperatorsInterface` 提供统一抽象基类。
- `GUIFrame`、`GUIWidget`、`GUIDialog` 共同构成主界面、属性面板、工具条和参数对话框。
- `CFDPostAPP` 最终把这些模块链接并启动为桌面程序。

## 仓库结构

```text
APPCFDPost-master/
├── CMakeLists.txt                 # 顶层 CMake 入口
├── CFDPost.pro                    # 兼容 qmake
├── CFDPostAPP/                    # 主程序入口与运行时装配
├── FITK_Kernel/                   # 底座与应用框架
│   ├── FITKCore/
│   ├── FITKAppFramework/
│   └── FITKEasyParam/
├── FITK_Interface/                # 接口与算法层
│   ├── FITKInterfaceCFDPost/
│   └── FITKVTKAlgorithm/
├── FITK_Component/                # 通用组件层
│   ├── FITKRenderWindowVTK/
│   ├── FITKPlotWindow/
│   └── FITKWidget/
├── PostGraphAdaptor/              # 后处理图形适配层
├── OperatorsInterface/            # 操作器公共接口
├── OperatorsGUI/                  # 界面交互操作器
├── OperatorsModel/                # 后处理业务操作器
├── GUIFrame/                      # 主界面框架
├── GUIWidget/                     # 通用界面控件
├── GUIDialog/                     # 参数与功能对话框
├── Resource/                      # 资源文件
├── Tools/                         # 三方工具与 cmake 配置
├── output/                        # 统一输出目录
└── generate/                      # 自动生成文件输出目录
```

## 核心模块说明

### 1. FITK_Kernel

`FITK_Kernel` 是仓库底座层，包含：

- `FITKCore`：对象模型、数据仓库、操作器仓库、图形抽象、线程任务和基础工具。
- `FITKAppFramework`：应用生命周期、主窗口生成、全局数据工厂、组件工厂、命令行处理、工作台、插件和运行期扩展。
- `FITKEasyParam`：参数对象抽象、参数集合管理和参数控件工厂。

这一层决定了上层模块如何共享统一的对象与运行机制。

### 2. FITK_Interface

`FITK_Interface` 是接口与算法层，当前与 CFD 后处理直接相关的两个关键模块是：

- `FITKInterfaceCFDPost`：定义稳态、非稳态和派生后处理对象，以及灯光、色库和读取适配。
- `FITKVTKAlgorithm`：提供 VTK 数据处理、变换、Actor 封装和对比工具。

这一层负责把底层算法和结果对象组织成上层可调用的统一接口。

### 3. FITK_Component

`FITK_Component` 是通用组件层，当前仓库主要使用：

- `FITKRenderWindowVTK`：三维渲染窗口组件。
- `FITKPlotWindow`：二维曲线、频谱和极坐标图表组件。
- `FITKWidget`：通用 Qt 控件与业务辅助对话框。

这一层为界面层和适配层提供可直接嵌入的图形显示能力。

### 4. 图形适配与操作器层

这一层由四个模块协作完成：

- `PostGraphAdaptor`：把后处理对象映射为渲染对象、灯光对象和交互部件。
- `OperatorsInterface`：定义图形事件、图形交互和参数界面相关操作器抽象。
- `OperatorsGUI`：处理树点击、拾取和预览等界面交互。
- `OperatorsModel`：执行导入、删除、裁剪、切片、流线、阈值、变形、探针等业务逻辑。

这一层是“数据对象如何变成可见结果并响应交互”的关键桥梁。

### 5. GUI 层

GUI 层由三个模块组成：

- `GUIFrame`：主窗口、树视图、渲染窗口和 action 事件处理骨架。
- `GUIWidget`：属性面板、信息面板、工具条、拾取信息和探针曲线等通用控件。
- `GUIDialog`：切片、裁剪、流线、等值面、阈值、变形、探针和色库等功能对话框。

这一层负责将操作器和图形适配能力转化为最终用户可交互的界面。

### 6. CFDPostAPP

`CFDPostAPP` 是应用入口层，负责：

- 提供 `main.cpp`。
- 生成主窗口。
- 装配组件工厂、全局数据工厂、命令行处理器和工作台处理器。
- 链接整个仓库中需要参与运行的模块。

它是当前仓库最终形成可执行程序的直接入口。

## 典型运行链路

从当前模块关系看，一个典型的后处理执行链路如下：

1. `CFDPostAPP` 启动应用并创建主窗口。
2. `GUIFrame` 生成树、渲染区、工具条和属性面板。
3. 用户通过工具条、菜单或树节点触发功能。
4. `GUIDialog` 弹出参数输入界面，或 `OperatorsGUI` 接管界面交互。
5. `OperatorsModel` 根据参数创建或修改 `FITKInterfaceCFDPost` 中的后处理对象。
6. `PostGraphAdaptor` 将后处理对象映射为可渲染对象和交互部件。
7. `FITKRenderWindowVTK` 与 `FITKPlotWindow` 完成三维或二维结果展示。
8. `GUIWidget` 同步属性、信息、色标和探针曲线等显示内容。

这条链路说明仓库整体是围绕“对象体系 + 图形适配 + 操作器 + 界面”协同工作的。

## 依赖关系

### 外部依赖

根据顶层和子模块 CMake，可确认当前仓库主要依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- Qt5::Xml
- Qt5::Network
- Qt5::Svg
- Qt5::OpenGL
- VTK::VTK
- CGNS::CGNS
- Qwt

### 内部依赖主链

- `FITKAppFramework` 依赖 `FITKCore`
- `FITKEasyParam` 依赖 `FITKCore`
- `FITKInterfaceCFDPost` 依赖 `FITKCore`、`FITKAppFramework`、`FITKVTKAlgorithm`
- `PostGraphAdaptor` 依赖 `FITKInterfaceCFDPost`、`FITKRenderWindowVTK`、`FITKVTKAlgorithm`
- `OperatorsModel` 依赖 `GUIDialog`、`GUIFrame`、`GUIWidget`、`OperatorsInterface`、`PostGraphAdaptor`
- `CFDPostAPP` 链接上述关键模块形成最终可执行程序

## 构建方式

### CMake

当前仓库的主构建入口是根目录 `CMakeLists.txt`，其特点包括：

- 使用 C++11
- 统一启用 AUTOMOC、AUTORCC、AUTOUIC
- 统一输出到 `output/bin` 与 `output/bin_d`
- 依赖 `TOOLS_DIR` 提供的 cmake 配置
- 通过 `add_subdirectory(...)` 依次纳入底座、组件、接口、适配层、操作器层和应用入口层

典型构建流程示例：

```bash
mkdir build
cd build
cmake -DTOOLS_DIR=../Tools
cmake --build . --config Debug
```

若工具链路径由环境变量提供，也可以直接依赖环境变量中的 `TOOLS_DIR`。

### 其他工程组织方式

仓库中同时保留：

- `CFDPost.pro`
- 各子模块 `.pro`、`.pri`

当前仓库兼容 qmake 和 Visual Studio 工程组织方式。

## 运行与部署

### 输出目录

- Debug：`output/bin_d`
- Release：`output/bin`

### 运行时特点

- 主程序位于 `CFDPostAPP` 对应的可执行文件。
- 各共享库模块会输出到统一的 `output` 目录下。
- `TOOLS_DIR` 中的第三方配置、VTK、Qwt、CGNS 等运行时资产需要可被正确找到。

仓库根目录还提供了若干辅助脚本，例如：

- `InitSubmodules.bat` / `InitSubmodules.sh`
- `PullSubmodules.bat` / `PullSubmodules.sh`
- `CopyLibs.bat`
- `Create_CFDPost_sln.bat`

这些脚本主要服务于仓库初始化、子模块同步、工程生成和运行库复制。

## 适用场景

该仓库适合以下场景：

- 导入并浏览 CFD 结果文件。
- 对结果执行切片、裁剪、流线、阈值和变形等后处理操作。
- 在三维窗口中查看后处理对象并进行交互选择。
- 使用曲线窗口或探针结果进行辅助分析。
- 在 FITK/FastCAE 体系内继续扩展新的操作器、界面模块或图形适配逻辑。

## 开发建议

- 若修改应用启动或模块装配流程，优先从 `CFDPostAPP` 和 `FITKAppFramework` 入手。
- 若修改后处理对象结构，优先查看 `FITKInterfaceCFDPost`。
- 若修改 VTK 数据处理或图元封装，优先查看 `FITKVTKAlgorithm`。
- 若修改渲染对象映射和交互部件，优先查看 `PostGraphAdaptor`。
- 若修改业务操作，优先查看 `OperatorsModel`。
- 若修改主界面、工具条和属性面板，优先查看 `GUIFrame` 与 `GUIWidget`。
- 若修改参数输入流程，优先查看 `GUIDialog`。

## 注意事项

- 当前仓库依赖外部 `TOOLS_DIR`，未配置时顶层 CMake 会直接失败。
- 部分模块 README 中为了概括结构使用了分组写法，阅读源码时应以真实类名和真实文件名为准。

## 最后更新时间

最后更新时间：2026-04-23