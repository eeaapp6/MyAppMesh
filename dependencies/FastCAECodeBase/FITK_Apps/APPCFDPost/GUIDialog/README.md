# GUIDialog - 后处理功能对话框模块

## 简介

GUIDialog 是当前仓库中承载后处理参数编辑对话框的界面模块，负责为切片、裁剪、流线、等值面、阈值、变形、探针、色库等功能提供可交互的设置窗口。

该模块本质上是 GUI 层的参数录入和对话框管理模块，而不是业务执行模块：

- 提供各类功能对话框与对应 UI 文件。
- 提供对话框基类与对话框管理器。
- 统一处理“单实例对话框”“对话框生命周期”“参数输入界面”的问题。

## 模块定位

GUIDialog 位于界面层，介于主窗口和业务操作器之间。

- 它负责展示参数输入界面。
- 它依赖 GUIWidget、PostGraphAdaptor 和 OperatorsInterface 等模块完成具体协作。
- 它通常不直接承担结果对象的创建和业务更新，而是把用户输入转交给操作器或渲染适配层。

## 架构与目录

```text
GUIDialog/
├── GUIDialogBase.*              # 对话框基类
├── GUIDialogManager.*           # 对话框管理器
├── FileDialogDirectory.*        # 文件目录选择对话框
├── SliceDialog.*                # 切片对话框
├── ClipDialog.*                 # 裁剪对话框
├── IsosurfDialog.*              # 等值面对话框
├── StreamLineDialog.*           # 流线对话框
├── GlyphDialog.*                # Glyph 对话框
├── ThresholdDialog.*            # 阈值对话框
├── DeformDialog.*               # 变形对话框
├── ProbeDialog.*                # 探针对话框
├── ExtremumDialog.*             # 极值对话框
├── ColorLibaryDialog.*          # 色库对话框
├── ColorLibaryEditDialog.*      # 色库编辑对话框
├── ColorMapperEditDialog.*      # 色标编辑对话框
├── LightWidget.* / LightSubWidget.*
├── ClipSliceWidget*. *          # 各类几何输入子控件
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
- FITKInterfaceCFDPost
- FITKWidget
- FITKEasyParam
- GUIFrame
- GUIWidget
- OperatorsInterface
- PostGraphAdaptor

### 构建属性

- 构建为共享库 GUIDialog
- 启用 Qt 自动 MOC、UIC、RCC
- Debug 输出到 output/bin_d
- Release 输出到 output/bin

## 关键类与功能说明

### GUIDialogBase

GUIDialogBase 是模块内各类功能对话框的共同基类，用于统一对话框行为和扩展点。

### GUIDialogManager

GUIDialogManager 是对话框全生命周期管理器，提供：

- 单例访问 getInstance()
- dialogCreated() 用于对话框创建登记
- dialogDestoried() 用于销毁后清理
- getDialog<T>() 用于按类型获取在生命周期内的对话框

其中 uniqueFlag 机制说明该模块支持“某类对话框只打开一个实例”的约束。

### SliceDialog / ClipDialog / IsosurfDialog / StreamLineDialog

这些类分别承载切片、裁剪、等值面和流线参数设置，是后处理常用功能的直接 UI 入口。

### ClipSliceWidgetPlane / Sphere / Cylinder / Box

这组子控件用于把不同几何类型的输入界面拆分复用，避免在单个对话框中堆积大量几何参数逻辑。

### ColorLibaryDialog / ColorMapperEditDialog

这组对话框用于管理色库与色标配置，是渲染表现层的重要界面入口。

## 构建与集成

### CMake 特征

当前模块在 CMake 中：

- 使用 add_library(${PROJECT_NAME} SHARED)
- 编译并链接大量 .ui 文件
- 通过 link_fitk_library 接入 GUIFrame、GUIWidget、PostGraphAdaptor 等内部模块

### 集成方式

GUIDialog 不独立运行，通常通过以下方式接入：

1. 由主窗口动作或树节点事件触发。
2. 由 OperatorsGUI 或界面事件适配层决定弹出哪类对话框。
3. 由对话框收集参数并驱动后续业务执行。

## 使用方法

### 典型使用流程

1. 用户在主界面点击导入后的某个功能按钮或菜单。
2. 界面层创建对应对话框实例。
3. GUIDialogManager 记录对话框生命周期，并处理唯一性约束。
4. 用户修改参数后将输入转交给操作器或图形适配层。

### 适用场景

- 需要图形化设置后处理参数时。
- 需要统一管理多个功能对话框时。
- 需要复用平面、球、圆柱、盒等几何输入子控件时。

## 与其他模块的关系

- GUIFrame：提供触发这些对话框的主窗口环境。
- GUIWidget：提供部分复用控件与属性界面支持。
- OperatorsInterface：提供界面操作器接口。
- PostGraphAdaptor：提供几何部件和渲染对象协作能力。
- OperatorsModel：通常在参数确认后执行真正业务逻辑。

## 注意事项

- GUIDialog 是界面输入层，不应在 README 中描述为后处理算法实现层。
- 当前模块依赖大量 UI 文件，更新文档时应优先强调对话框分类和管理关系，而不是罗列每个按钮行为。
- GUIDialogManager 中存在 dialogDestoried 命名拼写，文档应以源码现状为准，不擅自“修正”为其他 API 名称。
- 本说明仅根据目录、CMake 和管理器头文件归纳，未展开每个对话框内部参数项细节。

## 最后更新时间

最后更新时间：2026-04-23