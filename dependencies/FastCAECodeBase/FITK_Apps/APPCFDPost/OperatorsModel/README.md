# OperatorsModel - 后处理业务操作器模块

## 简介

OperatorsModel 是 CFDPost 中承载业务操作器实现的核心模块，负责导入、删除、颜色设置、等值面、裁剪、切片、流线、Glyph、极值、阈值、变形、灯光、探针等后处理操作的执行逻辑。

如果说 OperatorsInterface 定义操作器抽象边界，那么 OperatorsModel 就是这些边界上的主要业务实现层。

## 模块定位

OperatorsModel 位于业务逻辑层，承担以下职责：

- 提供具体后处理功能对应的操作器类。
- 作为组件被应用程序装入运行时。
- 协调 GUIDialog、GUIFrame、GUIWidget、PostGraphAdaptor 与 FITKInterfaceCFDPost 之间的业务调用。

## 架构与目录

```text
OperatorsModel/
├── OpersModelInterface.*       # 模块组件接口
├── OperatorsBase.*             # 操作器基类
├── OperToolbarViews.*          # 工具条视图操作器
├── OperatorsImport.*           # 导入
├── OperatorsDelete.*           # 删除
├── OperatorsColor.*            # 颜色设置
├── OperatorsColorMapEdit.*     # 色标编辑
├── OperatorsIsosurf.*          # 等值面
├── OperatorsClip.*             # 裁剪
├── OperatorsSlice.*            # 切片
├── OperatorsStreamLine.*       # 流线
├── OperatorsGlyph.*            # Glyph
├── OperatorsExtremum.*         # 极值
├── OperatorsThreshold.*        # 阈值
├── OperatorsDeform.*           # 变形
├── OperatorsLight.*            # 灯光
├── OperatorsProbe.*            # 探针
├── OperatorsWorkingDir.*       # 工作目录
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
- FITKWidget
- FITKInterfaceCFDPost
- GUIFrame
- GUIWidget
- GUIDialog
- OperatorsInterface
- PostGraphAdaptor

## 关键类与功能说明

### OpersModelInterface

OpersModelInterface 继承自 AppFrame::FITKComponentInterface，是 OperatorsModel 作为组件被加载时的入口类。

它提供：

- init()
- getComponentName()

这说明该模块并不是简单的头文件集合，而是会以组件方式把一批业务操作器注册到应用程序中。

### OperatorsBase

OperatorsBase 是各类具体操作器的共同基础类，用于承载通用操作器逻辑。

### OperatorsImport

负责导入后处理文件，是从外部结果文件进入后处理对象体系的起点之一。

### OperatorsClip / OperatorsSlice / OperatorsIsosurf / OperatorsStreamLine

这组类覆盖了 CFD 后处理中最常见的派生功能，是该模块最核心的业务能力集合。

### OperatorsColor / OperatorsColorMapEdit / OperatorsLight

这组类围绕显示表现层工作，负责颜色映射、色标编辑和灯光控制。

### OperatorsProbe / OperatorsExtremum / OperatorsThreshold / OperatorsDeform

这组类负责分析与结果变换类功能，覆盖探针、极值查询、阈值筛选和变形显示。

## 构建与集成

### CMake 特征

- 构建为共享库 OperatorsModel
- 链接 VTK::VTK
- 同时依赖 GUIDialog、GUIFrame、GUIWidget、PostGraphAdaptor 和 FITKInterfaceCFDPost

### 集成方式

1. 通过 OpersModelInterface 完成组件级初始化。
2. 响应 GUI 侧事件或对话框确认动作。
3. 操作 FITKInterfaceCFDPost 中的数据对象。
4. 驱动 PostGraphAdaptor 和 GUI 层刷新结果。

## 使用方法

### 典型接入步骤

1. 在应用程序启动时加载 OperatorsModel 组件。
2. 由菜单、工具条、对话框或树事件触发具体操作器。
3. 操作器根据当前对象和参数执行导入、裁剪、切片等业务逻辑。
4. 将结果同步到渲染对象和界面状态。

### 适用场景

- 导入 CFD 结果文件。
- 创建或删除派生后处理对象。
- 修改颜色、灯光、探针和阈值等分析参数。
- 与图形适配层配合刷新场景显示。

## 与其他模块的关系

- GUIDialog：为 OperatorsModel 提供参数输入来源。
- GUIFrame / GUIWidget：为操作器提供界面上下文与展示刷新目标。
- OperatorsInterface：为具体操作器提供继承接口。
- PostGraphAdaptor：负责把业务结果转换为图形对象表现。
- FITKInterfaceCFDPost：提供被操作的后处理数据对象体系。

## 注意事项

- OperatorsModel 是业务逻辑层，README 应强调“执行功能”而不是“展示功能”。
- 由于功能类较多，文档宜按功能分组说明，而不是逐类展开所有槽函数和成员变量。
- 当前模块通过组件接口接入，更新文档时应保留这一运行时加载事实。

## 最后更新时间

最后更新时间：2026-04-23