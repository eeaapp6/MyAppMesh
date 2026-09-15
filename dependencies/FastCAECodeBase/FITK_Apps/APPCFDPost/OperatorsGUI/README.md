# OperatorsGUI - 界面交互操作器适配模块

## 简介

OperatorsGUI 是连接界面事件与后处理操作逻辑的 GUI 适配模块，负责处理树点击、图形拾取、预处理和界面侧操作器接口装配。

它不同于 OperatorsModel：

- OperatorsModel 更偏业务执行。
- OperatorsGUI 更偏界面交互和界面响应适配。

## 模块定位

OperatorsGUI 位于界面层和操作器层之间，主要承担以下职责：

- 把界面动作转换为可复用的 GUI 操作器流程。
- 处理模型树鼠标事件和图形拾取相关交互。
- 为 GUI 侧提供统一的组件初始化入口。

## 架构与目录

```text
OperatorsGUI/
├── OperatorsGUIInterface.*       # 模块组件接口
├── OperatorsGraphPreprocess.*    # 图形预处理相关操作器
├── OperModelTreeMouseEvent.*     # 模型树鼠标事件处理
├── OperatorsWidget.*             # 控件相关操作器
├── OperGraphPick.*               # 图形拾取操作
├── OperGraphPickPreview.*        # 拾取预览操作
├── OperatorsGUIAPI.h
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
- FITKInterfaceCFDPost
- FITKRenderWindowVTK
- FITKWidget
- PostGraphAdaptor
- OperatorsInterface
- GUIFrame
- GUIWidget

## 关键类与功能说明

### OperatorsGUIInterface

OperatorsGUIInterface 继承自 AppFrame::FITKComponentInterface，是该模块作为组件被加载时的入口类。

它提供：

- init()
- getComponentName()

这说明 OperatorsGUI 会以组件方式挂入应用程序，而不是以静态工具类方式直接调用。

### OperModelTreeMouseEvent

该类用于处理模型树鼠标事件，是对象树交互和后处理对象选择逻辑的重要入口。

### OperGraphPick

该类用于图形拾取操作，说明模块直接参与渲染窗口中的选择交互。

### OperGraphPickPreview

该类用于拾取预览，表明模块不只处理最终结果，还处理交互过程中的即时反馈。

### OperatorsGraphPreprocess

该类名称表明该模块中还包含图形预处理相关的交互操作器。

## 构建与集成

### CMake 特征

- 构建为共享库 OperatorsGUI
- 链接 VTK::VTK
- 与 GUIFrame、GUIWidget、PostGraphAdaptor、OperatorsInterface 联动

### 集成方式

1. 应用启动后由组件接口完成初始化。
2. 主窗口树控件、渲染控件等界面事件交由 OperatorsGUI 侧操作器处理。
3. 需要时再调用 OperatorsModel 或 PostGraphAdaptor 继续完成业务和渲染更新。

## 使用方法

### 典型接入步骤

1. 通过 OperatorsGUIInterface 完成模块初始化。
2. 将树点击、拾取、预览等界面事件绑定到对应操作器。
3. 在交互完成后把结果同步到渲染适配层或业务操作器层。

### 适用场景

- 处理对象树鼠标交互。
- 处理渲染窗口中的拾取与预览。
- 将 UI 事件转换为统一操作器调用。

## 与其他模块的关系

- GUIFrame：提供界面事件来源。
- GUIWidget：提供部分交互控件支持。
- OperatorsInterface：提供 GUI 操作器继承基类或公共接口。
- OperatorsModel：在需要真正执行业务时被进一步调用。
- PostGraphAdaptor：承接图形拾取、预览和渲染对象协作。

## 注意事项

- OperatorsGUI 是界面适配层，不应把它写成完整业务逻辑层。
- 该模块通过组件接口加载，文档中应强调其运行时接入方式。
- 当前头文件注释中明确写出“GUI 操作器的接口调用，无实际作用”，说明 OperatorsGUIInterface 主要承担模块入口职责。

## 最后更新时间

最后更新时间：2026-04-23