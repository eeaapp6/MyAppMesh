# OperatorsGUI

## 简介

OperatorsGUI 是 CADOCC 应用中负责界面与三维图形交互实现的操作器模块。

它实现了 OperatorsInterface 中定义的图形事件、图形工具、图形拾取、模型树交互和视角切换等接口，并通过 FITKOperatorRepo 进行注册。

如果说 OperatorsInterface 定义了抽象协议，那么 OperatorsGUI 提供的就是当前应用的 GUI 侧具体实现。

它不负责创建几何命令数据，也不负责定义参数对话框。

它解决的是“主窗口、渲染视图、模型树和拾取系统如何用操作器方式响应用户交互”的问题。

## 模块定位

- 所在目录：OperatorsGUI
- 构建产物：共享库 OperatorsGUI
- 主要使用者：CADOCCApp 组件工厂、GUIFrame、GUIDialog、OperatorsModel
- 主要上游依赖：FITKAppFramework、FITKCore、FITKWidget、OperatorsInterface、FITKOCCGraphAdaptor、FITKRenderWindowOCC、GraphDataProvider、GUIFrame、GUIWidget、FITKInterfaceGeometry、FITKInterfaceModel、FITKGeoCompOCC、PickDataProvider、Qt、OCC

该模块既是组件，也是一组具体操作器实现集合。

## 架构与目录

当前目录主要包含：

```text
OperatorsGUI
├── OpersGUIInterface
├── OperGraphEvent3D
├── OperGraphPreprocess
├── OperGraphPick
├── OperGraphTool
├── OperGUIView
├── OperTreeEvent
├── OperLicense
└── 构建文件
```

可以把它理解成三类能力：

- 图形事件与渲染刷新
- GUI 动作与视角控制
- 模型树/拾取等辅助交互

## 依赖关系

根据 OperatorsGUI/CMakeLists.txt，当前显式链接：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- OCC::OCC
- FITKAppFramework
- FITKCore
- FITKWidget
- OperatorsInterface
- FITKOCCGraphAdaptor
- FITKRenderWindowOCC
- GraphDataProvider
- GUIFrame
- GUIWidget
- FITKInterfaceGeometry
- FITKInterfaceModel
- FITKGeoCompOCC
- PickDataProvider

这表明该模块处于 GUI 实现链路中心，横跨：

- 主窗口
- 渲染窗口
- 图形对象提供器
- 拾取数据管理

## 主要类与职责

### OperatorsGUIInterface

OperatorsGUIInterface 继承自 AppFrame::FITKComponentInterface。

它是本模块的组件入口类。

当前真实接口包括：

- void init() override
- QString getComponentName() override

当前实现里 getComponentName() 返回：

- OpersGUIInterface

这也是组件工厂装配时可识别的名称。

### OperGraphEvent3D

OperGraphEvent3D 继承自 EventOper::GraphEventOperator。

它提供三维图形事件操作器的公共实现基础。

头文件中确认存在的关键受保护接口包括：

- Render::FITKGraph3DWindowOCC* getGraphWidget()
- void addGraphObjectToWidget(Exchange::FITKOCCGraphObject3D* obj, Render::FITKGraph3DWindowOCC* graphWidget, bool fitView = false)
- QList<Exchange::FITKOCCGraphObject3D*> getGraphObjectsByDataId(int dataObjId)

这说明具体图形操作器实现会以它为公共基础层。

### OperGraphPick

OperGraphPick 继承自 EventOper::GraphInteractionOperator。

它是拾取相关辅助功能操作器，当前重写：

- setGraphStateByPickInfo(int dataObjId = -1)
- setEnableGraphPickable(bool model, bool sketch, bool datum)
- setCustomPickMode()
- clearPickMode()

并通过以下宏注册到操作器仓库：

- Register2FITKOperatorRepo(GraphPick, OperGraphPick)

### OperGraphTool

OperGraphTool 继承自 EventOper::GraphToolOperator。

当前重写：

- measure
- clearMeasure
- initPickTool
- clearPickTool
- getPickToolObject
- setPickToolObjectVisible
- setPickDirectionAnchorTopoId
- setPickDirectionUserDirection
- getPickedDirection
- getPickedProjPosition
- setPickProjPosTargetId

并通过以下宏注册：

- Register2FITKOperatorRepo(GraphTool, OperGraphTool)

### OperGUIView

OperGUIView 继承自 Core::FITKActionOperator。

它用于处理视口相关菜单动作。

当前头文件中可确认其通过宏注册了大量 action 名：

- OperGUIView
- actionGUIViewFront
- actionGUIViewBack
- actionGUIViewTop
- actionGUIViewBottom
- actionGUIViewLeft
- actionGUIViewRight
- actionGUIViewISO
- actionGUIViewAutoFit
- actionGUIViewOrthogonal
- actionGUIViewPerspective
- actionGUIViewSaveImage
- actionGUIViewShowGrid
- actionGUIShowDatums

这使得主窗口按钮可以通过统一 action 字符串映射到视角操作器。

### OperTreeEvent

OperTreeEvent 继承自 EventOper::TreeEventOperator。

并通过宏注册：

- Register2FITKOperatorRepo(TreeEvent, OperTreeEvent)

它用于承接模型树点击、双击和更新逻辑。

## 构建与集成

OperatorsGUI 通过 CMake 构建为共享库：

- project(OperatorsGUI VERSION 1.0.0 LANGUAGES CXX)
- add_library(${PROJECT_NAME} SHARED)
- 启用 Qt 自动生成工具
- 输出目录为 output/bin_d 与 output/bin

同时保留 qmake 与 VS 工程文件。

## 使用示例

### 场景 1：通过仓库获取视角操作器

```cpp
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

auto* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionGUIViewFront");
if (oper)
{
    oper->execProfession();
}
```

这里使用 Core::FITKActionOperator 指针，是因为 OperGUIView 的 execGUI 和 execProfession 在派生类中都是私有重写，外部通常通过 QAction 与 ActionEventHandler，或通过基类接口触发执行。

### 场景 2：通过图形拾取操作器切换拾取模式

```cpp
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsGUI/OperGraphPick.h"

auto* pickOper = FITKOPERREPO->getOperatorT<GUIOper::OperGraphPick>("GraphPick");
if (pickOper)
{
    pickOper->setEnableGraphPickable(true, true, true);
    pickOper->setCustomPickMode();
}
```

### 场景 3：使用图形工具操作器初始化方向拾取工具

```cpp
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsGUI/OperGraphTool.h"

auto* toolOper = FITKOPERREPO->getOperatorT<GUIOper::OperGraphTool>("GraphTool");
if (toolOper)
{
    toolOper->initPickTool(true, EventOper::PTT_Direction);

    QList<double> dir;
    bool hasDir = toolOper->getPickedDirection(dir);
    Q_UNUSED(hasDir)
}
```

## 注意事项

- OperGUIView 的核心执行函数是私有重写，通常不应绕过主窗口动作系统直接调用内部逻辑。
- OperatorsGUI 是实现层，文档里不应把 OperatorsInterface 中的纯接口行为误写成此模块独有能力。
- 许多 GUI 行为依赖当前激活的 OCC 渲染窗口，若主窗口和渲染区未初始化，操作器可能无法生效。

最后更新时间：2026-04-23