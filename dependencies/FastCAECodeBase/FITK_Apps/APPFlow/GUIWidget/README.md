# GUIWidget - GUI基础部件模块

## 简介

GUIWidget 是 FastCAE 集成工具包（FITK）的 GUI 基础部件模块，提供可复用的界面组件，包括对话框、树控件、属性编辑器等通用 UI 元素。

## 模块定位

GUIWidget 位于项目根目录，提供通用的 GUI 组件，主要承担以下职责：

- 提供基础对话框和窗口组件
- 实现可复用的树控件和属性编辑控件
- 封装拾取信息和交互数据处理
- 作为 GUIFrame 的底层支撑模块

## 架构与目录

```text
GUIWidget/
├── GUIDialogBase.h/cpp         # 对话框基类
├── GUIWidgetBase.h/cpp         # 部件基类
├── GUIWidgetAPI.h              # API声明
├── MainMenuBase.h/cpp          # 菜单基类
├── TreeWidget.h/cpp            # 树控件
├── CompTreeItem.h/cpp          # 树项组件
├── CompLineEdit.h/cpp          # 行编辑器组件
├── PropertyEmptyWidget.h/cpp   # 空属性面板
├── PropertyChildWidgetBase.h   # 属性子面板基类
├── PickedData.h/cpp            # 拾取数据
├── PickedDataProvider.h/cpp    # 拾取数据提供者
├── PickedDataCalculator.h      # 拾取数据计算器
├── GUIPickInfo.h/cpp           # 拾取信息
└── WidgetOCCEvent.h/cpp        # OCC事件处理
```

## 依赖关系

### 编译依赖

- Qt5::Core
- Qt5::Widgets
- FITKCore（核心对象）

## 主要类与功能说明

### `GUIDialogBase`

对话框基类，提供通用对话框功能。

### `GUIWidgetBase`

部件基类，所有 GUI 部件的基类。

### `TreeWidget`

树控件，提供树形数据展示和交互。

### `CompTreeItem`

树项组件，用于构建树形结构的节点。

### `PickedData`

拾取数据类，存储用户拾取的几何或网格数据。

**主要方法：**
- 获取拾取的点、边、面、体信息
- 支持多种选择模式

### `PickedDataProvider`

拾取数据提供者，管理拾取数据的获取和分发。

### `GUIPickInfo`

拾取信息类，封装拾取操作的详细信息。

### `WidgetOCCEvent`

OCC 事件处理类，处理 OpenCASCADE 相关的交互事件。

## 使用示例

### 1. 使用拾取数据

```cpp
#include "PickedData.h"
#include "PickedDataProvider.h"

// 获取拾取数据提供者
GUI::PickedDataProvider* provider = GUI::PickedDataProvider::instance();

// 获取当前拾取数据
GUI::PickedData* pickedData = provider->getPickedData();

// 获取拾取的点
QList<Core::FITKPoint*> points = pickedData->getPoints();

// 获取拾取的面
QList<Core::FITKFace*> faces = pickedData->getFaces();
```

### 2. 创建树控件

```cpp
#include "TreeWidget.h"
#include "CompTreeItem.h"

// 创建树控件
GUI::TreeWidget* treeWidget = new GUI::TreeWidget(parent);

// 创建树项
GUI::CompTreeItem* rootItem = new GUI::CompTreeItem(treeWidget);
rootItem->setText(0, "根节点");

// 添加子项
GUI::CompTreeItem* childItem = new GUI::CompTreeItem(rootItem);
childItem->setText(0, "子节点");
```

## 注意事项

- GUIWidget 提供通用组件，不包含业务逻辑
- 拾取数据需要配合渲染模块使用
- OCC 事件处理依赖 OpenCASCADE 库
- 所有组件设计为可复用的独立单元

最后更新时间：2026-04-30