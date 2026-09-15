# OperatorsModel - 操作器模型模块

## 简介

OperatorsModel 是 FastCAE 集成工具包（FITK）的操作器模型模块，提供各种操作器的业务逻辑实现，包括新建、打开、保存、运行、后处理等核心操作。

## 模块定位

OperatorsModel 位于项目根目录，是操作器模块的核心层，主要承担以下职责：

- 提供文件操作（新建、打开、保存）
- 管理网格生成和处理
- 配置边界条件和求解设置
- 控制求解器运行和后处理
- 管理工作目录和许可证

## 架构与目录

```text
OperatorsModel/
├── OperatorsModelAPI.h         # API声明
├── OpersModelInterface.h       # 模型接口
├── OperManagerBase.h           # 操作器管理器基类
├── OperatorsNew.h/cpp          # 新建操作
├── OperatorsOpen.h/cpp         # 打开操作
├── OperatorsSave.h/cpp         # 保存操作
├── OperatorsSetup.h/cpp        # 设置操作
├── OperatorsRun.h/cpp          # 运行操作
├── OperatorsPost.h/cpp         # 后处理操作
├── OperatorsInitial.h          # 初始条件
├── OperatorsBoundaryConditions.h # 边界条件
├── OperatorsMeshBaseManager.h  # 网格基础管理
├── OperatorsMeshGeoManager.h   # 几何网格管理
├── OperatorsMeshPointManager.h # 点网格管理
├── OperatorsMeshLocalManager.h # 局部网格管理
├── OperatorsMeshManager.h      # 网格管理器
├── OperatorsRegionMesh.h       # 区域网格
├── OperatorsGeoBool.h          # 几何布尔操作
├── OperatorsGeoCubeManager.h   # 立方体管理器
├── OperatorsGeoCylinderManager.h # 圆柱体管理器
├── OperatorsGeoSphereManager.h # 球体管理器
├── OperatorsImportManager.h    # 导入管理器
├── OperatorsSolution.h         # 求解设置
├── OperatorsTurbulence.h       # 湍流模型
├── OperatorsTransport.h        # 传输模型
├── OperatorsThermo.h           # 热物理属性
├── OperatorsRadiation.h        # 辐射模型
├── OperatorsOperCondition.h    # 操作条件
├── OperatorsDiscretization.h   # 离散设置
├── OperatorsCFDPost.h          # CFD后处理
├── OperatorsWorkingDir.h       # 工作目录
└── OperatorsLicense.h          # 许可证管理
```

## 依赖关系

### 编译依赖

- Qt5::Core
- FITKCore（核心对象）
- FITKInterfaceFlowOF（流体接口）

## 主要类与功能说明

### `OperatorsNew`

新建操作类，负责创建新的项目文件。

### `OperatorsOpen`

打开操作类，负责打开已有的项目文件。

### `OperatorsSave`

保存操作类，负责保存项目文件。

### `OperatorsRun`

运行操作类，负责启动求解器并监控运行状态。

### `OperatorsPost`

后处理操作类，负责处理和显示仿真结果。

### `OperatorsMeshBaseManager`

网格基础管理器，提供网格生成的基础功能。

### `OperatorsBoundaryConditions`

边界条件管理类，负责设置和管理边界条件。

### `OperatorsSolution`

求解设置类，负责配置求解器参数。

## 使用示例

创建并运行操作：

```cpp
#include "OperatorsNew.h"
#include "OperatorsRun.h"

// 创建新项目
Operators::OperatorsNew* newOp = new Operators::OperatorsNew();
newOp->createNewProject("MyProject");

// 配置边界条件
Operators::OperatorsBoundaryConditions* bcOp = 
    new Operators::OperatorsBoundaryConditions();
bcOp->setVelocityBoundary("inlet", 10.0);

// 运行求解
Operators::OperatorsRun* runOp = new Operators::OperatorsRun();
runOp->startSolver();
```

## 注意事项

- OperatorsModel 包含业务逻辑，不直接处理界面
- 操作器通过接口与 GUI 层通信
- 网格管理依赖几何和网格模块
- 求解设置需要与 OpenFOAM 接口配合

最后更新时间：2026-04-30