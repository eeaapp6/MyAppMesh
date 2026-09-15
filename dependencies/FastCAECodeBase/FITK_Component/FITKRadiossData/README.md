# FITKRadiossData - Radioss 求解数据模型组件

## 简介

FITKRadiossData 是 FastCAE 集成工具包（FITK）中负责承载 Radioss 有限元分析软件前处理与求解数据模型的核心组件。该模块定义了 Radioss 仿真工作流中所需的全部数据类型，涵盖算例管理、几何部件、网格模型、材料模型、单元属性、物理边界、初始场、相互作用、刚性墙、连接、重力场、求解设置、结果请求、时程输出、探针测量以及失效模型等数据域，并通过统一的数据管理器接口完成对象的创建、查询与维护。

FITKRadiossData 的设计目标是为上层 Radioss 读写适配器和界面模块提供统一的数据抽象，使得不同功能模块之间能够基于相同的数据对象进行交互，而无需关注数据对象的底层实现细节。

## 模块定位

FITKRadiossData 位于 `FITK_Component` 下，属于业务数据组件层，主要职责包括：

- 定义 Radioss 各数据类型对应的 C++ 类及其属性接口。
- 通过 `RadiossKeyWord` 宏建立数据类与 Radioss 关键字（如 `/MAT/LAW1`、`/PROP/TYPE1`）之间的映射关系。
- 提供各类数据的统一管理器（如材料管理器、边界条件管理器、相互作用管理器）。
- 以 `FITKRadiossCase` 为入口聚合完整算例数据，支持多算例切换。
- 通过 `FITKAbstractRadiossData` 基类提供可见性控制和数据类型标记能力。

## 架构与目录

从当前目录中的头文件和源文件看，FITKRadiossData 可以概括为以下几组能力：

```text
FITKRadiossData
├── 数据抽象基类
│   ├── FITKAbstractRadiossData
│   ├── FITKAbstractBCS
│   ├── FITKAbstractRWall
│   ├── FITKAbstractInteraction
│   ├── FITKAbstractInitialField
│   ├── FITKAbstractProbe
│   ├── FITKAbstractTimeHistory
│   ├── FITKAbstractConnection
│   ├── FITKAbstractEquationOfState
│   ├── FITKAbstractFailureModel
│   └── FITKAbstractBEM
├── 算例与网格管理
│   ├── FITKRadiossCase
│   ├── FITKRadiossCaseManager
│   ├── FITKRadiossMeshModel
│   ├── FITKRadiossGeomPart
│   ├── FITKRadiossNodesParts
│   └── FITKRadiossMeshTopo
├── 材料模型
│   ├── FITKAbstractRadMaterial
│   ├── FITKMaterialElastic
│   ├── FITKMaterialPlasJohns
│   ├── FITKMaterialPlasBrit
│   ├── FITKMaterialPlasTab
│   └── FITKMaterialHydro
├── 单元属性
│   ├── FITKPropAbstract
│   ├── FITKPropShell
│   ├── FITKPropSolid
│   ├── FITKPropBeam
│   ├── FITKPropBeamSPR
│   ├── FITKPropSandwichShell
│   └── FITKPropSPH
├── 物理与边界
│   ├── FITKRadiossBCS
│   ├── FITKInitialField
│   ├── FITKRWall
│   ├── FITKGravity
│   ├── FITKInteractionTied
│   └── FITKInteractionMultiUsageImpact
├── 求解配置
│   ├── FITKRadiossSolverSettings
│   ├── FITKRadiossResultRequest
│   └── FITKRadiossSolution
├── 后处理与输出
│   ├── FITKProbeAcceleration
│   ├── FITKProbeSection
│   ├── FITKTimeHistoryACCEL
│   ├── FITKTimeHistoryINTER
│   ├── FITKTimeHistoryRBODY
│   ├── FITKTimeHistoryRWALL
│   └── FITKTimeHistorySECTIO
├── 辅助数据
│   ├── FITKNonLinerCurve
│   ├── FITKConnectionRBODY
│   ├── FITKEquationOfStatePolynomial
│   ├── FITKFailureModelBIQUAD
│   └── FITKFailureModelJOHNSON
└── 表面与集合
    ├── FITKAbstractRadiossSurface
    ├── FITKRadiossSurfacePart
    ├── FITKRadiossSurfaceSeg
    ├── FITKRadiossNodeGroup
    └── FITKRadiossElementGroup
```

