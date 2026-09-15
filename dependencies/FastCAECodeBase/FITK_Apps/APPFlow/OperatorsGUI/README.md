# OperatorsGUI - 操作器GUI模块

## 简介

OperatorsGUI 是 FastCAE 集成工具包（FITK）的操作器 GUI 模块，提供操作器相关的界面组件和交互功能，包括工具栏视图、3D交互事件、拾取预览等。

## 模块定位

OperatorsGUI 位于项目根目录，是操作器模块的界面层，主要承担以下职责：

- 提供操作器工具栏视图
- 处理3D场景交互事件
- 实现拾取预览和选择功能
- 管理模型树的鼠标事件

## 架构与目录

```text
OperatorsGUI/
├── OperatorsGUIAPI.h           # API声明
├── OpersGUIInterface.h         # GUI接口
├── OperToolbarViews.h/cpp      # 工具栏视图
├── OperGraphEvent3D.h/cpp      # 3D交互事件
├── OperGraphPick.h/cpp         # 拾取操作
├── OperGraphPickPreview.h/cpp  # 拾取预览
├── OperGraphPreprocess.h/cpp   # 预处理操作
└── OperModelTreeMouseEvent.h   # 模型树鼠标事件
```

## 依赖关系

### 编译依赖

- Qt5::Core
- Qt5::Widgets
- FITKCore（核心对象）
- VTK（3D渲染）

## 主要类与功能说明

### `OperToolbarViews`

工具栏视图类，管理操作器相关的工具栏组件。

### `OperGraphEvent3D`

3D交互事件类，处理3D场景中的用户交互。

**主要功能：**
- 处理鼠标点击、拖拽事件
- 管理视图旋转、缩放、平移
- 支持选择和拾取操作

### `OperGraphPick`

拾取操作类，负责对象的选择和拾取。

### `OperGraphPickPreview`

拾取预览类，提供拾取对象的实时预览功能。

### `OperModelTreeMouseEvent`

模型树鼠标事件类，处理模型树中的鼠标交互。

## 使用示例

处理3D交互事件：

```cpp
#include "OperGraphEvent3D.h"

// 创建交互事件处理器
Operators::OperGraphEvent3D* eventHandler = 
    new Operators::OperGraphEvent3D(renderWidget);

// 设置拾取模式
eventHandler->setPickMode(Operators::PickMode::Face);

// 连接拾取信号
connect(eventHandler, &Operators::OperGraphEvent3D::sigPicked,
        this, &MyClass::onPicked);
```

## 注意事项

- OperatorsGUI 依赖 VTK 进行3D渲染和交互
- 拾取功能需要配合渲染窗口使用
- 工具栏视图需要与主窗口集成
- 事件处理通过信号槽机制实现

最后更新时间：2026-04-30