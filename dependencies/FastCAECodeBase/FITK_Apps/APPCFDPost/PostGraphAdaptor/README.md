# PostGraphAdaptor - 后处理图形适配模块

## 简介

PostGraphAdaptor 是 CFDPost 中把后处理数据对象转换为渲染对象和交互部件的图形适配层模块。

它在系统中的作用不是生成后处理数据，而是负责：

- 管理后处理数据对象与渲染对象之间的映射关系。
- 提供图形对象管理器和灯光管理器。
- 封装平面、直线、球、圆柱、盒等交互部件及其回调。
- 为 GUI 层和操作器层提供面向渲染的统一适配入口。

## 模块定位

PostGraphAdaptor 位于后处理数据层与渲染表现层之间。

- 向下依赖 FITKInterfaceCFDPost、FITKVTKAlgorithm、FITKRenderWindowVTK。
- 向上被 GUIWidget、GUIDialog、OperatorsGUI、OperatorsModel 使用。
- 负责把“后处理对象”变成“可显示、可交互的图形对象”。

## 架构与目录

```text
PostGraphAdaptor/
├── PostGraphObjectAgent.*          # 渲染对象代理
├── PostGraphObjectBase.*           # 渲染对象基类
├── PostGraphObjectImport.*         # 导入对象图形适配
├── PostGraphObjectLight.*          # 灯光图形对象
├── PostGraphObjectPick.*           # 拾取图形对象
├── PostGraphObjectManager.*        # 渲染对象管理器
├── PostGraphProperty.*             # 图形属性封装
├── PostGraphWidgetPlane.*          # 平面交互部件
├── PostGraphWidgetLine.*           # 线交互部件
├── PostGraphWidgetSphere.*         # 球交互部件
├── PostGraphWidgetCylinder.*       # 圆柱交互部件
├── PostGraphWidgetBox.*            # 盒交互部件
├── 对应 *CallBack.*               # 各类交互回调
├── FITKVKTBoxRepresentaion.*       # Box 表示层扩展
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
- FITKVTKAlgorithm

## 关键类与功能说明

### PostGraphObjectManager

PostGraphObjectManager 是模块核心管理器，继承自 Core::FITKAbstractDataManager<PostGraphObjectAgent>，并通过 DeclSingleton 声明为单例。

它提供：

- getGraphObjectBase(FITKAbstractCFDPostData* postData)
- getGraphObjectBase(int PostDataID)
- getGraphLightManager()

这说明它是“从后处理数据 ID 获取对应渲染对象”的统一入口。

### PostGraphLightManager

该管理器专门负责渲染灯光对象，说明图形适配层不仅管理几何结果，也管理场景灯光表现。

### PostGraphObjectBase

该类是各类图形对象的共同基类，负责承载面向渲染对象的通用行为和属性。

### PostGraphObjectImport / PostGraphObjectPick / PostGraphObjectLight

这些类分别面向导入对象、拾取对象和灯光对象的图形表示，体现出图形适配层按结果类型拆分实现的方式。

### PostGraphWidgetPlane / Line / Sphere / Cylinder / Box

这组类封装 VTK 交互部件及其回调，是裁剪、切片、流线布线等功能与图形交互的重要桥梁。

## 构建与集成

### CMake 特征

- 构建为共享库 PostGraphAdaptor
- 链接 VTK::VTK
- 链接 FITKInterfaceCFDPost、FITKRenderWindowVTK、FITKVTKAlgorithm

### 集成方式

1. OperatorsModel 创建或更新后处理对象。
2. PostGraphAdaptor 根据后处理对象生成对应图形对象。
3. GUIFrame 和 GUIWidget 通过渲染窗口展示这些对象。
4. GUIDialog 和 OperatorsGUI 通过交互部件修改图形状态或输入几何参数。

## 使用方法

### 典型接入步骤

1. 获取或创建后处理对象。
2. 通过 PostGraphObjectManager 查找对应图形对象。
3. 根据需要启用平面、球、圆柱、盒等交互部件。
4. 将交互结果回写到业务操作器或后处理对象。

### 适用场景

- 将后处理数据映射为渲染对象。
- 为裁剪、切片、流线等功能提供交互部件。
- 管理灯光和拾取相关的图形表现。

## 与其他模块的关系

- FITKInterfaceCFDPost：提供后处理数据对象输入。
- FITKVTKAlgorithm：提供底层 VTK 算法与图形辅助能力。
- GUIWidget：通过属性面板、拾取信息等控件消费图形对象状态。
- GUIDialog：通过交互部件和参数窗口协作完成图形输入。
- OperatorsModel：负责创建业务对象并触发图形更新。

## 注意事项

- PostGraphAdaptor 是适配层，不直接定义业务结果对象本身。
- 由于该模块同时包含图形对象、交互部件和回调，README 应强调“映射和交互”而非单一渲染工具库。
- 当前头文件中存在 FITKVKTBoxRepresentaion 这类命名，文档应保持源码原名，不擅自更正拼写。

## 最后更新时间

最后更新时间：2026-04-23