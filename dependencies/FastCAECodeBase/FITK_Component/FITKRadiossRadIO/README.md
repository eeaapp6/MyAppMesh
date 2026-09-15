# FITKRadiossRadIO - Radioss RAD 文本读写组件

## 简介

FITKRadiossRadIO 是 FastCAE 集成工具包（FITK）中负责 Radioss RAD 文本格式文件读取与写入的核心组件。该模块通过组件接口对外提供统一的 RAD 导入与导出入口，内部由读取器和写出器执行具体文件操作，并通过适配器模式完成关键字级数据映射，实现 RAD 文件与内存数据对象之间的双向转换。

FITKRadiossRadIO 的设计目标是为上层应用程序提供完整的 Radioss 输入文件（.rad）读写能力，支持节点、部件、材料、属性、边界条件、相互作用、刚性墙、初始场、时程输出、探针、求解设置等全量数据域的解析与生成。

## 模块定位

FITKRadiossRadIO 位于 `FITK_Component` 下，属于 IO 组件层模块，主要职责包括：

- 定义 `FITKRadiossRadIoInterface` 组件接口，提供线程化读写执行能力。
- 实现 `FITKRadiossRadReader` 按关键字逐行解析 RAD 文件并分发到适配器。
- 实现 `FITKRadiossRadWriter` 将内存数据写出为符合 Radioss 格式的文本文件。
- 通过 `FITKRadiossAbstractAdaptor` 及其派生类建立关键字与数据对象的映射关系。
- 支持 `#include` 语句、0000/0001 双文件自动识别与拆分等 Radioss 特有机制。

## 架构与目录

从当前目录中的头文件和源文件看，FITKRadiossRadIO 可以概括为以下几组能力：

```text
FITKRadiossRadIO
├── 组件接口与读写器
│   ├── FITKRadiossRadIoInterface
│   ├── FITKRadiossRadReader
│   └── FITKRadiossRadWriter
├── 适配器基类与工具
│   ├── FITKRadiossAbstractAdaptor
│   ├── RadKeyLineSpliter
│   ├── RadReadInformationMapper
│   └── RadWriteInformationMapper
├── 材料适配器
│   ├── FITKRadiossAdaptorMaterialVoid
│   ├── FITKRadiossAdaptorMaterialElastic
│   ├── FITKRadiossAdaptorMaterialPlasJohns
│   ├── FITKRadiossAdaptorMaterialPlasBrit
│   ├── FITKRadiossAdaptorMaterialPlasTab
│   └── FITKRadiossAdaptorMaterialHydro
├── 属性适配器
│   ├── FITKRadiossAdaptorPropSolid / PropSolidValue
│   ├── FITKRadiossAdaptorPropShell / PropShellValue
│   ├── FITKRadiossAdaptorPropShellSandwich
│   ├── FITKRadiossAdaptorPropBeam / PropBeamSPR
│   └── FITKRadiossAdaptorPropSPH
├── 边界与物理适配器
│   ├── FITKRadiossAdaptorBCS / BCSCyclic / BCSLagMul
│   ├── FITKRadiossAdaptorRWallPlane / RWallCYL / RWallSpher
│   ├── FITKRadiossAdaptorGravity
│   ├── FITKRadiossAdaptorInitialFieldTra / InitialFieldAxis
│   └── FITKRadiossAdaptorInteractionTied / InteractionMUImpact
├── 求解配置适配器
│   ├── FITKRadiossAdaptorSolverSetting
│   └── FITKRadiossAdaptorSolverRequest
├── 后处理适配器
│   ├── FITKRadiossAdaptorTimerHistory
│   ├── FITKRadiossAdaptorProbeAccel / ProbeSection
│   └── FITKRadiossAdaptorBEMDAA
└── 辅助适配器
    ├── FITKRadiossAdaptorCase / Node / Part / Subset
    ├── FITKRadiossAdaptorNodeGroup / ElementGroup
    ├── FITKRadiossAdaptorNonLinerCurve
    ├── FITKRadiossAdaptorConnectionRBODY
    ├── FITKRadiossAdaptorEOSPolynomial
    ├── FITKRadiossAdaptorFailBIQUAD / FailJOHNSON
    └── FITKRadiossAdaptorSurfaceSeg / SurfaceOther
```

## 依赖关系

### 编译依赖

根据 `FITK_Component/FITKRadiossRadIO/CMakeLists.txt`，该模块当前显式依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- FITKAdaptor
- FITKInterfaceIO
- FITKInterfaceModel
- FITKRadiossData

### 模块层级关系

