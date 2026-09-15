# FITKInterfaceFlowOF - OpenFOAM 流体仿真接口模块

## 简介

FITKInterfaceFlowOF 是 FastCAE 集成工具包（FITK）中负责 OpenFOAM 流体仿真参数配置的接口模块。它定义了求解器、边界条件、湍流模型、热物理属性、辐射模型等核心接口，为上层应用提供统一的流体仿真数据配置能力。

## 模块定位

FITKInterfaceFlowOF 位于 `FITK_Interface` 下，属于接口层模块，主要承担以下职责：

- 定义 OpenFOAM 求解器抽象接口
- 提供物理场数据管理能力
- 封装湍流模型、边界条件、热物理属性等配置接口
- 作为组件层与核心层之间的桥梁

## 架构与目录

```text
FITKInterfaceFlowOF
├── 核心接口
│   ├── FITKInterfaceFlowOFAPI          # API声明宏
│   ├── FITKAbstractOFSolver            # 求解器基类
│   ├── FITKAbstractOFTurbulence        # 湍流模型基类
│   └── FITKOFPhysicsManager            # 物理数据管理器(单例)
├── 求解器接口
│   ├── FITKOFSolverSIMPLE              # SIMPLE求解器
│   ├── FITKOFSolverCHTMultiRegion      # CHT多区域求解器
│   └── FITKOFSolverInter               # 求解器接口
├── 湍流模型
│   ├── FITKOFTurbKEpsilon              # k-epsilon模型
│   ├── FITKOFTurbKOmega                # k-omega模型
│   ├── FITKOFTurbKOmegaSST             # k-omega SST模型
│   ├── FITKOFTurbSpalartAllmaras       # S-A模型
│   └── FITKOFTurbSmagorinsky           # Smagorinsky模型
└── 辅助接口
    ├── FITKOFBoundary                  # 边界条件
    ├── FITKOFThermo                    # 热物理属性
    ├── FITKOFRadiation                 # 辐射模型
    ├── FITKOFTransportProp             # 传输属性
    ├── FITKOFSolution                  # 求解配置
    └── FITKOFInitialConditions         # 初始条件
```

## 依赖关系

### 编译依赖

- Qt5::Core
- FITKCore（基础对象、数据对象）

### 模块层级关系

- FITKInterfaceFlowOF 依赖 `FITKCore`
- 组件层（FITKOFDriver、FITKOFDictWriter）依赖此接口模块
- 提供统一的仿真参数配置接口

## 主要类与功能说明

### `FITKAbstractOFSolver`

求解器基类，继承自 `FITKAbstractNDataObject`。定义求解器的核心接口。

**主要方法：**
- `getSolverCommand()`：获取求解器运行命令（纯虚函数）
- `getSolverType()`：获取求解器类型（纯虚函数）
- `getRadiation()`：获取辐射模型参量
- `getThermo()`：获取热物理属性
- `getDiscretization()`：获取离散数据
- `getTransportProp()`：获取传输属性
- `getSolution()`：获取求解配置
- `getBoundary()`：获取边界条件
- `getInitialConditions()`：获取初始条件
- `getRunControl()`：获取运行配置
- `getOperatingConditions()`：获取工作条件

### `FITKOFPhysicsManager`

物理场数据管理器，单例模式。管理各类物理参数的配置。

**主要方法：**
- `getSolverManager()`：获取求解器管理器
- `getTurbulenceManager()`：获取湍流模型管理器
- `getSolutionSolverManager()`：获取求解配置管理器
- `getTransportModelManager()`：获取传输模型管理器
- `getBoundaryTypeManager()`：获取边界类型管理器
- `getRadiationParaManager()`：获取辐射参数管理器
- `getThermoPropManager()`：获取热物理属性管理器

### 湍流模型类

提供多种湍流模型接口：
- `FITKOFTurbKEpsilon`：k-epsilon 模型
- `FITKOFTurbKOmega`：k-omega 模型
- `FITKOFTurbKOmegaSST`：k-omega SST 模型（含DES、IDDES、SAS变体）
- `FITKOFTurbSpalartAllmaras`：Spalart-Allmaras 模型（含DES、DDES、IDDES变体）
- `FITKOFTurbRNGKEpsilon`：RNG k-epsilon 模型
- `FITKOFTurbSmagorinsky`：Smagorinsky LES 模型

### 辅助接口类

- `FITKOFBoundary`：边界条件接口
- `FITKOFThermo`：热物理属性接口
- `FITKOFRadiation`：辐射模型接口
- `FITKOFTransportProp`：传输属性接口
- `FITKOFSolution`：求解配置接口
- `FITKOFInitialConditions`：初始条件接口

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `add_library(FITKInterfaceFlowOF SHARED)`
- 启用 `AUTOMOC`
- 链接依赖：`FITKCore`

### 集成方式

作为接口层模块，通常通过以下方式接入应用：

1. 上层组件通过 `FITKOFPhysicsManager` 获取各类管理器
2. 通过 `FITKAbstractOFSolver` 派生类创建具体求解器
3. 配置边界条件、湍流模型、初始条件等参数

## 使用示例

### 1. 使用物理管理器

```cpp
#include "FITKOFPhysicsManager.h"

// 获取物理管理器实例
Interface::FITKOFPhysicsManager* physicsManager = 
    Interface::FITKOFPhysicsManager::getInstance();

// 获取求解器管理器
Interface::FITKOFSolverManager* solverManager = 
    physicsManager->getSolverManager();

// 获取湍流模型管理器
Interface::FITKOFTurbulenceManager* turbManager = 
    physicsManager->getTurbulenceManager();
```

### 2. 创建求解器

```cpp
#include "FITKOFSolverSIMPLE.h"

// 创建SIMPLE求解器
Interface::FITKOFSolverSIMPLE* solver = 
    new Interface::FITKOFSolverSIMPLE();

// 获取求解器命令
QString command = solver->getSolverCommand();

// 获取各类配置
Interface::FITKOFBoundary* boundary = 
    solver->getBoundary(Interface::FITKOFSolverTypeEnum::Velocity);
Interface::FITKOFInitialConditions* initCond = 
    solver->getInitialConditions();
```

### 3. 配置边界条件

```cpp
#include "FITKOFBoundary.h"

// 获取边界条件
Interface::FITKOFBoundary* boundary = solver->getBoundary(
    Interface::FITKOFSolverTypeEnum::Pressure);

// 设置边界参数
// boundary->setPressure(101325);  // 设置压力值
```

## 注意事项

- FITKInterfaceFlowOF 是接口层模块，不包含具体实现
- 具体求解器实现由组件层提供
- 物理参数配置需与 OpenFOAM 格式一致
- 使用前需确保求解器类型与物理模型匹配

最后更新时间：2026-04-30