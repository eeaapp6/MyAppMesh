# FITKInterfaceMeshGen - 网格生成接口抽象层

## 简介

FITKInterfaceMeshGen 是 FITK_Interface 中负责网格生成流程抽象的接口模块，位于模型/几何数据与具体 mesher 实现之间。模块把全局尺寸、局部尺寸、算法信息、驱动执行、结果处理、局部参数和种子点统一组织为一套稳定接口，供上层组件以相同方式接入不同网格引擎。

模块的核心入口是 `Interface::FITKMeshGenInterface`。该类按 mesher 标签维护一组上下文数据，把尺寸生成器、几何尺寸生成器、算法生成器、驱动器、处理器以及运行期管理对象绑定到一起，使业务层能够围绕同一 mesher 名称完成注册、读取、执行与清理。

## 模块定位

该模块主要承担以下职责：

- 定义多 mesher 共用的统一访问入口。
- 定义全局尺寸、全局算法和局部控制的数据对象。
- 定义尺寸生成器、算法生成器、驱动器、处理器等抽象基类。
- 定义线、面、体局部网格参数及其管理器。
- 定义边种子点、全局种子尺寸及其管理器。
- 提供按 mesher 和 dataObjId 清理运行态对象的统一接口。

从层级上看，FITKInterfaceMeshGen 位于 FITKCore 数据对象体系之上，依赖 `FITKInterfaceModel` 与 `FITKInterfaceGeometry` 提供模型和几何上下文；具体 mesher 适配通常位于 `FITK_Component` 或应用层，通过派生抽象基类接入。

## 架构概览

```text
FITKInterfaceMeshGen
├── 统一入口
│   ├── FITKMeshGenInterface
│   └── FITKMeshGenInterfaceData
├── 全局控制
│   ├── FITKGlobalMeshSizeInfo
│   └── FITKGlobalMeshGenerateAlgorithmInfo
├── 局部尺寸控制
│   ├── FITKRegionMeshSize / Manager
│   ├── FITKGeometryMeshSize / Manager
│   ├── FITKRegionGeometryRefine / Manager
│   └── FITKZonePoint / Manager
├── 流程抽象接口
│   ├── FITKAbstractMeshSizeInfoGenerator
│   ├── FITKAbstractGeometryMeshSizeGenerator
│   ├── FITKAbstractMeshAlgorithmInfoGenerator
│   ├── FITKAbstractMesherDriver
│   └── FITKAbstractMeshProcessor
└── 局部参数与种子
    ├── FITKMeshParameterBase
    ├── FITKMeshParameterLine / Surface / Volume
    ├── FITKMeshParameterManager
    ├── FITKMeshSeed
    └── FITKMeshSeedManager
```

可以把模块理解为两段式接口层：

- 接入期：注册某个 mesher 的生成器、驱动器和处理器。
- 运行期：维护全局/局部控制数据，并在执行前后完成读取、清理和结果处理。

## 编译依赖

根据当前目录中的 `CMakeLists.txt`，模块显式依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- FITKInterfaceModel
- FITKInterfaceGeometry

依赖关系可以概括为：

- `FITKCore` 提供数据对象、管理器和单例基础设施。
- `FITKInterfaceModel` 提供模型/拓扑上下文，使局部参数能够绑定虚拓扑对象。
- `FITKInterfaceGeometry` 提供几何上下文，使局部尺寸、几何细化和失效种子清理具备依据。
- Qt 提供 `QObject`、`QString`、`QHash`、`QStringList` 等基础类型和信号槽机制。

## 模块边界

该模块主要负责“网格生成前后的控制数据组织与执行入口抽象”，但不直接等同于具体网格引擎实现。可以将其边界理解为：

- 负责统一组织参数、执行入口和结果处理接口。
- 负责为上层提供跨 mesher 的一致访问方式。
- 不负责具体 mesher 的算法实现细节。
- 不负责最终网格结果的显示与可视化表达，这部分通常由 `FITKInterfaceMesh` 或更上层模块承接。

因此，上层在接入时通常需要同时准备两部分内容：

- 一组具体 mesher 的适配实现。
- 一套围绕模型对象生命周期的参数清理策略。

## 核心类

### FITKMeshGenInterface

