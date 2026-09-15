# FITKRadiossDataIOHDF5 - Radioss 数据 HDF5 持久化组件

## 简介

FITKRadiossDataIOHDF5 是 FastCAE 集成工具包（FITK）中负责 Radioss 算例数据 HDF5 格式持久化的核心组件。该模块通过组件接口对外提供统一的 HDF5 导入与导出入口，内部由 HDF5 读取器和写出器执行具体文件操作，并通过适配器模式完成对象字段与 HDF5 节点的双向映射，实现 Radioss 内存数据与 HDF5 文件之间的双向转换。

FITKRadiossDataIOHDF5 的设计目标是为上层应用程序提供工程文件中的 Radioss 数据持久化能力，支持几何部件、网格节点、部件、集合、子集、表面、材料、属性、边界条件、相互作用、刚性墙、重力场、求解设置、结果请求、时程输出、探针、连接、失效模型等全量数据域的序列化与反序列化。

## 模块定位

FITKRadiossDataIOHDF5 位于 `FITK_Component` 下，属于数据持久化组件层模块，主要职责包括：

- 定义 `FITKRadiossDataHDF5Interface` 组件接口，提供线程化 HDF5 读写执行能力。
- 实现 `FITKRadiossDataHDF5Reader` 按 HDF5 分组结构读取数据并分发到适配器。
- 实现 `FITKRadiossDataHDF5Writer` 将内存数据映射到 HDF5 分组结构并写出。
- 通过 `FITKRadiossDataHDF5Adaptor` 及其派生类建立数据对象与 HDF5 节点的映射关系。
- 支持版本信息维护、HDF5 文件流共享、多线程执行等工程应用场景。

## 架构与目录

从当前目录中的头文件和源文件看，FITKRadiossDataIOHDF5 可以概括为以下几组能力：

```text
FITKRadiossDataIOHDF5
├── 组件接口与读写器
│   ├── FITKRadiossDataHDF5Interface
│   ├── FITKRadiossDataHDF5Reader
│   └── FITKRadiossDataHDF5Writer
├── 适配器基类
│   └── FITKRadiossDataHDF5Adaptor
├── 算例与网格适配器
│   ├── FITKRadiossDataCaseAdaptor
│   ├── FITKRadiossDataGeomPartAdaptor
│   ├── FITKRadiossDataNodeAdaptor
│   ├── FITKRadiossDataPartAdaptor
│   ├── FITKRadiossDataSetAdaptor
│   ├── FITKRadiossDataSubSetAdaptor
│   └── FITKRadiossDataSurfaceAdaptor
├── 材料适配器
│   ├── FITKRadiossDataMaterialVoidAdaptor
│   ├── FITKRadiossDataMaterialElasticAdaptor
│   ├── FITKRadiossDataMaterialPlasJohnsAdaptor
│   ├── FITKRadiossDataMaterialPlasBritAdaptor
│   ├── FITKRadiossDataMaterialPlasTabAdaptor
│   ├── FITKRadiossDataMaterialHydroAdaptor
│   └── FITKRadiossDataMaterialFailBiquadAdaptor
├── 属性适配器
│   ├── FITKRadiossDataPropSolidAdaptor
│   ├── FITKRadiossDataPropShellAdaptor
│   ├── FITKRadiossDataPropSandwichShellAdaptor
│   ├── FITKRadiossDataPropBeamAdaptor
│   ├── FITKRadiossDataPropBeamSPRAdaptor
│   └── FITKRadiossDataPropSPHAdaptor
├── 边界与物理适配器
│   ├── FITKRadiossDataBCSAdaptor
│   ├── FITKRadiossDataRWallPlaneAdaptor
│   ├── FITKRadiossDataRWallCylinderAdaptor
│   ├── FITKRadiossDataGravityAdaptor
│   ├── FITKRadiossDataInitialFieldTraAdaptor
│   ├── FITKRadiossDataInteractionTiedAdaptor
│   └── FITKRadiossDataInteractionMultiUsageImpactAdaptor
├── 求解配置适配器
│   ├── FITKRadiossDataSolverSettingsAdaptor
│   ├── FITKRadiossDataResultRequestAdaptor
│   └── FITKRadiossDataSolutionAdaptor
├── 后处理适配器
│   ├── FITKRadiossDataTimeHistoryACCELAdaptor
│   ├── FITKRadiossDataTimeHistoryINTERAdaptor
│   ├── FITKRadiossDataTimeHistoryRBODYAdaptor
│   ├── FITKRadiossDataTimeHistoryRWALLAdaptor
│   ├── FITKRadiossDataTimeHistorySECTIOAdaptor
│   ├── FITKRadiossDataTimeHistorySPHCELAdaptor
│   ├── FITKRadiossDataProbeAccelerationAdaptor
│   └── FITKRadiossDataProbeSectionAdaptor
└── 辅助适配器
    ├── FITKRadiossDataNonLinerCurveAdaptor
    ├── FITKRadiossDataConnectionRBODYAdaptor
    ├── FITKRadiossEquationOfStatePolynomialAdaptor
    ├── FITKRadiossDataFailureModelBIQUADAdaptor
    └── FITKRadiossDataFailureModelJOHNSONAdaptor
```