## 依赖关系

### 编译依赖

根据 `FITK_Component/FITKRadiossData/CMakeLists.txt`，该模块当前显式依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- FITKInterfaceModel
- FITKInterfaceGeometry

### 模块层级关系

- FITKRadiossData 依赖 `FITKCore` 提供的基础对象模型、数据管理模板和对象容器能力。
- FITKRadiossData 同时依赖 `FITKInterfaceModel` 和 `FITKInterfaceGeometry` 中定义的抽象部件与几何接口。
- 上层 Radioss 读写适配器（位于 `FITK_Interface/FITKInterfaceIORadioss`）依赖 FITKRadiossData 进行数据序列化与反序列化。
- 界面模块和应用程序框架通过 `FITKRadiossCase` 及其下级管理器访问和修改算例数据。

## 主要类与功能说明

### 数据抽象基类

#### `Radioss::FITKAbstractRadiossData`

所有 Radioss 数据类型的共同基类。它通过 `RadiossKeyWord` 宏为派生类关联 Radioss 关键字字符串，并提供 `setVisible(bool)` / `getVisible()` 接口控制对象在 RAD 文件中的读写可见性。

```cpp
RadiossKeyWord(FITKMaterialElastic, /MAT/LAW1)
```

#### `Radioss::FITKAbstractBCS`

边界条件抽象基类，定义坐标系 ID、组 ID 等通用属性，支持 BCS、Cyclic、BCSLAGMUL 等边界条件类型。`FITKRadiossBCSManager` 负责管理边界条件集合。

#### `Radioss::FITKAbstractRWall`

刚性壁抽象基类，支持 PLANE、CYL、SPHER、PARAL 四种几何类型，并可配置滑移行为（无摩擦滑移、绑定、有摩擦滑移）。

#### `Radioss::FITKAbstractInteraction`

相互作用抽象基类，定义主面 ID 和从节点组 ID，支持 Tied、KinematicTied、MultiUsageImpact 等相互作用类型。

#### `Radioss::FITKAbstractProbe`

探针数据抽象基类，支持 Acceleration 和 Section 两种探针类型。`FITKProbeManager` 负责管理探针集合。

#### `Radioss::FITKAbstractTimeHistory`

时程输出请求抽象基类，支持 ACCEL、INTER、RWALL、RBODY、SECTIO、SPHCEL 等时程类型。

### 算例与网格管理

#### `Radioss::FITKRadiossCase`

单个 Radioss 算例的数据聚合对象，通过 `FITKRadiossCaseManager` 进行多算例管理。算例内部聚合了以下管理器：

- `getGeomPartManager()` - 几何部件管理器
- `getMeshModel()` - 网格模型
- `getMaterialManager()` - 材料管理器
- `getSectionManager()` - 截面管理器
- `getEquationOfState()` - 状态方程管理器
- `getBCSManager()` - 边界条件管理器
- `getInitialFieldManager()` - 初始场管理器
- `getRWallManager()` - 刚性墙管理器
- `getInteractionManager()` - 相互作用管理器
- `getGravityManager()` - 重力场管理器
- `getConnectionManager()` - 连接管理器
- `getFailureModelManager()` - 失效模型管理器
- `getProbeManager()` - 探针管理器
- `getNonLinerCurveManager()` - 非线性曲线管理器
- `getDefaultPropValueManager()` - 默认属性值管理器
- `getDefaultInteractionManager()` - 默认相互作用管理器
- `getSolverSettings()` - 求解设置
- `getResultRequest()` - 结果请求
- `getSolutionManager()` - 求解过程管理器

#### `Radioss::FITKRadiossCaseManager`

多算例管理器（继承数据管理模板），提供 `setCurrentCaseID(int)` / `getCurrentCaseID()` / `getCurrentCase()` 接口切换和管理当前激活算例。

#### `Radioss::FITKRadiossMeshModel`

Radioss 网格模型对象，继承自 `Interface::FITKAbstractModel`。提供节点列表、零件管理器、子集管理器、组件管理器和网格拓扑的访问接口，并支持节点边界框查询与数据清理。

### 材料与属性

#### 材料类

- `FITKMaterialElastic` - 弹性材料（`/MAT/LAW1`）
- `FITKMaterialPlasJohns` - Johnson-Cook 塑性材料（`/MAT/LAW2`）
- `FITKMaterialPlasBrit` - 塑性-断裂材料（`/MAT/LAW27`）
- `FITKMaterialPlasTab` - 表格式塑性材料（`/MAT/LAW44`）
- `FITKMaterialHydro` - 流体材料（`/MAT/LAW6`）

