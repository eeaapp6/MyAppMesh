# OperatorsInterface

## 简介

OperatorsInterface 是 CADOCC 应用中面向操作器的抽象接口层。

它定义了图形事件、图形工具、图形拾取、参数窗口交互以及模型树事件等操作器基类，供上层具体实现复用。

该模块的定位与 FITKCore 中的 FITKAbstractOperator、FITKActionOperator 有明显差异。

FITKCore 提供的是通用操作器抽象。

OperatorsInterface 提供的是“本应用 GUI/建模场景下的专用操作器接口约定”。

它解决的是“哪些操作器需要操作三维窗口、哪些操作器需要驱动参数对话框、哪些操作器负责模型树交互”这类应用级接口分层问题。

## 模块定位

- 所在目录：OperatorsInterface
- 构建产物：共享库 OperatorsInterface
- 主要使用者：OperatorsGUI、OperatorsModel、GUIWidget 中的 GUIDialogBase
- 主要上游依赖：FITKCore、FITKAppFramework、Qt

该模块本身不注册大量具体动作。

它的核心价值在于定义稳定的操作器基类。

## 架构与目录

当前目录中的主要类型如下：

```text
OperatorsInterface
├── GraphEventOperator
├── GraphToolOperator
├── GraphInteractionOperator
├── ParaWidgetInterfaceOperator
├── TreeEventOperator
└── 构建文件
    ├── OperatorsInterfaceAPI.h
    ├── CMakeLists.txt
    ├── .pro / .pri
    └── VS 工程文件
```

这套结构体现出本模块的五类抽象职责：

- 图形刷新与预览
- 图形工具与辅助图元
- 三维拾取模式控制
- 参数窗口绑定与收尾
- 模型树事件处理

## 依赖关系

根据 OperatorsInterface/CMakeLists.txt，模块显式链接：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- FITKAppFramework

没有直接依赖 GUIFrame、GraphDataProvider 或具体几何模块。

这进一步说明它是抽象接口层，而不是实现层。

## 主要类与职责

### GraphEventOperator

GraphEventOperator 继承自 Core::FITKAbstractOperator。

它是前处理三维可视对象事件接口基类。

当前头文件中确认存在的关键能力包括：

- initialize3D()
- updateGraph(int dataObjId, bool forceUpdate = false)
- updateGraphs()
- updateGraphRecursively(int dataObjId, int refLayer = 2, bool recDown = true)
- updateGraphVisibility(int dataObjId)
- preview(int dataObjId, PreviewType type = PT_Default, QColor color = QColor())
- clearPreview(int dataObjId = -1)
- showGlobalComponent(int dataObjId)
- clearGlobalComponent()
- setDatumsVisibility(bool visible)
- setSketchTopMost(int dataObjId = -1)
- clearSketchTopMost()
- setShapeTransparency(int cmdId, int geomType, QList<int> geoIndice, double trans)
- setShapeHidden(int cmdId, int geomType, QList<int> geoIndice)
- clearShapeFaceTransparency()
- highlight(int dataObjId, QVariant info = QVariant(), QColor color = QColor())
- clearHighlight()
- reRender(bool fitView = false)
- enableSketchMode(bool isOn, int virtualTopoID)

这说明 GraphEventOperator 几乎覆盖了“图形刷新、预览、高亮、可见性、草图模式”整套界面交互语义。

### GraphToolOperator

GraphToolOperator 继承自 Core::FITKAbstractOperator。

它主要服务测量和辅助拾取工具。

已确认的接口包括：

- measure(QList<int> items, MeasureToolType type, QColor color = QColor())
- clearMeasure()
- initPickTool(bool calculateCenter, PickToolType type, QColor color = QColor())
- clearPickTool()
- getPickToolObject(PickToolType type)
- setPickToolObjectVisible(PickToolType type, bool visible)
- setPickDirectionAnchorTopoId(int mainId, int subId)
- setPickDirectionUserDirection(QList<double> dir)
- bool getPickedDirection(QList<double>& dir)
- bool getPickedProjPosition(QList<double>& pos)
- setPickProjPosTargetId(int targetCmdId, int targetTopoId)

