# APPCADOCC

## 简介

APPCADOCC 是一个基于 FastCAE / FITK 底座构建的 OpenCASCADE 桌面 CAD 应用工程。

从当前仓库结构和根 CMake 配置看，它不是单个可执行程序源码目录，而是一个完整应用程序工作区。

仓库中同时包含：

- FITK_Kernel 底座子模块
- FITK_Interface 接口与数据结构子模块
- FITK_Component 组件子模块
- 面向当前应用的 GUI、操作器、图形、拾取、Python 包装和主程序装配模块

最终生成的主程序是 CADOCCApp。

该应用解决的核心问题不是“如何实现某一个几何命令”，而是“如何把 FITK 底座、OCC 几何能力、三维显示、参数对话框、操作器体系和脚本接口组织成可运行的 CAD 应用”。

## 仓库定位

- 仓库名称：APPCADOCC
- 根 CMake 工程名：CADOCCApp
- 最终产物类型：桌面可执行程序 + 多个应用内共享库
- 主要技术栈：C++、Qt5、OpenCASCADE、HDF5、VTK、PythonQt、FITK
- 主要目标平台：Windows，兼顾 Linux

当前根目录不是单一模块，而是应用级聚合工程。

它通过顶层 CMake 把底座、接口、组件和本应用模块统一纳入同一套构建输出。

## 整体架构

按照当前代码组织，可以把整个应用理解为四层：

1. FITK_Kernel：提供应用框架、核心仓库、数据仓库、Python 支撑、参数系统等基础能力。
2. FITK_Interface：提供几何、模型、IO、物理等抽象接口与数据结构。
3. FITK_Component：提供 OCC 几何组件、渲染窗口、消息窗口、图形适配等可复用组件实现。
4. 应用层模块：围绕当前 CAD 应用补充主窗口、参数对话框、操作器、拾取数据、图形对象管理、Python 包装和最终程序入口。

从顶层 CMakeLists.txt 可以直接确认，当前应用层模块包括：

- GUIWidget
- GUIFrame
- PickDataProvider
- GUIDialog
- GraphDataProvider
- OperatorsInterface
- OperatorsGUI
- OperatorsModel
- PythonInterface
- CADOCCApp

这些模块共同组成当前 OCC CAD 应用的完整运行链路。

## 根目录结构

根目录下最重要的目录可按职责理解为：

```text
APPCADOCC
├── FITK_Kernel          # 应用底座、核心框架、Python、参数系统
├── FITK_Interface       # 通用接口与数据结构
├── FITK_Component       # OCC 几何、渲染窗口、图形适配等组件
├── GUIWidget            # 通用界面部件与对话框基类
├── GUIFrame             # 主窗口骨架、动作分发、渲染区与模型树
├── PickDataProvider     # OCC 拾取结果管理
├── GUIDialog            # 业务参数对话框集合
├── GraphDataProvider    # 数据对象到 OCC 图形对象的管理层
├── OperatorsInterface   # 操作器抽象接口层
├── OperatorsGUI         # 图形与界面交互操作器实现
├── OperatorsModel       # 建模、文件、导入导出等业务操作器实现
├── PythonInterface      # PythonQt 包装层
├── CADOCCApp            # 应用入口与最终装配模块
├── output               # 统一输出目录
├── Tools                # 第三方依赖与工具链
└── generate             # 生成文件与中间产物
```

这个目录划分说明当前仓库采用“底座复用 + 应用集成”的组织方式，而不是把全部逻辑直接塞进主程序目录。

## 启动与运行链路

根据根 CMake 配置和 CADOCCApp 下的 main.cpp、ComponentFactory、GlobalDataFactory、PythonRegister、WorkBenchHandler，可以把应用启动过程概括为：

1. main.cpp 创建 ApplicationCAD。
2. 注册 MainWindowGenerator，用于生成主窗口。
3. 注册 GlobalDataFactory，用于创建全局主数据。
4. 注册 ComponentFactory，用于创建消息、渲染、OCC 几何、模型操作器和 GUI 操作器组件。
5. 注册 CommandLineHandler、SignalProcessor、WorkBenchHandler、CADOCCSettings。
6. 注册 PyRegister，把 PythonInterface 中的包装类接入脚本环境。
7. 进入 app.exec()，启动桌面应用主循环。

这意味着应用不是通过单一控制器类启动，而是依赖 FITKApplication 的注册式装配流程完成初始化。

## 应用层模块分工

### 1. GUIWidget

GUIWidget 提供菜单描述类、界面基类、属性页基类和 GUIDialogBase。

它位于应用 GUI 复用层，负责沉淀当前应用自己的基础界面能力。

### 2. GUIFrame

GUIFrame 提供 MainWindow、RenderWidget、TreeWidget、MessageTabWidget 和 ActionEventHandler。

它负责主窗口骨架、界面承载和动作分发。

### 3. GUIDialog

GUIDialog 汇总大量建模参数窗口，例如基础几何、布尔、修复、变换、组合等对话框。

它是具体业务参数界面的集合，而不是新的框架层。

### 4. OperatorsInterface

OperatorsInterface 定义图形事件、图形工具、拾取、参数窗口和模型树相关操作器接口。

它负责建立统一的操作器语义边界。