- FITKRadiossRadIO 依赖 `FITKCore` 提供的基础对象模型和工具类。
- FITKRadiossRadIO 依赖 `FITKAdaptor` 提供的 IO 适配器基类和工厂机制。
- FITKRadiossRadIO 依赖 `FITKInterfaceIO` 提供的文本读写抽象接口。
- FITKRadiossRadIO 依赖 `FITKRadiossData` 定义的数据模型，适配器负责将 RAD 文件内容映射到数据对象。
- 界面模块和操作器通过 `FITKRadiossRadIoInterface` 组件接口发起读写请求。

## 主要类与功能说明

### 组件接口

#### `Radioss::FITKRadiossRadIoInterface`

对外组件入口，继承 `AppFrame::FITKComponentInterface`。通过 `getComponentName()` 返回固定组件名 `FITKRadiossRadIO`。支持线程执行与同步执行两种模式，通过 `setFileName()` 设置文件路径，`setDataObject("RadiossCaseData", ...)` 绑定算例对象，`exec(1)` 执行读取，`exec(2)` 执行写出。读写完成后通过 `ioThreadFinishedSig()` 信号通知上层。

#### `Radioss::FITKRadiossRadReader`

RAD 文本读取器，继承 `Interface::FITKAbstractTextReader`。核心流程为 `loadFile()` 加载主文件与 include 文件、`run()` 按关键字循环调用 `readLine()` 读取并通过 `createAdaptor()` 创建匹配适配器执行 `adaptR()`。提供 `setSkipBlankLine()` 控制空行跳过、`clearSolverSettingRequest()` 在读取前清理求解旧参数。

#### `Radioss::FITKRadiossRadWriter`

RAD 文本写出器，继承 `Interface::FITKAbstractTextWriter`。`run()` 执行文件名校验后调用 `write0000File()` 写出算例主体数据到 `*_0000.rad`、`write0001File()` 写出求解设置与结果请求数据到 `*_0001.rad`。通过适配器执行 `adaptW()` 完成各数据域的序列化。

### 适配器机制

#### `Radioss::FITKRadiossAbstractAdaptor`

所有 RAD 关键字适配器的基类，继承 `Adaptor::FITKAbstractIOAdaptor`。提供 `setCaseData()` 绑定算例对象、`setFileReader()` / `setFileWriter()` 绑定读写器。通过 `static createAdaptor()` 和 `static registerAdaptor()` 实现按关键字动态创建适配器的工厂机制，使用 `RegRadiossRadAdaptor` 宏完成注册。

#### `Radioss::RadKeyLineSpliter`

RAD 关键字行分割器，负责解析以 `/` 开头的关键字行，提取关键字列表、ID 列表、参数列表等字段，供适配器读取使用。

#### `Radioss::RadReadInformationMapper`

读取信息映射表，负责在读取过程中维护 RAD 文件 ID 与内存对象 ID 的映射关系，包括材料 ID 映射、属性 ID 映射等，并在读取完成后调用 `map()` 进行信息整合。

### 材料与属性适配器

#### 材料适配器

- `FITKRadiossAdaptorMaterialVoid` - 空材料（`/MAT/LAW0`）
- `FITKRadiossAdaptorMaterialElastic` - 弹性材料（`/MAT/LAW1`、`/MAT/ELAST`）
- `FITKRadiossAdaptorMaterialPlasJohns` - Johnson-Cook 塑性材料
- `FITKRadiossAdaptorMaterialPlasBrit` - 塑性-断裂材料
- `FITKRadiossAdaptorMaterialPlasTab` - 表格式塑性材料
- `FITKRadiossAdaptorMaterialHydro` - 流体材料

#### 属性适配器

- `FITKRadiossAdaptorPropSolid` - 实体单元属性（`/PROP/TYPE14`）
- `FITKRadiossAdaptorPropShell` - 壳单元属性（`/PROP/TYPE1`）
- `FITKRadiossAdaptorPropShellSandwich` - 夹芯壳属性
- `FITKRadiossAdaptorPropBeam` - 梁单元属性（`/PROP/TYPE3`）
- `FITKRadiossAdaptorPropBeamSPR` - 梁-弹簧属性
- `FITKRadiossAdaptorPropSPH` - SPH 粒子属性

### 边界与物理适配器

#### `FITKRadiossAdaptorBCS` / `BCSCyclic` / `BCSLagMul`

边界条件适配器，分别对应标准边界条件、循环边界条件、拉格朗日乘数边界条件。负责解析 `/BCS` 关键字并映射到 `FITKRadiossBCS` 数据对象。