#### 属性类

- `FITKPropShell` - 壳单元属性（`/PROP/TYPE1`）
- `FITKPropSolid` - 实体单元属性（`/PROP/TYPE14`）
- `FITKPropBeam` - 梁单元属性（`/PROP/TYPE3`）
- `FITKPropBeamSPR` - 梁-弹簧属性
- `FITKPropSandwichShell` - 夹芯壳属性
- `FITKPropSPH` - SPH 粒子属性

### 求解配置与后处理

#### `Radioss::FITKRadiossSolverSettings`

求解设置类，包含运行时间参数、全局时间参数、能量参数、输出控制参数等，用于配置 Radioss 求解器的运行行为。

#### `Radioss::FITKRadiossResultRequest`

结果请求类，定义节点动画输出、单元动画输出、Brick/Shell/Beam 动画输出等结果数据项。

#### `Radioss::FITKRadiossSolution`

求解执行类，聚合边界条件、初始场、刚性墙、重力、相互作用、探针、求解设置和结果请求管理器，提供完整的求解数据准备入口。

## 使用示例

### 示例1：在全局数据工厂中注册 FITKRadiossCase

```cpp
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

Core::FITKAbstractDataObject* GlobalDataFactory::createPhysicsData()
{
    return new Radioss::FITKRadiossCase;
}
```

### 示例2：获取当前算例的材料管理器并遍历材料

```cpp
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"

void updateMaterialList(QListWidget* listWidget)
{
    if (listWidget == nullptr) return;

    listWidget->clear();
    Radioss::FITKRadiossCase* dataCase =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (dataCase == nullptr) return;

    Interface::FITKMaterialManager* manager = dataCase->getMaterialManager();
    if (manager == nullptr) return;

    for (int i = 0; i < manager->getDataCount(); ++i)
    {
        Interface::FITKAbstractMaterial* mat = manager->getDataByIndex(i);
        if (mat == nullptr) continue;
        listWidget->addItem(mat->getDataObjectName());
    }
}
```

### 示例3：创建刚性墙并添加到算例

```cpp
#include "FITK_Component/FITKRadiossData/FITKRWall.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

void createRWall(Radioss::FITKRadiossCase* caseData, int nodeId)
{
    if (caseData == nullptr) return;

    Radioss::FITKRadiossRWallManager* manager = caseData->getRWallManager();
    if (manager == nullptr) return;

    Radioss::FITKRWall* rwall = new Radioss::FITKRWall;
    rwall->setNodeId(nodeId);
    rwall->setSlidingType(Radioss::FITKAbstractRWall::Sliding);

    manager->addData(rwall);
}
```

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `project(FITKRadiossData VERSION 1.0.0 LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`、`AUTOUIC`、`AUTORCC`
- 输出目录为 `output/bin_d` 和 `output/bin`
- 编译定义包含 `FITKRadiossData_API`

### 在仓库中的参与方式

FITKRadiossData 作为组件层模块被上层适配器模块和界面模块依赖。它通常与 `FITK_Interface/FITKInterfaceIORadioss` 配合，完成 Radioss 输入文件（.rad）的解析与生成。

## 使用建议

- 新增 Radioss 数据类型时，优先考虑是否可以通过继承 `FITKAbstractRadiossData` 并使用 `RadiossKeyWord` 宏来接入现有管理体系。
- 算例数据应通过 `FITKRadiossCase` 统一入口访问，不要直接操作子管理器的内部状态。
- 材料和属性的管理器由 `FITKRadiossCase` 在构造时自动创建，无需手动初始化。
- 若需要在 RAD 文件中控制某个对象的读写可见性，可通过 `setVisible(false)` 排除该对象。

## 注意事项

- FITKRadiossData 定义的是数据模型本身，不包含文件读写或界面展示的具体实现，这些能力由上层适配器和界面模块提供。
- 部分数据类（如 `FITKRadiossSurfacePart`）已在头文件中注释掉，若需使用请取消注释并补充实现。
- 当前模块依赖 `FITKCore` 和 `FITKInterface` 系列模块，这些模块必须在构建时正确链接。
- 在多算例场景下，应通过 `FITKRadiossCaseManager::setCurrentCaseID()` 确保当前操作的算例是正确的。

最后更新时间：2026-04-28