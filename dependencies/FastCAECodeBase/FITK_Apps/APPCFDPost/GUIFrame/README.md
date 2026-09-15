# GUIFrame - 主界面框架模块

## 简介

GUIFrame 是 CFDPost 图形界面的主框架模块，负责定义主窗口、树视图、渲染窗口和动作事件处理骨架。

它的职责不是提供具体业务功能，而是提供主界面壳体：

- 组织主窗口布局。
- 承载树、渲染、属性、信息和工具条等核心界面区域。
- 为其他 GUI 模块和操作器模块提供统一的主界面入口。

## 模块定位

GUIFrame 位于界面层的中心位置，是 CFDPostAPP 创建出来的主窗口模块。

- 向上由应用程序入口模块创建。
- 向下依赖 GUIWidget 提供的具体控件。
- 通过 RenderWidget 与渲染窗口相关组件协作。

## 架构与目录

```text
GUIFrame/
├── MainWindow.*            # 主窗口
├── MainWindow.ui           # 主窗口界面文件
├── MainTreeWidget.*        # 主树界面
├── RenderWidget.*          # 渲染区域封装
├── ActionEventHandler.*    # action 点击事件处理
├── GUIFrameAPI.h           # 导出宏
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
- FITKRenderWindowVTK
- FITKWidget
- GUIWidget

### 资源依赖

- Resource/postResource.qrc

## 关键类与功能说明

### MainWindow

MainWindow 继承自 QMainWindow 和 Core::FITKAbstractGUIObject，是整个程序主界面的核心承载类。

从头文件可确认它直接管理：

- MainTreeWidget
- RenderWidget
- PropertyWidget
- InformationWidget
- ToolBarColorMapper
- ToolBarAnimation
- ToolBarFilter
- ToolBarEvent
- ActionEventHandler

这说明 GUIFrame 负责把多个 GUIWidget 子模块组织成完整的主界面。

### MainWindow 提供的核心接口

MainWindow 暴露了一组用于跨模块协作的访问接口：

- init()
- createAction()
- findAction()
- getRenderWidget()
- getTreeWidget()
- getPropertyWidget()
- getToolBarColorMapper()
- getToolBarAnimation()
- getToolFilter()
- getToolEvent()
- getInfoWidget()
- getEventWidget()

这些接口表明其他模块通常通过主窗口实例获取具体界面部件，而不是直接管理子控件。

### MainTreeWidget

MainTreeWidget 承担对象树或结构树展示职责，是数据对象、结果对象和交互入口之间的重要连接点。

### RenderWidget

RenderWidget 承担渲染区域封装职责，是主界面连接渲染窗口组件的重要桥梁。

### ActionEventHandler

ActionEventHandler 负责 action 点击处理，说明 GUIFrame 不仅展示界面，也集中管理主窗口级别的动作分发。

## 构建与集成

### CMake 特征

- 使用 add_library(${PROJECT_NAME} SHARED)
- 编译 MainWindow.ui 和资源文件
- 链接 FITKRenderWindowVTK、GUIWidget 等内部模块

### 集成方式

1. CFDPostAPP 通过 MainWindowGenerator 创建 GUIFrame::MainWindow。
2. MainWindow 初始化树、渲染、属性和工具条。
3. 其他模块通过 MainWindow 提供的 getter 接口访问界面部件。

## 使用方法

### 典型接入步骤

1. 在应用启动阶段创建主窗口。
2. 调用 init() 初始化 central widget 和 actions。
3. 通过主窗口暴露的接口获取各个子界面。
4. 将操作器、对话框和渲染逻辑挂接到这些子界面上。

### 适用场景

- 需要完整主界面壳体时。
- 需要树、属性面板、工具条和渲染视图协同工作时。
- 需要统一处理主窗口级别动作和关闭事件时。

## 与其他模块的关系

- CFDPostAPP：创建和持有主窗口。
- GUIWidget：为主窗口提供属性、信息、工具条等子控件。
- GUIDialog：通常由主窗口动作触发弹出。
- OperatorsGUI：可接管树点击、拾取预览等界面交互。

## 注意事项

- GUIFrame 是主界面框架模块，不负责实现裁剪、流线等业务逻辑。
- 文档中应把 MainWindow 视作外部访问入口，把其他类视作其内部支撑部件。
- 当前模块通过资源文件加载图标和界面资源，运行时应保证资源路径随工程正确打包。

## 最后更新时间

最后更新时间：2026-04-23