它为 GUI 工具条上的测量、方向、投影拾取等功能提供抽象接口。

### GraphInteractionOperator

GraphInteractionOperator 继承自 Core::FITKAbstractOperator。

它负责三维拾取状态控制，当前接口包括：

- setGraphStateByPickInfo(int dataObjId = -1)
- setEnableGraphPickable(bool model, bool sketch, bool datum)
- setCustomPickMode()
- clearPickMode()

这类接口的目标不是“读取拾取结果”，而是“开启或关闭拾取模式”。

### ParaWidgetInterfaceOper

ParaWidgetInterfaceOper 继承自 Core::FITKActionOperator。

它是参数窗口相关操作器基类。

当前确认存在的公共接口很简洁：

- finalize(bool updateCommand = false)

并维护 _currentStage，用于多阶段参数交互。

GUIDialogBase 通过 setOperator 绑定的对象，通常就是它的派生类。

### TreeEventOperator

TreeEventOperator 继承自 Core::FITKAbstractOperator。

它定义模型树相关事件接口：

- updateGeometryTree()
- mouseClicked(QTreeWidgetItem* item)
- mouseDoubleClicked(QTreeWidgetItem* item)

## 构建与集成

OperatorsInterface 通过 CMake 构建为共享库：

- project(OperatorsInterface VERSION 1.0.0 LANGUAGES CXX)
- add_library(${PROJECT_NAME} SHARED)
- 启用 Qt 自动生成工具
- 输出目录为 output/bin_d 与 output/bin

由于该模块不包含 .ui 文件和复杂资源，构建体量相对较轻。

## 模块层级关系

当前代码结构表明：

1. OperatorsInterface 定义抽象接口。
2. OperatorsGUI 提供图形与界面事件操作器实现。
3. OperatorsModel 提供建模与文件操作器实现。
4. GUIWidget/GUIDialog 使用 ParaWidgetInterfaceOper 与图形接口配合窗口行为。

这使得操作器体系能保持“接口层”和“实现层”分离。

## 使用示例

### 场景 1：通过图形事件操作器刷新对象与预览

```cpp
#include "OperatorsInterface/GraphEventOperator.h"

EventOper::GraphEventOperator* oper = /* 具体实现对象 */ nullptr;

if (oper)
{
    oper->updateGraph(dataId, true);
    oper->preview(dataId, EventOper::PT_Default);
    oper->reRender(true);
}
```

### 场景 2：通过图形工具操作器启用测量与拾取工具

```cpp
#include "OperatorsInterface/GraphToolOperator.h"

EventOper::GraphToolOperator* toolOper = /* 具体实现对象 */ nullptr;

if (toolOper)
{
    toolOper->measure({dataId}, EventOper::MTT_Distance);
    toolOper->initPickTool(true, EventOper::PTT_Direction);
}
```

### 场景 3：通过拾取接口切换拾取状态

```cpp
#include "OperatorsInterface/GraphInteractionOperator.h"

EventOper::GraphInteractionOperator* pickOper = /* 具体实现对象 */ nullptr;

if (pickOper)
{
    pickOper->setEnableGraphPickable(true, true, true);
    pickOper->setCustomPickMode();
}
```

### 场景 4：参数窗口结束后统一收尾

```cpp
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

EventOper::ParaWidgetInterfaceOper* paraOper = /* 具体实现对象 */ nullptr;

if (paraOper)
{
    paraOper->finalize(true);
}
```

## 注意事项

- 该模块只定义接口，不应在 README 中暗示其已经具备完整 OCC 渲染实现。
- GraphEventOperator 和 GraphToolOperator 的大多数函数是虚函数，实际行为要看 OperatorsGUI 中的具体实现。
- ParaWidgetInterfaceOper 继承自 FITKActionOperator，而非纯抽象接口，因此它既是界面交互接口，也是可执行动作的一部分。

最后更新时间：2026-04-23