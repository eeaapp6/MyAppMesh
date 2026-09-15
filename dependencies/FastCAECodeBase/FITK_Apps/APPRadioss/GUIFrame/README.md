# GUIFrame - 主窗口界面框架模块

## 简介

GUIFrame 负责构建和管理应用程序的主窗口界面，基于 SARibbonMainWindow，集成菜单栏、工具栏、中心窗口、消息输出面板等 UI 组件。

## 模块定位

- 构建基于 Ribbon 风格的主窗口界面
- 管理主窗口中的菜单、工具栏和快捷操作
- 组织中心窗口区域，承载图形显示和面板布局
- 提供消息输出和信息显示面板
- 处理界面动作事件，分发到对应的操作器

## 架构与目录

```text
GUIFrame
├── MainWindow.h / .cpp / .ui         主窗口类
├── CentralWidget.h / .cpp / .ui     中心窗口组件
├── ActionEventHandler.h / .cpp       动作事件处理器
├── SARibbonBarHelper.h / .cpp       Ribbon工具栏辅助类
└── MessageTableWidget.h / .cpp        消息输出标签窗口
```

## 依赖关系

- Qt5::Core、Qt5::Gui、Qt5::Widgets
- FITKCore、SARibbon

## 主要类与功能说明

### MainWindow

主窗口类，继承自 SARibbonMainWindow 和 FITKAbstractGUIObject：

```cpp
MainWindow::getInstanceFromFramework();  // 从框架获取主窗口
CentralWidget* cw = mainWindow->getCentralWidget();
mainWindow->setProgressValue(50);
ControlPanel* cp = mainWindow->getControlPanel();
```

### CentralWidget

中心窗口组件，作为主窗口的核心区域：

```cpp
graphArea = centralWidget->getGraphMidArea();
centralWidget->setCurGraphWidgetToPre();   // 设置为前处理窗口
centralWidget->setCurGraphWidgetToPost();  // 设置为后处理窗口
```

### ActionEventHandler

界面动作事件处理器，负责分发界面触发的事件到操作器。

### SARibbonBarHelper

Ribbon 工具栏辅助类，负责创建和管理 Ribbon 界面元素。

## 注意事项

- MainWindow 继承自 SARibbonMainWindow，需确保 SARibbon 库正确集成
- 主窗口实例通过 FITKAppFramework 管理，不应直接 delete
- CentralWidget 中的图形窗口管理由 GUIWidget 模块负责

最后更新时间：2026-04-28