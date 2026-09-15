# CFDPostAPP - CFDPost 主应用程序

## 简介

CFDPostAPP 是当前仓库的桌面应用入口，负责把 FITK 底座、后处理接口、图形组件和业务操作器装配成可启动的 CFD 后处理程序。

它不是独立算法模块，而是应用层集成模块：

- 负责主程序启动与退出。
- 负责主窗口生成与预初始化流程。
- 负责全局数据、组件和工作台相关对象的装配。
- 负责把 GUIFrame、GUIWidget、OperatorsModel、OperatorsGUI、OperatorsInterface 等模块接入运行时。

## 模块定位

CFDPostAPP 位于应用程序最外层，承担以下职责：

- 提供 main 函数和可执行程序目标。
- 生成主窗口并挂接应用框架生命周期。
- 注册或组装全局数据、组件工厂、命令行处理和工作台逻辑。
- 作为上层壳程序链接各个动态库模块。

从 CMake 看，它最终构建为 WIN32 可执行文件，而不是共享库。

## 架构与目录

```text
CFDPostAPP/
├── main.cpp                     # 程序入口
├── MainWindowGenerator.*        # 主窗口生成器
├── ComponentFactory.*          # 组件工厂接入
├── GlobalDataFactory.*         # 全局数据工厂接入
├── CommandLineHandler.*        # 命令行处理
├── WorkBenchHandler.*          # 工作台相关处理
├── PreWindowInitializer.*      # 主窗口前置初始化
├── CFDPostAPPSettings.*        # 应用设置
├── icon.ico                    # 应用图标
└── CMakeLists.txt
```

## 依赖关系

### 外部依赖

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- VTK::VTK

### 内部依赖

- FITKAppFramework
- FITKCore
- FITKRenderWindowVTK
- FITKWidget
- FITKInterfaceCFDPost
- FITKPlotWindow
- GUIFrame
- GUIWidget
- OperatorsModel
- OperatorsGUI
- FITKEasyParam
- OperatorsInterface

### 输出位置

- Debug 输出到 output/bin_d
- Release 输出到 output/bin

## 关键类与文件

### main.cpp

应用程序启动入口，负责启动 Qt 应用和上层框架流程。README 不展开其细节实现，但它定义了该模块作为可执行程序的角色。

### MainWindowGenerator

MainWindowGenerator 继承自 AppFrame::FITKAbstractMainwindowGenerator，用于生成主窗口实例。

它是应用层把 GUIFrame 主界面接入 FITKAppFramework 的直接入口。

### ComponentFactory

该类负责应用可用组件的装配和注册，是运行时把各个组件模块纳入系统的重要接入点。

### GlobalDataFactory

该类负责全局数据对象的创建与装配，通常与业务运行态的数据仓库初始化相关。

### PreWindowInitializer

该类负责主窗口创建之前的前置初始化，用于把启动早期的依赖准备和环境配置与 UI 创建过程分离。

### WorkBenchHandler

该类负责工作台相关逻辑，是应用层工作区组织和后处理工作流程的重要协调点。

### CommandLineHandler

该类用于处理命令行参数，说明应用程序不仅支持图形界面启动，也保留了启动参数处理通道。

## 构建与集成

### CMake 特征

当前模块的 CMake 具有以下特征：

- 使用 project(CFDPostAPP VERSION 1.0.0 LANGUAGES CXX)
- 通过 add_executable(${PROJECT_NAME} WIN32) 定义为可执行文件
- 启用 AUTOMOC、AUTORCC、AUTOUIC
- 在 Windows 下根据 icon.ico 生成资源文件
- 通过 link_fitk_library 链接大量内部模块

### 集成方式

CFDPostAPP 不供其他模块独立复用，而是作为最终运行外壳：

1. 链接底座与接口模块。
2. 链接图形与界面模块。
3. 链接操作器和参数模块。
4. 在启动时生成主窗口并组织运行时对象。

## 使用方法

### 启动流程

典型启动流程可概括为：

1. 进入 main.cpp。
2. 初始化应用框架与程序设置。
3. 执行 PreWindowInitializer 等前置初始化。
4. 通过 MainWindowGenerator 创建主窗口。
5. 由主窗口继续承载树、渲染、属性和工具条等子界面。

### 适用场景

- 启动 CFD 后处理桌面程序。
- 装配仓库中的各类后处理模块。
- 为 GUIFrame、OperatorsModel、PostGraphAdaptor 提供统一运行入口。

## 与其他模块的关系

- GUIFrame：提供主窗口壳体。
- GUIWidget：提供属性面板、信息面板和工具条等控件。
- GUIDialog：提供功能设置对话框。
- OperatorsModel：提供导入、裁剪、流线等业务操作器。
- OperatorsGUI：处理界面侧交互适配。
- OperatorsInterface：提供操作器公共接口基类。
- PostGraphAdaptor：提供后处理对象与渲染对象之间的映射与交互适配。

## 注意事项

- 该模块依赖 TOOLS_DIR 以加载 VTK 配置，构建前必须保证外部工具链路径有效。
- 该模块主要承担集成与启动职责，不适合在 README 中写成业务算法模块。
- 当前目录同时保留 pro、vcxproj 和 CMake 文件，说明项目需要兼容多种工程组织方式。
- 本说明基于当前源码和 CMake 事实整理，未额外验证命令行参数的具体支持矩阵。

## 最后更新时间

最后更新时间：2026-04-23