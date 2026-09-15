# OperatorsInterface - 操作器公共接口模块

## 简介

OperatorsInterface 是 CFDPost 操作器体系的公共接口模块，负责定义图形事件、图形交互和参数界面相关操作器的抽象接口。

该模块的职责不是直接执行业务，而是为上层操作器提供统一继承基础：

- 统一界面操作器的抽象边界。
- 统一图形刷新、显示切换和交互事件相关接口。
- 降低 OperatorsModel、OperatorsGUI 与底座之间的直接耦合。

## 模块定位

OperatorsInterface 位于操作器体系的接口层。

- 向下依赖 FITKCore 和 FITKAppFramework。
- 向上被 OperatorsModel、OperatorsGUI 等模块复用。
- 作为“操作器契约层”存在，而不是具体功能实现层。

## 架构与目录

```text
OperatorsInterface/
├── GraphEventOperator.*           # 图形事件操作器接口
├── GraphInteractionOperator.*     # 图形交互操作器接口
├── ParaWidgetInterfaceOperator.*  # 参数界面操作器接口
├── OperatorsInterfaceAPI.h
└── CMakeLists.txt
```

## 依赖关系

### 外部依赖

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets

### 内部依赖

- FITKAppFramework
- FITKCore

### 构建属性

- 构建为共享库 OperatorsInterface
- 主要输出抽象接口类，不依赖 VTK

## 关键类与功能说明

### GraphEventOperator

GraphEventOperator 继承自 Core::FITKAbstractOperator，是三维可视化对象数据操作器接口类。

从头文件可确认它定义了以下虚接口：

- updateAll()
- updateSingle(int postDataID)
- updateLight(int dataID)
- updateShow(int postDataID, bool isShow)
- updateAllRotateVis(bool isVis)
- reRender()

这说明它主要约束“对象显示、灯光、重绘和整体刷新”的操作器行为。

### GraphInteractionOperator

该类用于抽象图形交互流程，适合承接鼠标、交互部件和渲染窗口相关的操作器行为。

### ParaWidgetInterfaceOperator

该类用于抽象参数界面相关操作器，是对话框、参数面板和业务操作器之间的重要接口层。

## 构建与集成

### CMake 特征

- 使用 add_library(${PROJECT_NAME} SHARED)
- 仅链接 Qt 与 FITK 底座模块
- 不直接链接 GUIFrame、GUIDialog、PostGraphAdaptor 等更高层模块

### 集成方式

1. 作为基础接口库先被构建。
2. OperatorsGUI 和 OperatorsModel 在其之上实现具体子类。
3. 应用运行时通过组件或操作器仓库使用这些具体实现。

## 使用方法

### 典型接入步骤

1. 选择适合的接口基类作为继承入口。
2. 在子类中重写图形事件、重绘或参数交互接口。
3. 将具体实现注册或装配到上层操作器模块中。

### 适用场景

- 为后处理对象定义统一刷新与重绘接口。
- 为图形交互流程定义统一继承结构。
- 为参数窗口和界面操作逻辑提供抽象基类。

## 与其他模块的关系

- OperatorsModel：提供具体导入、裁剪、流线等操作器实现。
- OperatorsGUI：提供树点击、拾取、预览等界面适配实现。
- GUIFrame 与 GUIDialog：通过具体实现间接使用这些接口。

## 注意事项

- 该模块应被描述为接口层，而不是直接的功能层。
- 因为其价值主要体现在抽象边界，README 应优先说明虚接口职责，而不是仅罗列文件名。
- GraphEventOperator 的接口非常适合文档中作为代表性示例，因为其职责边界清晰且与渲染刷新直接相关。

## 最后更新时间

最后更新时间：2026-04-23