#### `FITKRadiossAdaptorRWallPlane` / `RWallCYL` / `RWallSpher`

刚性墙适配器，分别处理平面壁、圆柱壁、球面壁三种几何类型。负责 `/RW/` 关键字解析并映射到 `FITKRWall` 数据对象。

#### `FITKRadiossAdaptorGravity`

重力场适配器，负责解析 `/GRAV` 关键字并创建 `FITKGravity` 数据对象。

#### `FITKRadiossAdaptorInitialFieldTra` / `InitialFieldAxis`

初始速度场适配器，`InitialFieldTra` 对应 `/INIVEL/TRA` 翻译速度场，`InitialFieldAxis` 对应 `/INIVEL/AXIS` 旋转速度场。

#### `FITKRadiossAdaptorInteractionTied` / `InteractionMUImpact`

相互作用适配器，`InteractionTied` 处理绑定相互作用（`/INTER/TYPE2`），`InteractionMUImpact` 处理多用途冲击相互作用（`/INTER/TYPE7`）。

### 求解与后处理适配器

#### `FITKRadiossAdaptorSolverSetting`

求解设置适配器，负责解析 `/RUN` 等求解控制关键字，将运行时间、输出控制等参数映射到 `FITKRadiossSolverSettings` 数据对象。

#### `FITKRadiossAdaptorSolverRequest`

结果请求适配器，负责解析 `/ANIM`、`/TH` 等输出请求关键字，将动画输出、时程输出配置映射到 `FITKRadiossResultRequest` 数据对象。

#### `FITKRadiossAdaptorTimerHistory`

时程输出适配器，支持 `/TH/ACCEL`、`/TH/INTER`、`/TH/RWALL`、`/TH/RBODY`、`/TH/SECTIO`、`/TH/SPHCEL` 等多种时程类型。

#### `FITKRadiossAdaptorProbeAccel` / `ProbeSection`

探针适配器，`ProbeAccel` 对应加速度探针（`/PROBE/ACCEL`），`ProbeSection` 对应截面探针（`/PROBE/SECTION`）。

#### `FITKRadiossAdaptorBEMDAA`

BEM 双渐近近似适配器，负责解析 `/BEM/DAA` 关键字，处理边界元法中的双渐近近似计算配置。

### 辅助适配器

#### 网格与集合适配器

- `FITKRadiossAdaptorCase` - 算例级全局配置适配器
- `FITKRadiossAdaptorNode` - 节点坐标适配器（`/NODE`）
- `FITKRadiossAdaptorPart` - 部件定义适配器（`/PART`）
- `FITKRadiossAdaptorPartElements` - 部件单元关联适配器
- `FITKRadiossAdaptorSubset` - 子集定义适配器（`/SUBSET`）
- `FITKRadiossAdaptorNodeGroup` - 节点组适配器（`/GRNOD`）
- `FITKRadiossAdaptorElementGroup` - 单元组适配器（`/GRPART`）

#### 函数与曲线适配器

- `FITKRadiossAdaptorNonLinerCurve` - 非线性曲线适配器（`/FUNCT`）
- `FITKRadiossAdaptorEOSPolynomial` - 多项式状态方程适配器
- `FITKRadiossAdaptorFailBIQUAD` - BIQUAD 失效模型适配器
- `FITKRadiossAdaptorFailJOHNSON` - Johnson-Cook 失效模型适配器

#### 表面与连接适配器

- `FITKRadiossAdaptorSurfaceSeg` - 分段表面适配器（`/SURF`）
- `FITKRadiossAdaptorSurfaceOther` - 其他表面类型适配器
- `FITKRadiossAdaptorConnectionRBODY` - 刚体连接适配器（`/RBODY`）
- `FITKRadiossAdaptorGlobalSPH` - 全局 SPH 设置适配器

#### 相互作用默认值适配器

- `FITKRadiossAdaptorInteractionTiedDefault` - 绑定相互作用默认值
- `FITKRadiossAdaptorInteractionMUImpactDefault` - 冲击相互作用默认值

## 使用示例

### 示例1：在组件工厂中注册 RAD 读写组件

```cpp
#include "FITK_Component/FITKRadiossRadIO/FITKRadiossRadIoInterface.h"

QList<AppFrame::FITKComponentInterface*> ComponentFactory::createComponents()
{
    QList<AppFrame::FITKComponentInterface*> componentList;
    componentList << new Radioss::FITKRadiossRadIoInterface;
    return componentList;
}
```

### 示例2：导入 RAD 文件

