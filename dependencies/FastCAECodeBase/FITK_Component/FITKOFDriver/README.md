# FITKOFDriver - OpenFOAM 程序驱动组件

## 简介

FITKOFDriver 是 FastCAE 集成工具包（FITK）中负责 OpenFOAM 程序执行与管理的功能组件。它封装了 OpenFOAM 各类工具的调用接口，包括网格生成工具（blockMesh、snappyHexMesh）、求解器执行工具以及后处理工具，提供统一的程序驱动管理能力。

该模块解决的核心问题是：如何在 FastCAE 框架中统一管理 OpenFOAM 程序的启动、监控和终止，实现仿真流程的自动化执行。

## 模块定位

FITKOFDriver 位于 `FITK_Component` 下，属于功能组件层，主要承担以下职责：

- 提供 OpenFOAM 程序的统一驱动接口
- 封装网格生成工具（blockMesh、snappyHexMesh、createPatch）
- 封装求解器执行工具（foamRun、foamMultiRun）
- 封装并行计算工具（decomposePar、reconstructPar）
- 封装后处理工具（foamToVTK）
- 管理程序执行状态和输出监控

## 架构与目录

从当前目录中的头文件和源文件看，FITKOFDriver 可以概括为以下几组能力：

```text
FITKOFDriver
├── 核心接口
│   ├── FITKOFDriverAPI          # API声明宏
│   ├── FITKOFInterface          # 组件注册接口
│   └── FITKOFInputInfo          # 输入参数类
├── 驱动基类
│   └── FITKOpenFoamMeshingDriverBase  # OpenFOAM网格划分父类
└── 具体驱动类
    ├── FITKOFBlockMeshDriver           # blockMesh驱动
    ├── FITKOFSnappyHexMeshDriver       # snappyHexMesh驱动
    ├── FITKOFSnappyHexMeshParallelDriver # 并行snappyHexMesh驱动
    ├── FITKOFCreatePatchDriver         # createPatch驱动
    ├── FITKOFFoamRunDriver             # foamRun求解器驱动
    ├── FITKOFFoamMultiRunDriver        # foamMultiRun求解器驱动
    ├── FITKOFFoamDictionaryDriver      # 字典操作驱动
    ├── FITKOFDecomposeParDriver        # decomposePar驱动
    ├── FITKOFReconstructParDriver      # reconstructPar驱动
    └── FITKOFFoamToVTKDriver           # foamToVTK驱动
```

## 依赖关系

### 编译依赖

根据模块源码中的头文件引用，该模块当前显式依赖：

- Qt5::Core
- FITKCore（基础对象、数据对象）
- FITKAppFramework（程序驱动框架、命令执行器）

### 模块层级关系

- FITKOFDriver 依赖 `FITKCore` 和 `FITKAppFramework`
- 上层流体仿真模块通过该组件启动和管理 OpenFOAM 程序
- 该组件与 `FITKMeshGenOF`、`FITKOFDictWriter` 配合完成完整的仿真流程

## 主要类与功能说明

### 核心接口类

#### `FITKOFInterface`

OpenFOAM 驱动组件的对外接口，继承自 `FITKComponentInterface`。作为组件注册和初始化的入口。

**主要方法：**
- `init()`：初始化组件
- `getComponentName()`：返回组件名称

**成员变量：**
- `_processId`：进程ID
- `_commandRunner`：终端命令执行器

#### `FITKOFInputInfo`

OpenFOAM 程序输入参数类，继承自 `FITKProgramInputInfo`。管理程序启动参数和监控配置。

**主要方法：**
- `setArgs(args)`：设置启动参数
- `args()`：获取启动参数
- `setMonitorPath(path)`：设置监控目录
- `getMonitorPath()`：获取监控目录
- `setMonitorFiles(files)`：设置监控文件
- `getMonitorFiles()`：获取监控文件

### 驱动基类

#### `FITKOpenFoamMeshingDriverBase`

OpenFOAM 网格划分驱动基类，继承自 `FITKLinuxShellDriver`。提供网格划分驱动的通用功能。

**槽函数：**
- `threadFinishedSlot()`：线程执行结束处理

### 具体驱动类

#### `FITKOFBlockMeshDriver`

blockMesh 网格划分驱动类，继承自 `FITKOpenFoamMeshingDriverBase`。

**主要方法：**
- `getProgramType()`：返回程序类型标识
- `getProgramName()`：返回 "blockMesh"
- `start()`：启动 blockMesh 程序
- `stop()`：终止程序

#### `FITKOFSnappyHexMeshDriver`

snappyHexMesh 网格划分驱动类，继承自 `FITKOpenFoamMeshingDriverBase`。

**主要方法：**
- `getProgramType()`：返回程序类型标识
- `getProgramName()`：返回 "SnappyHexMesh"
- `start()`：启动 snappyHexMesh 程序
- `stop()`：终止程序

