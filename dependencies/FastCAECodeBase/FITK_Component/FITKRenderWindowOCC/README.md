# FITKRenderWindowOCC

## 简介

FITKRenderWindowOCC 是 FITK 组件层中的 OCC 三维渲染窗口模块，对外通过 Render::FITKGraph3DWindowOCCInterface 暴露组件接口，对内由 Render::FITKGraph3DWindowOCC 维护 OCC Viewer、View、InteractiveContext 以及与 Qt 窗口事件之间的桥接关系。

如果说 FITKOCCGraphAdaptor 负责“把数据对象变成图形对象”，那么 FITKRenderWindowOCC 负责“把图形对象放到可交互的 OCC 三维窗口中”。它既是渲染窗口本体，也是应用层接入 OCC 三维显示能力的统一组件入口。

## 模块定位

FITKRenderWindowOCC 位于 FITK_Component 下，主要承担以下职责：

- 为应用层提供可注册、可获取的三维窗口组件。
- 管理 OCC 视图对象、交互上下文和窗口刷新过程。
- 统一三维对象的添加、移除、清空、重绘与视角控制。
- 处理选择、框选、Shift 选择、草绘拾点等交互行为。
- 为截图、背景色、投影方式和标准视角切换提供稳定接口。

该模块不负责业务数据到图形对象的转换，不承担应用菜单、命令调度、几何建模和工程读写逻辑。

## 架构与目录

结合当前目录与 CMakeLists.txt，模块内部可以概括为以下几组代码：

1. 组件接口层

- FITKGraph3DWindowOCCInterface.h / .cpp

2. 主窗口与内部背景窗口

- FITKGraph3DWindowOCC.h / .cpp

3. OCC 视图与交互控制封装

- FITKV3d_View.h / .cpp
- FITKV3d_Viewer.h / .cpp
- FITKAIS_ViewController.h / .cpp
- FITKV3d_RectangularGrid.h / .cpp

4. 渲染对象与草绘辅助

- FITKGraphObjectOCC.h / .cpp
- FITKAIS_SketchShape.h / .cpp
- WidgetAddSketchPos.h / .cpp / .ui

5. 资源与翻译文件

- res_occ_window.qrc
- Resource/translations/Translate_FITKRenderWindowOCC_EN.ts
- Resource/translations/Translate_FITKRenderWindowOCC_CN.ts

在应用层，该模块通常通过 ComponentFactory 注册到全局组件容器。当前 CADOCCApp/ComponentFactory.cpp 已经通过 new Render::FITKGraph3DWindowOCCInterface 的方式接入该组件。典型调用链是：组件工厂创建接口实例，组件容器保存名为 Graph3DWindowOCC 的组件，界面层通过 getComponentByName 获取组件，再通过 getWidget(port) 拿到实际 QWidget 并嵌入主界面。

## 依赖关系

### 外部依赖

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- OCC::OCC

### FITK 内部依赖

- FITKCore
- FITKAppFramework

从依赖关系可以看出，该模块建立在 FITKCore 的抽象图形窗口体系之上，通过 FITKAppFramework 接入组件系统，本身不依赖具体业务模块，因此可以作为多个应用共享的 OCC 窗口组件。

## 主要类与功能说明

### Render::FITKGraph3DWindowOCCInterface

这是对外暴露的组件接口，继承自 AppFrame::FITKComponentInterface。当前头文件与实现可以确认两个关键事实：

- getWidget(const int port) 会创建并返回新的 FITKGraph3DWindowOCC 实例。
- getComponentName() 返回固定字符串 Graph3DWindowOCC。

应用层通常通过组件容器按名字获取该组件，再将返回的 QWidget 嵌入布局。

### Render::FITKGraph3DWindowOCC

这是 OCC 三维显示主体，继承自 Core::FITKAbstractGraph3DWidget。当前头文件可直接确认的一组核心接口包括：

- enableSketchMode(bool, double*, double*, double*, bool)
- enableGridLines(bool, bool)
- enableSketch(bool, CommonOCC::SketchShapeType, bool)
- getSketchPoint()
- getSketchPln(...)
- addSketchPosition(double, double)
- getSketchStatus()
- setParallelProjection(bool)
- setBackgroundColor(float*, float*)
- getBackgroundColor(float*, float*)
- setView(Core::FITKGraph3DStandardView)
- setView(Core::FITKGraph3DStandardView, bool)
- addObject(FITKGraphObjectOCC*, bool fitView = true)
- removeGraphObj(Core::FITKAbstractGraphObject*)
- enableSelect(bool)
- enableShiftSelect(bool)
- enableAreaSelect(bool)
- getSelectedShapes(QVector<TopoDS_Shape>&)
- getFirstSelectedShape(TopoDS_Shape&)
- clearSelection()
- hasSelection()
- removeSelectedShape(const TopoDS_Shape&)
- fitView()
- fitSelected()
- reRender()
- clear()
- saveImage(const QString&, int, int)

