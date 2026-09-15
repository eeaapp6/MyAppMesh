# PickDataProvider

## 简介

PickDataProvider 是 CADOCC 应用中的三维拾取数据管理模块。

它负责把 OCC 三维窗口中的拾取结果整理成统一的数据对象，并向上提供拾取状态、拾取结果列表、方向拾取和投影坐标拾取等能力。

该模块不负责创建全部图形对象，也不负责建模命令本身。

它解决的是“用户在三维视窗中点选、框选或拾取辅助工具对象后，应用如何统一存储和查询这些拾取结果”的问题。

## 模块定位

- 所在目录：PickDataProvider
- 构建产物：共享库 PickDataProvider
- 主要依赖者：GUIDialog、OperatorsGUI、OperatorsModel、GUIFrame
- 主要上游依赖：FITKCore、FITKAppFramework、FITKInterfaceGeometry、FITKInterfaceModel、FITKGeoCompOCC、FITKRenderWindowOCC、FITKOCCGraphAdaptor、FITKWidget、GUIFrame、Qt、OCC

从依赖链路看，PickDataProvider 位于“渲染交互”和“业务操作器”之间。

## 架构与目录

当前模块主要由以下几部分构成：

```text
PickDataProvider
├── GraphPickedDataProvider
├── GraphPickedData
├── GUIPickInfo
├── ContextMenuHandler
└── 构建文件
    ├── PickDataProviderAPI.h
    ├── CMakeLists.txt
    ├── .pro / .pri
    └── VS 工程文件
```

其中：

- GraphPickedDataProvider 负责拾取结果的单例管理。
- GraphPickedData 负责保存单次或单组拾取数据。
- GUIPickInfo 提供拾取方式相关元数据。
- ContextMenuHandler 负责和上下文菜单交互相关的辅助逻辑。

## 依赖关系

根据 PickDataProvider/CMakeLists.txt，当前模块显式链接：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- OCC::OCC
- FITKCore
- FITKAppFramework
- FITKInterfaceGeometry
- FITKInterfaceModel
- FITKGeoCompOCC
- FITKRenderWindowOCC
- FITKOCCGraphAdaptor
- FITKWidget
- GUIFrame

这说明该模块需要同时接触：

- 几何对象与虚拓扑数据
- OCC 渲染窗口
- 主界面上下文
- 图形对象适配层

## 主要类型与职责

### GraphPickedDataProvider

GraphPickedDataProvider 是当前模块的核心单例。

当前已确认的关键接口包括：

- static GraphPickedDataProvider* getInstance()
- static void Delete()
- void clearPickedData(bool clearSelecttion = true)
- bool isPickStatus()
- void init()
- QList<GraphPickedData*> getPickedList()
- void removeSelected(int cmdId, Interface::FITKGeoEnum::VTopoShapeType sType, int vTopoIndex)

它还提供信号：

- sig_dataPicked()
- sig_directionPicked()
- sig_projPosPicked()

这些信号说明该模块除了保存数据，还承担交互通知职责。

### GraphPickedData

GraphPickedData 用来保存单条拾取结果。

当前代码中定义了多个相关枚举：

- PickedDataType
- PickedMouseType
- GraphDataType

以及一个 GraphInfo 结构：

- GraphDataType Type
- int DataObjId

GraphPickedData 公开接口包括：

- 多组构造函数，支持虚拓扑、基准元素、图形对象等不同拾取来源
- bool isEmpty()
- void setGraphInfo(GraphInfo graphInfo)
- GraphInfo getGraphInfo()
- GUI::GUIPickInfoStru getPickedInfo()
- Interface::FITKAbsVirtualTopo* getVirtualTopo()
- QList<Interface::FITKAbsVirtualTopo*> getVirtualTopoAll()
- QList<int> getVirtualTopoIndice()
- QList<Interface::VirtualShape> getVirtualShapes()
- Interface::FITKAbsGeoShapeAgent* getGeoShapeAgent()
- int getDatumID()
- void setDatumID(int datumID)
- void setGraphObject(Exchange::FITKOCCGraphObject3D* obj)
- QVariant getGraphOutputInfo(int outputRole = -1)
- template<class T> T getGraphOutputInfoAs(int outputRole = -1)
- void appendVirtualTopo(Interface::FITKAbsVirtualTopo* vTopo)