#### `FITKOFSnappyHexMeshParallelDriver`

并行 snappyHexMesh 驱动类，用于并行网格生成。

#### `FITKOFCreatePatchDriver`

createPatch 驱动类，用于边界补丁创建。

#### `FITKOFFoamRunDriver`

foamRun 求解器驱动类，继承自 `FITKOpenFoamMeshingDriverBase`。

**主要方法：**
- `getProgramType()`：返回程序类型标识
- `getProgramName()`：返回程序名称
- `start()`：启动求解器
- `stop()`：终止求解器

#### `FITKOFFoamMultiRunDriver`

foamMultiRun 求解器驱动类，支持多算例并行求解。

#### `FITKOFFoamDictionaryDriver`

字典操作驱动类，用于字典文件的修改和操作。

#### `FITKOFDecomposeParDriver`

decomposePar 驱动类，用于并行计算的域分解。

#### `FITKOFReconstructParDriver`

reconstructPar 驱动类，用于并行计算结果的重构。

#### `FITKOFFoamToVTKDriver`

foamToVTK 驱动类，用于将 OpenFOAM 结果转换为 VTK 格式。

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `add_library(FITKOFDriver SHARED)`
- 启用 `AUTOMOC`
- 链接依赖：`FITKCore`、`FITKAppFramework`

### 在上层工程中的参与方式

FITKOFDriver 作为组件层模块，通常通过以下方式接入应用：

1. 通过组件工厂注册到应用框架
2. 在仿真流程中根据需要创建对应的驱动实例
3. 设置输入参数并启动程序
4. 监控程序执行状态和输出

## 使用示例

### 1. 使用 blockMesh 驱动

```cpp
#include "FITKOFBlockMeshDriver.h"
#include "FITKOFInputInfo.h"

// 创建 blockMesh 驱动
FoamDriver::FITKOFBlockMeshDriver* driver = 
    new FoamDriver::FITKOFBlockMeshDriver();

// 设置工作目录
driver->setWorkDir("/path/to/case");

// 创建输入参数
FoamDriver::FITKOFInputInfo* inputInfo = new FoamDriver::FITKOFInputInfo();
QStringList args;
args << "-dict" << "system/blockMeshDict";
inputInfo->setArgs(args);

// 设置监控文件
QStringList monitorFiles;
monitorFiles << "log.blockMesh";
inputInfo->setMonitorFiles(monitorFiles);

// 启动程序
driver->start();

// 释放资源（实际应用中应在程序结束后释放）
// delete inputInfo;
// delete driver;
```

### 2. 使用求解器驱动

```cpp
#include "FITKOFFoamRunDriver.h"

// 创建求解器驱动
FoamDriver::FITKOFFoamRunDriver* driver = 
    new FoamDriver::FITKOFFoamRunDriver();

// 设置工作目录
driver->setWorkDir("/path/to/case");

// 启动求解器
driver->start();

// 释放资源
// delete driver;
```

### 3. 使用并行计算驱动

```cpp
#include "FITKOFDecomposeParDriver.h"
#include "FITKOFFoamRunDriver.h"
#include "FITKOFReconstructParDriver.h"

// 第一步：域分解
FoamDriver::FITKOFDecomposeParDriver* decomposeDriver = 
    new FoamDriver::FITKOFDecomposeParDriver();
decomposeDriver->setWorkDir("/path/to/case");
decomposeDriver->start();

// 第二步：并行求解
FoamDriver::FITKOFFoamRunDriver* runDriver = 
    new FoamDriver::FITKOFFoamRunDriver();
runDriver->setWorkDir("/path/to/case");
runDriver->start();

// 第三步：结果重构
FoamDriver::FITKOFReconstructParDriver* reconstructDriver = 
    new FoamDriver::FITKOFReconstructParDriver();
reconstructDriver->setWorkDir("/path/to/case");
reconstructDriver->start();

// 释放资源
// delete decomposeDriver;
// delete runDriver;
// delete reconstructDriver;
```

### 4. 使用组件接口

```cpp
#include "FITKOFInterface.h"

// 获取组件实例
FoamDriver::FITKOFInterface* interface = new FoamDriver::FITKOFInterface();

// 初始化组件
interface->init();

// 获取组件名称
QString name = interface->getComponentName();
qDebug() << "Component Name:" << name;

// 释放资源
delete interface;
```

## 注意事项

- FITKOFDriver 依赖 OpenFOAM 环境的正确配置
- 程序执行前需确保工作目录和输入文件存在
- 并行计算需要正确配置 decomposeParDict
- 程序执行可能耗时较长，建议在后台线程中执行
- 需正确处理程序执行异常和错误输出

最后更新时间：2026-04-30