# GUIFrame - GUI框架模块

## 简介

GUIFrame 是 FastCAE 集成工具包（FITK）的 GUI 框架模块，提供主窗口、菜单、工具栏、属性面板等核心界面组件，构建应用程序的整体界面布局。

## 模块定位

GUIFrame 位于项目根目录，是应用程序的界面核心，主要承担以下职责：

- 提供主窗口界面框架（基于 SARibbon）
- 管理菜单栏、工具栏和属性面板
- 集成渲染窗口和树结构视图
- 处理用户交互和事件响应

## 架构与目录

```text
GUIFrame/
├── MainWindow.h/cpp           # 主窗口类
├── MainMenu.h/cpp             # 主菜单
├── MainTreeWidget.h/cpp       # 树结构视图
├── TabWidget.h/cpp            # Tab标签页
├── RenderWidget.h/cpp         # 渲染窗口
├── PropertyWidget.h/cpp       # 属性面板
├── GroupPropertyWidget.h/cpp  # 组属性面板
├── ControlPanelWidget.h/cpp   # 控制面板
├── PanelWidgetBase.h/cpp      # 面板基类
├── ActionEventHandler.h/cpp   # 事件处理器
├── GUIFrameAPI.h              # API声明
└── MainWindow.ui              # UI设计文件
```

## 依赖关系

### 编译依赖

- Qt5::Core
- Qt5::Widgets
- SARibbon（Ribbon界面库）
- FITKCore（核心对象）
- FITKRenderWindowVTK（渲染模块）

## 主要类与功能说明

### `MainWindow`

主窗口类，继承自 `SARibbonMainWindow` 和 `FITKAbstractGUIObject`。是应用程序的主界面容器。

**主要方法：**
- `getActionEventHandle()`：获取事件处理器
- `getTreeWidget()`：获取树结构视图
- `getRenderWidget()`：获取渲染窗口
- `getPropertyWidget()`：获取属性面板
- `getGroupPropertyWidget()`：获取组属性面板
- `createAction()`：创建动作按钮

**信号：**
- `sigMainWindowClose()`：主窗口关闭信号

### `MainTreeWidget`

树结构视图，用于显示项目和数据的层次结构。

### `RenderWidget`

渲染窗口，用于3D图形渲染和可视化。

### `PropertyWidget`

属性面板，用于显示和编辑对象属性。

### `ActionEventHandler`

事件处理器，负责处理用户操作和命令执行。

## 界面布局

主窗口采用经典的三栏布局：

```
┌─────────────────────────────────────────────────────┐
│  Ribbon工具栏                                       │
├──────────┬─────────────────────────────────────────┤
│          │                                         │
│  树结构   │          渲染窗口                        │
│   视图    │                                         │
│          │                                         │
├──────────┼─────────────────────────────────────────┤
│          │              属性面板                     │
└──────────┴─────────────────────────────────────────┘
```

## 使用示例

创建主窗口：

```cpp
#include "MainWindow.h"

// 创建主窗口
GUI::MainWindow* mainWindow = new GUI::MainWindow();

// 获取子部件
GUI::MainTreeWidget* treeWidget = mainWindow->getTreeWidget();
GUI::RenderWidget* renderWidget = mainWindow->getRenderWidget();
GUI::PropertyWidget* propWidget = mainWindow->getPropertyWidget();

// 创建动作
QAction* action = mainWindow->createAction("提示信息", "actionName", "icon.png", "按钮文本");

// 显示窗口
mainWindow->show();
```

## 注意事项

- GUIFrame 依赖 SARibbon 库实现 Ribbon 风格界面
- 主窗口包含多个子部件，通过 getter 方法访问
- 事件处理通过 `ActionEventHandler` 统一管理
- 渲染窗口需要配合 VTK 进行3D显示

最后更新时间：2026-04-30