这些接口已经覆盖了一个三维 CAD 视窗在应用层常用的大部分能力。

### Render::FITKGraph3DWindowOCCInternal

这是内部背景窗口，负责承接 Qt 事件并转发给 OCC 视图控制流程。当前头文件中可以看到它重写了 paintEvent、resizeEvent、mousePressEvent、mouseDoubleClickEvent、mouseReleaseEvent、mouseMoveEvent、wheelEvent 和 focusOutEvent，这说明窗口交互不是简单的 QWidget 包装，而是经过专门的事件桥接。

### FITKAIS_ViewController 与 Render::FITKGraphObjectOCC

FITKAIS_ViewController 负责鼠标事件驱动的视图变换、拾取更新和相关交互流程。Render::FITKGraphObjectOCC 则是窗口层接收的统一图形对象基类，也是 FITKOCCGraphAdaptor 输出对象的基础类型之一。

## 构建与集成

当前模块通过 CMake 构建为共享库：

- project(FITKRenderWindowOCC VERSION 1.0.1 LANGUAGES CXX)
- add_library(${PROJECT_NAME} SHARED)
- 启用 CMAKE_AUTOMOC、CMAKE_AUTOUIC、CMAKE_AUTORCC
- 输出目录为 output/bin_d 和 output/bin
- AUTOGEN_BUILD_DIR 位于 generate/FITKRenderWindowOCC/autogen

在上层工程中，FITKRenderWindowOCC 一般通过组件方式参与集成，而不是被单独启动。上游如 FITKOCCGraphAdaptor、GraphDataProvider 或界面层模块会向该窗口添加 Render::FITKGraphObjectOCC 体系对象，并复用其显示与交互能力。

## 使用示例

以下示例全部基于当前源码中已确认存在的接口。

### 1. 在组件工厂中注册 OCC 窗口组件

```cpp
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCCInterface.h"

QList<AppFrame::FITKComponentInterface *> ComponentFactory::createComponents()
{
    QList<AppFrame::FITKComponentInterface *> componentList;
    componentList << new Render::FITKGraph3DWindowOCCInterface;
    return componentList;
}
```

### 2. 按组件名获取并嵌入窗口

```cpp
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCCInterface.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"

void attachOccWindow(QLayout* layout)
{
    if (layout == nullptr || FITKAPP == nullptr) return;

    auto compBase = FITKAPP->getComponents()->getComponentByName("Graph3DWindowOCC");
    auto comp = dynamic_cast<Render::FITKGraph3DWindowOCCInterface*>(compBase);
    if (comp == nullptr) return;

    auto widget = comp->getWidget(0);
    if (widget == nullptr) return;

    layout->addWidget(widget);
}
```

### 3. 对渲染窗口执行常见操作

```cpp
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCC.h"

void setupGraphWindow(Render::FITKGraph3DWindowOCC* graphWidget)
{
    if (graphWidget == nullptr) return;

    graphWidget->enableSelect(true);
    graphWidget->enableShiftSelect(true);
    graphWidget->setParallelProjection(false);
    graphWidget->fitView();
}
```

## 注意事项

1. getWidget(const int port) 当前实现中直接 Q_UNUSED(port)，说明端口参数暂时没有参与窗口选择逻辑。
2. getWidget 每次调用都会 new 一个 FITKGraph3DWindowOCC，因此调用方应明确窗口生命周期和所有权。
3. 组件名 Graph3DWindowOCC 是源码中的固定字符串，应用层通过名称获取组件时应保持完全一致。
4. 该模块接收的是 Render::FITKGraphObjectOCC 体系对象，业务数据本身需要先经过适配后再加入窗口。
5. 如果交互异常，应优先检查 OCC 上下文初始化、Qt 事件转发以及第三方库部署，而不是先修改业务层。
6. 模块包含 qrc、ui 和 ts 资源文件，构建异常时应同时检查 AUTOUIC、AUTORCC 与翻译文件路径。

## 文档信息

- 编写日期：2026-04-09
- 最后更新时间：2026-04-23