模块对外的主入口。它以 mesher 标签为键维护 `FITKMeshGenInterfaceData`，负责：

- 注册和获取尺寸生成器、算法生成器、几何尺寸生成器。
- 注册和获取 mesher 驱动器、结果处理器。
- 获取区域尺寸、几何尺寸、区域几何细化、Zone 点等管理器。
- 按 `dataObjId` 获取或移除 `FITKMeshParameterManager` 与 `FITKMeshSeedManager`。
- 清理指定 mesher 或全部运行态数据。
- 查询当前已注册的 mesher 名称列表。

这个设计的关键点是“按 mesher 隔离上下文”。业务层只需在注册、查询、执行和清理时保持 mesher 标签一致，就能在统一流程下接入多个网格引擎。

### FITKMeshGenInterfaceData

`FITKMeshGenInterface` 的内部上下文容器，用于归档单个 mesher 对应的核心对象，包括尺寸生成器、算法生成器、几何尺寸生成器、驱动器、处理器以及全局网格尺寸信息。它的作用是保证同一 mesher 的运行期对象集中管理，而不是散落在多个模块中。

### 全局控制对象

- `FITKGlobalMeshSizeInfo`：表达全局尺寸配置，通常至少包含全局尺寸、最小尺寸、最大尺寸等控制量。
- `FITKGlobalMeshGenerateAlgorithmInfo`：表达全局算法配置，供具体 mesher 将抽象算法参数翻译为自身可识别的选项。

### 局部尺寸控制对象

- `FITKRegionMeshSize` 及 Box、Cylinder、Sphere、Geom 派生类：用于表达区域型局部尺寸约束。
- `FITKGeometryMeshSize`：用于表达与特定几何对象绑定的局部尺寸限制。
- `FITKRegionGeometryRefine`：用于表达区域几何细化或优化控制。
- `FITKZonePoint`：用于表达区域内部参考点、材料点或 mesher 需要的辅助输入。

### 流程抽象接口

- `FITKAbstractMeshSizeInfoGenerator`：定义全局尺寸信息生成接口。
- `FITKAbstractGeometryMeshSizeGenerator`：定义几何尺寸信息生成接口。
- `FITKAbstractMeshAlgorithmInfoGenerator`：定义算法信息生成接口。
- `FITKAbstractMesherDriver`：定义网格执行入口，是接口层与具体 mesher 内核之间最关键的隔离点。
- `FITKAbstractMeshProcessor`：定义网格结果或中间产物的统一处理入口。

### 局部参数与种子管理

- `FITKMeshParameterBase` 及其线、面、体派生类：表达按虚拓扑对象施加的局部网格参数。
- `FITKMeshParameterManager`：按虚拓扑 ID 管理线、面、体参数对象，支持获取、创建、列举和清理。
- `FITKMeshSeed`：表达边种子点及其局部约束、方法、偏置等信息。
- `FITKMeshSeedManager`：负责按虚拓扑 ID 管理种子点、筛选种子、计算种子位置、清理失效种子，并维护全局种子开关与全局种子尺寸。

## 构建与集成

### CMake

当前模块通过 `add_library(${PROJECT_NAME} SHARED)` 构建为共享库，并启用了：

- `CMAKE_AUTOMOC`
- `CMAKE_AUTORCC`
- `CMAKE_AUTOUIC`

构建输出目录为：

- Debug：`output/bin_d`
- Release：`output/bin`

本目录 `CMakeLists.txt` 通过 `link_fitk_library` 宏完成内部库链接，说明该模块的设计目标是在上层工程中统一编译和链接，而不是独立发布。

### qmake

目录中同时保留 `.pro` 和 `.pri` 文件，说明模块仍兼容现有 qmake 工程链路，便于旧工程继续复用。

## 典型接入流程

上层组件通常按以下顺序使用该模块：

1. 初始化阶段注册某个 mesher 对应的生成器、驱动器和处理器。
2. 在参数编辑阶段写入全局尺寸、局部尺寸、局部参数和种子点数据。
3. 执行阶段通过 `FITKAbstractMesherDriver` 启动网格划分。
4. 结果返回后通过 `FITKAbstractMeshProcessor` 处理网格输出。
5. 在模型切换或几何变更后清理失效参数和种子对象。

## 使用示例