### 5. OperatorsGUI

OperatorsGUI 提供 GUI 侧的具体交互实现，包括视角切换、拾取、测量、树事件等。

它把主窗口动作与三维视图交互连接起来。

### 6. OperatorsModel

OperatorsModel 提供建模、导入导出、工程读写、撤销重做、参数驱动型几何命令执行等业务操作器。

它是当前应用动作体系中最集中的业务实现层。

### 7. PickDataProvider

PickDataProvider 管理 OCC 视图中的拾取结果。

它为参数窗口、图形工具和操作器提供拾取数据中转。

### 8. GraphDataProvider

GraphDataProvider 负责把数据对象映射为 OCC 图形对象，并维护模型 provider 与 marker provider。

它位于数据层和三维显示层之间。

### 9. PythonInterface

PythonInterface 提供 Point、Solid、Box、Chamfer、AppendCompound、RemoveCompound 等 PythonQt 包装对象。

它不是解释器本身，而是当前应用业务能力的脚本暴露层。

### 10. CADOCCApp

CADOCCApp 是最终程序入口与装配层。

它负责把上述模块与 FITK 子模块组装成可运行程序。

## 主要外部依赖

根据根 CMakeLists.txt 和应用模块配置，当前工程主要依赖以下外部库与工具：

- Qt5 Core / Gui / Widgets / Network / Xml / Svg / OpenGL / Designer
- OpenCASCADE
- HDF5
- VTK
- qwt
- SARibbon
- Python 3.7
- PythonQt

其中，Tools 目录用于承载当前仓库依赖的第三方配置与库文件。

顶层 CMake 默认会优先读取 CMake 参数中的 TOOLS_DIR，其次读取环境变量，最后回落到仓库内的 Tools 目录。

## 构建方式

当前仓库主要通过 CMake 构建。

根 CMake 的关键事实包括：

- 工程名为 CADOCCApp
- C++ 标准为 C++11
- 启用了 AUTOMOC、AUTOUIC、AUTORCC
- 输出目录统一设置为 output
- Debug 输出到 output/bin_d
- Release 输出到 output/bin

典型构建流程可概括为：

1. 准备 Qt、OCC、HDF5、VTK、PythonQt 等依赖。
2. 确认 TOOLS_DIR 可解析到当前仓库 Tools 或外部工具目录。
3. 在独立 build 目录执行 CMake 配置。
4. 生成并编译全部子目录目标。

如果只看仓库事实，当前工作区还保留了 qmake 工程和 Visual Studio 工程文件。

因此，该仓库不是“只支持 CMake”的单一路径工程，但当前根目录总体构建入口是 CMakeLists.txt。

## 输出与部署

构建完成后，主要输出位于：

- output/bin_d：Debug 版本
- output/bin：Release 版本

由于当前应用依赖 Qt、OCC、HDF5、PythonQt、脚本运行时及多个内部共享库，运行时部署不能只拷贝主程序本体。

仓库根目录提供了 CopyLibs.bat 和 CopyLibs.sh，用于辅助处理库复制与运行时依赖整理。

## 典型使用场景

从当前模块职责可以看出，应用主要面向以下场景：

- 几何建模与编辑
- OCC 三维显示与交互
- 模型树与参数窗口协同编辑
- 工程文件读写与 brep 输出
- Python 脚本驱动部分几何操作

README 不直接宣称全部高级功能都已完成，而是以当前仓库可确认的模块职责为准。

## 使用示例

### 场景 1：应用启动装配

下面的片段对应当前主程序真实启动方式：

```cpp
ApplicationCAD app(argc, argv);

app.regMainWindowGenerator(new MainWindowGenerator);
app.regGlobalDataFactory(new GlobalDataFactory);
app.regComponentsFactory(new ComponentFactory);
app.regCommandLineHandler(new CommandLineHandler);
app.addGolbalSignalProcesser(new SignalProcessor);
app.regWorkBenchHandler(new OCCAppWorkBenchHandler);
app.regAppSettings(new CADOCCSettings);
app.regPythonRegister(new PyRegister);

return app.exec();
```

### 场景 2：工作台模式调用操作器

当前 WorkBenchHandler.cpp 中，工程与几何文件处理通过操作器键路由：

```cpp
Core::FITKActionOperator* openOper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionOpenProject");
Core::FITKActionOperator* saveOper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionSaveProject");
Core::FITKActionOperator* exportOper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionExport");
```

这说明应用的批处理与桌面交互并不是两套完全独立的执行链，而是共享操作器体系。

## 注意事项

- 当前仓库事实能够确认主程序名为 CADOCCApp；如果外部文档使用 OCCCADApp 或类似命名，应以当前源码和构建配置为准。
- 全局数据工厂当前重点创建几何主数据，README 不应夸大为完整 CAE 五段数据全部接入。
- Python 接口注册方法名沿用现有实现中的 registWapper 拼写，不应在文档中擅自更名。
- Linux 下 main.cpp 会显式设置 Python 相关环境变量，部署时需要保证相对路径结构成立。
- 当前在 Windows 环境下核对了目录、构建和接口事实，Linux 运行行为仍应以实际构建和部署结果为准。

最后更新时间：2026-04-23