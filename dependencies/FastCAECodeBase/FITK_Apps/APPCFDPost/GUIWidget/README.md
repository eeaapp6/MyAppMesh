# GUIWidget - 通用界面控件模块

## 简介

GUIWidget 是 CFDPost 图形界面中的通用控件模块，负责提供属性面板、信息面板、工具条、拾取信息和若干基础控件封装。

它的核心价值在于把主窗口和对话框频繁复用的界面元素模块化：

- 减少主窗口和对话框中的重复 UI 代码。
- 提供围绕后处理对象的属性编辑与状态展示控件。
- 为渲染交互、颜色映射、动画和过滤器提供工具条封装。

## 模块定位

GUIWidget 位于 GUIFrame 和 GUIDialog 之下，是界面层的通用部件库。

- 它不直接承担应用程序壳体职责。
- 它不直接承担业务操作器职责。
- 它通常作为主窗口子控件或对话框内部控件被复用。

## 架构与目录

```text
GUIWidget/
├── GUIWidgetBase.*            # 控件基类
├── TreeWidget.*               # 树控件
├── PropertyWidget.*           # 属性面板
├── InformationWidget.*        # 信息面板
├── ToolBarBase.*              # 工具条基类
├── ToolBarAnimation.*         # 动画工具条
├── ToolBarColorMapper.*       # 色标工具条
├── ToolBarFilter.*            # 过滤器工具条
├── ToolBarEvent.*             # 事件工具条
├── GUISpinBox.* / GUIComboBox.*
├── GUIPickInfo.*              # 拾取信息展示
├── PickedData.* / PickedDataProvider.*
├── ProbeCurveWidget.*         # 探针曲线控件
├── LinearGradientWidget.*     # 线性渐变控件
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
- FITKWidget
- FITKRenderWindowVTK
- FITKPlotWindow
- PostGraphAdaptor

## 关键类与功能说明

### GUIWidgetBase

GUIWidgetBase 是模块内通用控件的基础抽象，供属性界面、信息界面等控件继承。

### PropertyWidget

PropertyWidget 是最核心的属性面板控件之一，提供：

- init()
- updateCurrentGraphObj(int postDataID)
- 不透明度相关界面更新槽函数

这说明它主要围绕当前选中的后处理对象进行属性同步。

### InformationWidget

InformationWidget 用于展示对象或系统相关的辅助信息，是主界面右侧或下方信息区域的候选承载控件。

### ToolBarAnimation / ToolBarColorMapper / ToolBarFilter / ToolBarEvent

这组工具条把不同功能维度拆开：

- 动画
- 色标与颜色映射
- 过滤器功能
- 事件与交互控制

它们通常由 GUIFrame::MainWindow 统一持有和展示。

### GUIPickInfo / PickedData / PickedDataProvider

这组类围绕拾取结果展示与数据提供协作，说明 GUIWidget 与渲染选择、后处理查询之间存在紧密连接。

### ProbeCurveWidget

ProbeCurveWidget 面向探针曲线展示，是后处理结果从数值对象转到图形化曲线控件的界面入口之一。

## 构建与集成

### CMake 特征

- 构建为共享库 GUIWidget
- 同时编译多个 .ui 文件
- 链接 VTK::VTK 以及 PostGraphAdaptor、FITKPlotWindow 等内部模块

### 集成方式

1. GUIFrame 通过 MainWindow 集成多个 GUIWidget 控件。
2. GUIDialog 在需要时复用 GUIWidget 中的子控件。
3. 控件通过 PostGraphAdaptor 或后处理数据接口同步当前选中对象状态。

## 使用方法

### 典型接入步骤

1. 在主窗口或对话框中创建控件实例。
2. 调用 init() 完成界面初始化。
3. 在选中对象变化时调用 updateCurrentGraphObj() 等接口刷新显示。
4. 将槽函数与主界面动作或渲染事件连接起来。

### 适用场景

- 构建属性面板和信息面板。
- 构建动画、色标和过滤器工具条。
- 展示拾取信息和探针曲线结果。

## 与其他模块的关系

- GUIFrame：直接组合使用 GUIWidget 的主要控件。
- GUIDialog：可复用部分输入控件和显示控件。
- PostGraphAdaptor：为属性面板和拾取信息提供渲染对象上下文。
- FITKInterfaceCFDPost：为属性和信息面板提供后处理数据对象事实来源。

## 注意事项

- GUIWidget 是界面控件库，不宜在 README 中描述为业务执行层。
- 由于该模块同时依赖渲染窗口与图形适配层，文档应强调它是“界面展示和编辑”模块，而不是“纯 Qt 小控件集合”。
- 本说明只基于当前控件命名、CMake 和代表性头文件整理，未逐个展开每个控件的内部槽函数逻辑。

## 最后更新时间

最后更新时间：2026-04-23