下面给出一个最小闭环示例，展示“注册 mesher 能力”和“写入参数并执行”的基本方式。

### 1. 注册某个 mesher

```cpp
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshSizeInfoGenerator.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractGeometryMeshSizeGenerator.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshAlgorithmInfoGenerator.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshProcessor.h"

void registerMesher(
    Interface::FITKAbstractMeshSizeInfoGenerator* sizeGenerator,
    Interface::FITKAbstractGeometryMeshSizeGenerator* geoSizeGenerator,
    Interface::FITKAbstractMeshAlgorithmInfoGenerator* algoGenerator,
    Interface::FITKAbstractMesherDriver* driver,
    Interface::FITKAbstractMeshProcessor* processor)
{
    Interface::FITKMeshGenInterface* meshInterface = Interface::FITKMeshGenInterface::getInstance();
    if (meshInterface == nullptr)
    {
        return;
    }

    const QString mesher = "ACIS";
    meshInterface->regMeshSizeGenerator(sizeGenerator, mesher);
    meshInterface->regGeometryMeshSizeGenerator(geoSizeGenerator, mesher);
    meshInterface->regMeshAlgorithmGenerator(algoGenerator, mesher);
    meshInterface->regMesherDriver(driver, mesher);
    meshInterface->regMeshProcessor(processor, mesher);
}
```

### 2. 写入参数并执行

```cpp
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshParameterManager.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshParameterLine.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshSeedManager.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"

void prepareMesher(int dataObjId, int edgeTopoId)
{
    Interface::FITKMeshGenInterface* meshInterface = Interface::FITKMeshGenInterface::getInstance();
    if (meshInterface == nullptr)
    {
        return;
    }

    const QString mesher = "ACIS";

    Interface::FITKGlobalMeshSizeInfo* globalInfo = meshInterface->getGlobalMeshSizeInfo(mesher);
    if (globalInfo != nullptr)
    {
        globalInfo->setGlobalSize(5.0);
        globalInfo->setMinSize(1.0);
        globalInfo->setMaxSize(10.0);
    }

    Interface::FITKMeshParameterManager* paraMgr = meshInterface->getMeshParameterManager(dataObjId, mesher, true);
    if (paraMgr != nullptr)
    {
        Interface::FITKMeshParameterLine* linePara = paraMgr->getLineMeshingParameters(edgeTopoId, true);
        Q_UNUSED(linePara);
    }

    Interface::FITKMeshSeedManager* seedMgr = meshInterface->getMeshSeedManager(dataObjId, mesher, true);
    if (seedMgr != nullptr)
    {
        seedMgr->setGlobalSeedDataTag(true);
        seedMgr->setGlobalSeedSize(2.5);
    }

    Interface::FITKAbstractMesherDriver* driver = meshInterface->getMesherDriver(mesher);
    if (driver != nullptr)
    {
        driver->startMesher();
    }
}
```

## 使用建议

- mesher 标签在注册、获取和清理时必须保持一致，否则会进入不同上下文。
- 长期挂在模型对象上的局部控制，优先放入 `FITKMeshParameterManager` 或 `FITKMeshSeedManager`，不要只保存在界面状态中。
- 几何发生变化后，应及时清理失效种子和不再有效的局部控制数据。
- 全局尺寸、局部尺寸、局部参数、种子点属于不同层次的控制信息，业务层应明确它们的分工，避免重复覆盖。
- 若接入新的 mesher，优先复用现有抽象基类，而不是新增平行接口。

## 注意事项

- 当前模块只定义接口层，不保证每个具体 mesher 都实现了全部抽象能力。
- `getGlobalMeshSizeInfo()`、`getGlobalMeshGenerateAlgorithmInfo()` 等接口依赖对应生成器已注册；未注册时调用方需要处理空指针。
- `getMeshParameterManager()` 和 `getMeshSeedManager()` 以 `dataObjId` 为粒度维护数据，模型删除或切换后建议同步移除。
- 模块同时使用 Qt 容器和 FITK 数据对象体系，调用方需要统一对象所有权策略，避免重复释放。
- 本 README 基于当前目录头文件与构建配置整理，重点覆盖接口职责和接入方式，不展开具体 mesher 私有适配细节。

最后更新时间：2026-04-23