```cpp
#include "FITK_Component/FITKRadiossRadIO/FITKRadiossRadIoInterface.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

bool importRadFile(const QString& fileName)
{
    auto io = FITKAPP->getComponents()->
        getComponentTByName<Radioss::FITKRadiossRadIoInterface>("FITKRadiossRadIO");
    if (io == nullptr) return false;

    Radioss::FITKRadiossCase* caseData =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (caseData == nullptr) return false;

    io->setFileName(fileName);
    io->setDataObject("RadiossCaseData", caseData);
    return io->exec(1);
}
```

### 示例3：导出 RAD 文件（同步模式）

```cpp
#include "FITK_Component/FITKRadiossRadIO/FITKRadiossRadIoInterface.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

bool exportRadFile(const QString& fileName, Radioss::FITKRadiossCase* caseData)
{
    if (caseData == nullptr) return false;

    auto io = FITKAPP->getComponents()->
        getComponentTByName<Radioss::FITKRadiossRadIoInterface>("FITKRadiossRadIO");
    if (io == nullptr) return false;

    io->runInThread(false);
    io->setFileName(fileName);
    io->setDataObject("RadiossCaseData", caseData);
    bool success = io->exec(2);
    return success && io->isSuccess();
}
```

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `project(FITKRadiossRadIO VERSION 1.0.0 LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`、`AUTOUIC`、`AUTORCC`
- 输出目录为 `output/bin_d` 和 `output/bin`
- 编译定义包含 `FITKRadiossRadIO_API`

### 在仓库中的参与方式

FITKRadiossRadIO 作为组件层 IO 模块被应用程序框架依赖。它通常与 `FITKRadiossData` 配合，完成 Radioss 输入文件（.rad）的解析与生成，并通过 `FITKRadiossRadIoInterface` 组件接口对外提供统一访问。

### 关键字映射与适配器注册

FITKRadiossRadIO 使用关键字映射机制将 RAD 文件中的关键字（如 `/MAT/LAW1`）与对应的适配器类关联。适配器注册通过 `RegRadiossRadAdaptor` 宏完成：

```cpp
RegRadiossRadAdaptor(/MAT/LAW1, FITKRadiossAdaptorMaterialElastic);
RegRadiossRadAdaptor(/MAT/ELAST, FITKRadiossAdaptorMaterialElastic, 1);
```

同一个关键字可以注册多个适配器变体（通过可变参数区分），`createAdaptor()` 会根据关键字创建对应的适配器实例。

### RAD 文件格式特点

Radioss RAD 文件具有以下特点，FITKRadiossRadIO 对此提供了专门支持：

1. **双文件结构**：主文件 `*_0000.rad` 包含算例主体数据，求解文件 `*_0001.rad` 包含求解设置与输出请求。
2. **`#include` 指令**：支持通过 `#include` 引用其他 RAD 文件，读取器会自动展开。
3. **关键字驱动**：所有数据块以 `/` 开头的关键字标识，如 `/MAT/LAW1`、`/PROP/TYPE1`。
4. **自由格式**：参数通过固定宽度字段排列，无需严格对齐但必须按顺序解析。

### 写出流程详解

`FITKRadiossRadWriter` 的写出流程如下：

1. `run()` 校验文件名是否符合 Radioss 规范。
2. 调用 `write0000File()` 遍历算例数据对象，通过各适配器的 `adaptW()` 写出算例主体。
3. 调用 `write0001File()` 写出求解设置与结果请求数据。
4. `RadWriteInformationMapper` 提供写出时的 ID 映射辅助。

### 调试与日志

适配器执行过程中，读取器和写出器通过 `consoleMessage()` 输出日志。日志级别分为：

- 级别 1（Normal）：正常信息，如读取进度、写出状态。
- 级别 2（Warning）：警告信息，如缺失可选参数、ID 映射异常。
- 级别 3（Error）：错误信息，如关键字不匹配、解析失败。

上层可通过重写 `consoleMessage()` 或拦截 `FITKSignalTransfer` 的消息信号来收集日志。

## 注意事项

- FITKRadiossRadIO 定义的是 RAD 文件读写机制，具体的数据定义由 `FITKRadiossData` 模块提供。
- 当前模块依赖 `FITKAdaptor` 的 IO 适配器框架，注册宏 `RegRadiossRadAdaptor` 与 `Register2FITKIOAdaptorFactory` 需要配合使用。
- 读取前 `FITKRadiossRadReader::clearSolverSettingRequest()` 会清理求解设置旧参数，确保读取结果不受干扰。
- 在多文件场景下，应确保 `*_0000.rad` 和 `*_0001.rad` 同时存在于同一目录，或通过 `#include` 正确引用。

最后更新时间：2026-04-28