## 依赖关系

### 编译依赖

根据 `FITK_Component/FITKRadiossDataIOHDF5/CMakeLists.txt`，该模块当前显式依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- HDF5
- FITKCore
- FITKAdaptor
- FITKInterfaceIO
- FITKRadiossData

### 模块层级关系

- FITKRadiossDataIOHDF5 依赖 `FITKCore` 提供的基础对象模型和工具类。
- FITKRadiossDataIOHDF5 依赖 `FITKAdaptor` 提供的 IO 适配器基类和工厂机制。
- FITKRadiossDataIOHDF5 依赖 `FITKInterfaceIO` 提供的 HDF5 读写抽象接口。
- FITKRadiossDataIOHDF5 依赖 `FITKRadiossData` 定义的数据模型，适配器负责将 HDF5 数据映射到内存对象。
- 界面模块和操作器通过 `FITKRadiossDataHDF5Interface` 组件接口发起 HDF5 读写请求。

## 主要类与功能说明

### 组件接口

#### `IO::FITKRadiossDataHDF5Interface`

对外组件入口，继承 `AppFrame::FITKComponentInterface`。通过 `getComponentName()` 返回固定组件名 `FITKRadiossDataHDF5IO`。支持线程执行与同步执行两种模式，通过 `setFileName()` 设置文件路径，`setHDF5Root()` 注入外部 HDF5 文件流指针，`setDataObject()` 绑定算例对象，`exec(1)` 执行读取，`exec(2)` 执行写出。读写完成后通过 `ioThreadFinishedSig()` 信号通知上层。

#### `IO::FITKRadiossDataHDF5Reader`

HDF5 读取执行器，继承 `FITKAbstractHDF5Reader`。`run()` 首先调用 `readVersion()` 校验版本信息，然后调用 `readRadiossDatas()` 读取 `Radioss` 分组数据并分发给适配器。

#### `IO::FITKRadiossDataHDF5Writer`

HDF5 写出执行器，继承 `FITKAbstractHDF5Writer`。`run()` 首先调用 `writeVersion()` 写出版本节点，然后调用 `writeRadiossDatas()` 遍历算例数据并通过适配器写出到 HDF5 分组。

### 适配器机制

#### `Interface::FITKRadiossDataHDF5Adaptor`

所有 Radioss 数据 HDF5 适配器的基类，继承 `Adaptor::FITKAbstractIOAdaptor`。提供 `setReader()` / `setWriter()` 绑定读写器上下文、`setH5Group()` 设置当前 HDF5 分组节点、`readDataObject()` / `writeDataObject()` 处理基础对象序列化、`readRadiossData()` / `writeRadiossData()` 处理 Radioss 特定数据序列化。派生适配器通过 `Register2FITKIOAdaptorFactory(HDF5, ...)` 注册到 IO 适配器工厂。

#### `Interface::FITKRadiossDataCaseAdaptor`