这说明它并不是一个简单的 ID 容器，而是一个面向多种拾取场景的统一封装对象。

### ContextMenuHandler

从命名和依赖可看出，ContextMenuHandler 负责渲染区右键菜单与当前选中对象之间的桥接。

它不是本模块唯一入口，但通常会和 GraphPickedDataProvider 协同使用。

### GUIPickInfo

GUIPickInfo 用于描述拾取模式、拾取对象类型以及相关辅助信息。

GraphPickedData 的构造函数直接依赖 GUIPickInfoStru，因此 GUI 交互层和拾取数据层通过它保持一致。

## 构建与集成

PickDataProvider 通过 CMake 构建为共享库：

- project(PickDataProvider VERSION 1.0.0 LANGUAGES CXX)
- add_library(${PROJECT_NAME} SHARED)
- 启用 Qt 自动生成工具
- Debug 与 Release 分别输出到 output/bin_d 和 output/bin

当前目录也保留 qmake 与 VS 工程文件，说明它需要兼容现有多套工程组织。

## 模块层级关系

从现有头文件可推断，模块处于如下链路：

1. GUIFrame 承载三维窗口。
2. OperatorsGUI 控制拾取模式启停。
3. PickDataProvider 从窗口与交互上下文中收集拾取结果。
4. GUIDialog 或 OperatorsModel 读取拾取列表并完成参数回填、方向计算或几何编辑。

因此它是交互数据层，而不是图形显示层。

## 使用示例

### 场景 1：初始化并读取拾取列表

```cpp
#include "PickDataProvider/GraphPickedDataProvider.h"

GraphData::GraphPickedDataProvider* provider = GraphData::GraphPickedDataProvider::getInstance();
provider->init();

QList<GraphData::GraphPickedData*> pickedList = provider->getPickedList();
bool picking = provider->isPickStatus();

Q_UNUSED(pickedList)
Q_UNUSED(picking)
```

### 场景 2：清理拾取数据并监听变化

```cpp
#include "PickDataProvider/GraphPickedDataProvider.h"

auto* provider = GraphData::GraphPickedDataProvider::getInstance();

QObject::connect(
    provider,
    &GraphData::GraphPickedDataProvider::sig_dataPicked,
    []() {
        // 拾取数据发生变化后刷新参数区或预览
    }
);

provider->clearPickedData(true);
```

### 场景 3：读取单条拾取结果中的虚拓扑和输出信息

```cpp
#include "PickDataProvider/GraphPickedData.h"

GraphData::GraphPickedData* picked = /* 已获取的拾取对象 */ nullptr;

if (picked && !picked->isEmpty())
{
    GraphData::GraphInfo info = picked->getGraphInfo();
    QList<int> topoIds = picked->getVirtualTopoIndice();
    QVariant output = picked->getGraphOutputInfo();

    Q_UNUSED(info)
    Q_UNUSED(topoIds)
    Q_UNUSED(output)
}
```

### 场景 4：按命令与虚拓扑索引取消选中

```cpp
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"

auto* provider = GraphData::GraphPickedDataProvider::getInstance();
provider->removeSelected(cmdId, Interface::FITKGeoEnum::VTopoShapeType::VS_Face, topoIndex);
```

示例中的 removeSelected、getPickedList、getGraphInfo、getVirtualTopoIndice 都是当前真实接口。

## 注意事项

- GraphPickedDataProvider 是单例，生命周期由模块自身控制，调用 Delete 前需要确认没有外部对象继续持有 GraphPickedData 指针。
- clearPickedData 的 clearSelecttion 参数拼写沿用现有代码，不应在 README 中私自改名。
- removeSelected 依赖命令 ID、形状类型和虚拓扑索引三者同时正确匹配，README 示例必须保留这些参数语义。
- 当前模块和 OCC 交互上下文强绑定，若后续引入其他渲染后端，需要单独适配。

最后更新时间：2026-04-23