算例级主适配器，负责协调各子适配器完成算例数据的完整读写。内部调用 `readGeomPart()` / `writeGeomPart()` 处理几何部件、`readPart()` / `writePart()` 处理部件、`readNode()` / `writeNode()` 处理节点、`readNodeSet()` / `writeNodeSet()` 处理节点集合、`readSubSet()` / `writeSubSet()` 处理子集、`readSurface()` / `writeSurface()` 处理表面、`readMaterials()` / `writeMaterials()` 处理材料、`readProperty()` / `writeProperty()` 处理属性、`readCurve()` / `writeCurve()` 处理非线性曲线、`readFailureModel()` / `writeFailureModel()` 处理失效模型、`readEquationOfState()` / `writeEquationOfState()` 处理状态方程、`readSolution()` / `writeSolution()` 处理求解数据。

### 材料与属性适配器

#### 材料适配器

- `FITKRadiossDataMaterialVoidAdaptor` - 空材料
- `FITKRadiossDataMaterialElasticAdaptor` - 弹性材料
- `FITKRadiossDataMaterialPlasJohnsAdaptor` - Johnson-Cook 塑性材料
- `FITKRadiossDataMaterialPlasBritAdaptor` - 塑性-断裂材料
- `FITKRadiossDataMaterialPlasTabAdaptor` - 表格式塑性材料
- `FITKRadiossDataMaterialHydroAdaptor` - 流体材料
- `FITKRadiossDataMaterialFailBiquadAdaptor` - 材料级 BIQUAD 失效模型

#### 属性适配器

- `FITKRadiossDataPropSolidAdaptor` - 实体单元属性
- `FITKRadiossDataPropShellAdaptor` - 壳单元属性
- `FITKRadiossDataPropSandwichShellAdaptor` - 夹芯壳属性
- `FITKRadiossDataPropBeamAdaptor` - 梁单元属性
- `FITKRadiossDataPropBeamSPRAdaptor` - 梁-弹簧属性
- `FITKRadiossDataPropSPHAdaptor` - SPH 粒子属性

### 边界与物理适配器

- `FITKRadiossDataBCSAdaptor` - 边界条件适配器
- `FITKRadiossDataRWallPlaneAdaptor` - 平面刚性墙适配器
- `FITKRadiossDataRWallCylinderAdaptor` - 圆柱刚性墙适配器
- `FITKRadiossDataGravityAdaptor` - 重力场适配器
- `FITKRadiossDataInitialFieldTraAdaptor` - 初始速度场适配器
- `FITKRadiossDataInteractionTiedAdaptor` - 绑定相互作用适配器
- `FITKRadiossDataInteractionMultiUsageImpactAdaptor` - 多用途冲击相互作用适配器

### 求解与后处理适配器

#### 求解配置适配器

- `FITKRadiossDataSolverSettingsAdaptor` - 求解设置适配器
- `FITKRadiossDataResultRequestAdaptor` - 结果请求适配器
- `FITKRadiossDataSolutionAdaptor` - 求解过程适配器

#### 时程与探针适配器

- `FITKRadiossDataTimeHistoryACCELAdaptor` - 加速度时程适配器
- `FITKRadiossDataTimeHistoryINTERAdaptor` - 相互作用时程适配器
- `FITKRadiossDataTimeHistoryRBODYAdaptor` - 刚体时程适配器
- `FITKRadiossDataTimeHistoryRWALLAdaptor` - 刚性墙时程适配器
- `FITKRadiossDataTimeHistorySECTIOAdaptor` - 截面时程适配器
- `FITKRadiossDataTimeHistorySPHCELAdaptor` - SPH 单元时程适配器
- `FITKRadiossDataProbeAccelerationAdaptor` - 加速度探针适配器
- `FITKRadiossDataProbeSectionAdaptor` - 截面探针适配器

### 辅助适配器

- `FITKRadiossDataNonLinerCurveAdaptor` - 非线性曲线适配器
- `FITKRadiossDataConnectionRBODYAdaptor` - 刚体连接适配器
- `FITKRadiossEquationOfStatePolynomialAdaptor` - 多项式状态方程适配器
- `FITKRadiossDataFailureModelBIQUADAdaptor` - BIQUAD 失效模型适配器
- `FITKRadiossDataFailureModelJOHNSONAdaptor` - Johnson-Cook 失效模型适配器

## 使用示例

### 示例1：在组件工厂中注册 Radioss HDF5 组件

```cpp
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataHDF5Interface.h"

QList<AppFrame::FITKComponentInterface*> ComponentFactory::createComponents()
{
    QList<AppFrame::FITKComponentInterface*> componentList;
    componentList << new IO::FITKRadiossDataHDF5Interface;
    return componentList;
}
```

### 示例2：打开工程时读取 Radioss 数据

```cpp
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataHDF5Interface.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

bool readRadiossCaseFromProject(const QString& fileName)
{
    Radioss::FITKRadiossCase* caseObj =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    auto radiossIO =
        FITKAPP->getComponents()->getComponentTByName<IO::FITKRadiossDataHDF5Interface>("FITKRadiossDataHDF5IO");
    if (caseObj == nullptr || radiossIO == nullptr) return false;

    radiossIO->setFileName(fileName);
    radiossIO->setDataObject(caseObj);
    radiossIO->setRunInThread(false);
    return radiossIO->exec(1) && radiossIO->isSuccess();
}
```

### 示例3：保存工程时写出 Radioss 数据（同步模式）

```cpp
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataHDF5Interface.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

bool saveRadiossCaseToProject(const QString& fileName)
{
    Radioss::FITKRadiossCase* caseObj =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (caseObj == nullptr) return false;

    auto radiossIO =
        FITKAPP->getComponents()->getComponentTByName<IO::FITKRadiossDataHDF5Interface>("FITKRadiossDataHDF5IO");
    if (radiossIO == nullptr) return false;

    radiossIO->setFileName(fileName);
    radiossIO->setDataObject(caseObj);
    radiossIO->setRunInThread(false);
    bool success = radiossIO->exec(2);
    return success && radiossIO->isSuccess();
}
```

## HDF5 数据结构

FITKRadiossDataIOHDF5 在 HDF5 文件中维护以下分组结构：

```text
HDF5 File
└── Radioss/
    ├── Version              # 版本信息
    ├── GeomPart/            # 几何部件
    ├── Part/                # 部件定义
    ├── Node/                # 节点数据
    ├── Set/                 # 集合数据
    ├── SubSet/              # 子集数据
    ├── Surface/             # 表面数据
    ├── Material/            # 材料数据
    │   ├── Elastic/
    │   ├── PlasJohns/
    │   └── ...
    ├── Property/            # 属性数据
    │   ├── Shell/
    │   ├── Solid/
    │   └── ...
    ├── BCS/                 # 边界条件
    ├── RWall/               # 刚性墙
    ├── Gravity/             # 重力场
    ├── InitialField/        # 初始场
    ├── Interaction/         # 相互作用
    ├── Curve/               # 非线性曲线
    ├── SolverSettings/      # 求解设置
    ├── ResultRequest/       # 结果请求
    ├── TimeHistory/         # 时程输出
    ├── Probe/               # 探针
    ├── Connection/          # 连接
    ├── FailureModel/        # 失效模型
    └── EquationOfState/     # 状态方程
```

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `project(FITKRadiossDataIOHDF5 VERSION 1.0.0 LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`、`AUTOUIC`、`AUTORCC`
- 输出目录为 `output/bin_d` 和 `output/bin`
- 编译定义包含 `FITKRadiossDataIOHDF5_API`

### HDF5 依赖配置

当前目录中的 `HDF5Config.cmake` 提供了 HDF5 库的查找与配置支持。构建时需要确保 HDF5 开发库已正确安装并可通过 `find_package(HDF5 REQUIRED)` 找到。

### 在仓库中的参与方式

FITKRadiossDataIOHDF5 作为数据持久化组件被应用程序框架依赖。它通常与几何 HDF5 IO 共用同一 HDF5 文件流，完成工程文件中 Radioss 数据的整体持久化。

## 注意事项

- FITKRadiossDataIOHDF5 定义的是 HDF5 持久化机制，具体的数据定义由 `FITKRadiossData` 模块提供。
- 当前模块依赖 `FITKAdaptor` 的 IO 适配器框架，注册宏 `Register2FITKIOAdaptorFactory(HDF5, ...)` 需要与 `FITKAbstractIOAdaptor` 基类配合使用。
- 版本信息（`Version` 节点）用于 HDF5 文件格式的向前兼容，读取时会校验版本兼容性。
- 在工程文件场景下，Radioss 数据通常与几何数据存储在同一 HDF5 文件的不同分组中，应通过 `setHDF5Root()` 共用文件流而非分别打开。

最后更新时间：2026-04-28