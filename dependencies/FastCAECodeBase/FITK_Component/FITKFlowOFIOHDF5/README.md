# FITKFlowOFIOHDF5 - OpenFOAM HDF5文件读写组件

## 简介

FITKFlowOFIOHDF5 是 FastCAE 集成工具包（FITK）中负责 OpenFOAM 求解器 HDF5 文件读写的组件模块。它基于 `FITKAbstractHDF5Reader` 和 `FITKAbstractHDF5Writer` 接口，提供了完整的几何数据、网格数据、物理属性、边界条件、初始条件等的 HDF5 格式读写能力。

该模块解决的核心问题是：如何将 OpenFOAM 求解器所需的各类仿真数据（几何、网格、物理模型、求解控制等）以高效、结构化的方式存储到 HDF5 文件中，并能够完整地从 HDF5 文件中恢复这些数据。

## 模块定位

FITKFlowOFIOHDF5 位于 `FITK_Component` 下，属于功能组件层，主要承担以下职责：

- 提供 HDF5 格式文件的读写接口，支持 OpenFOAM 仿真数据的持久化存储
- 通过适配器模式实现各类数据对象（几何、网格、物理等）与 HDF5 数据结构的转换
- 支持线程化读写操作，提升大文件处理性能
- 作为 FITKFlowOF（OpenFOAM 流程组件）的数据持久化支撑模块

## 架构与目录

从当前目录中的头文件和源文件看，FITKFlowOFIOHDF5 可以概括为以下几组能力：

```text
FITKFlowOFIOHDF5
├── 核心接口
│   ├── FITKFlowOFIOHDF5Interface    # HDF5文件读写组件接口
│   └── FITKFlowOFIOHDF5API          # API声明宏
├── 读写器
│   ├── FITKFlowOFHDF5Reader         # HDF5文件读取器
│   └── FITKFlowOFHDF5Writer         # HDF5文件写入器
├── 抽象适配器
│   └── FITKFlowOFHDF5AbstractAdaptor # 抽象HDF5读写适配器基类
└── 具体适配器
    ├── FITKFlowOFHDF5AdaptorGeometryComponent   # 几何分组适配器
    ├── FITKFlowOFHDF5AdaptorGeometryRefine      # 几何优化适配器
    ├── FITKFlowOFHDF5AdaptorGeometryMeshSize    # 网格大小适配器
    ├── FITKFlowOFHDF5AdaptorMaterialPoint       # 材料点适配器
    ├── FITKFlowOFHDF5AdaptorRegionMesh          # 区域网格适配器
    ├── FITKFlowOFHDF5AdaptorMesh                # 网格数据适配器
    ├── FITKFlowOFHDF5AdaptorPhysicsData         # 物理属性适配器
    ├── FITKFlowOFHDF5AdaptorBoundary            # 边界条件适配器
    ├── FITKFlowOFHDF5AdaptorInitialCondition    # 初始条件适配器
    ├── FITKFlowOFHDF5AdaptorSolution            # 求解方案适配器
    ├── FITKFlowOFHDF5AdaptorRunControl          # 运行控制适配器
    ├── FITKFlowOFHDF5AdaptorOperatingCond       # 操作条件适配器
    ├── FITKFlowOFHDF5AdaptorThermo              # 热物理适配器
    ├── FITKFlowOFHDF5AdaptorTurbulence          # 湍流模型适配器
    ├── FITKFlowOFHDF5AdaptorTransportProp       # 输运属性适配器
    └── FITKFlowOFHDF5AdaptorRadiation           # 辐射模型适配器
```

## 依赖关系

### 编译依赖

根据模块源码中的头文件引用，该模块当前显式依赖：

- Qt5::Core
- HDF5 库（通过 H5::H5File、H5::Group 等类使用）
- FITKCore（基础对象、数据对象、参数系统）
- FITKAppFramework（组件接口）
- FITKAdaptor（IO适配器基类）
- FITKInterfaceIO（抽象HDF5读写接口）

### 模块层级关系

- FITKFlowOFIOHDF5 依赖 `FITKCore`、`FITKAppFramework`、`FITKAdaptor` 和 `FITKInterfaceIO`
- 上层模块 `FITKFlowOF` 通过该组件实现仿真数据的 HDF5 格式读写
- 该组件通过 `FITKComponentInterface` 接口被应用框架加载和管理

## 主要类与功能说明

### 核心接口类

#### `FITKFlowOFIOHDF5Interface`

HDF5 文件读写组件的对外接口，继承自 `FITKComponentInterface`。负责管理文件流的打开与关闭、执行读写操作，并支持线程化执行模式。

**主要方法：**
- `setFileName()`：设置要读写的文件名
- `openFileStream()`：打开文件流（1=读取，2=写出）
- `closeFileStream()`：关闭文件流
- `exec()`：执行读取或写出操作
- `setRunInThread()`：设置是否在独立线程中执行

### 读写器类

#### `FITKFlowOFHDF5Reader`

HDF5 文件读取器，继承自 `FITKAbstractHDF5Reader`。负责从 HDF5 文件中读取各类仿真数据。

**主要方法：**
- `run()`：执行读取任务（线程入口）
- `readVersion()`：读取版本号
- `readDataGeometry()`：读取几何数据
- `readMesh()`：读取网格数据
- `readPhysicsData()`：读取物理属性数据
- `consoleMessage()`：输出控制台消息

#### `FITKFlowOFHDF5Writer`

HDF5 文件写入器，继承自 `FITKAbstractHDF5Writer`。负责将各类仿真数据写入 HDF5 文件。

**主要方法：**
- `run()`：执行写入任务（线程入口）
- `writeVersion()`：写出版本号
- `writeDataGeometry()`：写出几何数据
- `writeMesh()`：写出网格数据
- `writePhysicsData()`：写出物理属性数据
- `consoleMessage()`：输出控制台消息

### 抽象适配器类

#### `FITKFlowOFHDF5AbstractAdaptor`

抽象的 HDF5 文件读写适配器基类，继承自 `FITKAbstractIOAdaptor`。提供数据对象与 HDF5 数据结构之间转换的通用方法。

**核心功能：**
- 提供 DataObject 和 NDataObject 的读写方法
- 支持多种参数类型（Bool、Int、Double、String、Combox、RadioGroup 等）的读写
- 提供 Variant 数据的 HDF5 序列化能力
- 输出控制台消息的统一接口

### 具体适配器类

具体适配器类继承自 `FITKFlowOFHDF5AbstractAdaptor`，负责特定类型数据的读写：

| 适配器类 | 功能说明 |
|---------|---------|
| `FITKFlowOFHDF5AdaptorGeometryComponent` | 几何分组数据读写 |
| `FITKFlowOFHDF5AdaptorGeometryRefine` | 几何优化数据读写 |
| `FITKFlowOFHDF5AdaptorGeometryMeshSize` | 网格大小设置读写 |
| `FITKFlowOFHDF5AdaptorMaterialPoint` | 材料点数据读写 |
| `FITKFlowOFHDF5AdaptorRegionMesh` | 区域网格数据读写 |
| `FITKFlowOFHDF5AdaptorMesh` | 网格数据读写 |
| `FITKFlowOFHDF5AdaptorPhysicsData` | 物理属性数据读写 |
| `FITKFlowOFHDF5AdaptorBoundary` | 边界条件数据读写 |
| `FITKFlowOFHDF5AdaptorInitialCondition` | 初始条件数据读写 |
| `FITKFlowOFHDF5AdaptorSolution` | 求解方案数据读写 |
| `FITKFlowOFHDF5AdaptorRunControl` | 运行控制参数读写 |
| `FITKFlowOFHDF5AdaptorOperatingCond` | 操作条件数据读写 |
| `FITKFlowOFHDF5AdaptorThermo` | 热物理模型数据读写 |
| `FITKFlowOFHDF5AdaptorTurbulence` | 湍流模型数据读写 |
| `FITKFlowOFHDF5AdaptorTransportProp` | 输运属性数据读写 |
| `FITKFlowOFHDF5AdaptorRadiation` | 辐射模型数据读写 |

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库，需链接 HDF5 库及 FITK 核心模块：

- `add_library(FITKFlowOFIOHDF5 SHARED)`
- 启用 `AUTOMOC`
- 链接依赖：`FITKCore`、`FITKAppFramework`、`FITKAdaptor`、`FITKInterfaceIO`、`HDF5::HDF5`

### 在上层工程中的参与方式

FITKFlowOFIOHDF5 作为组件层模块，通常通过以下方式接入应用：

1. 通过组件工厂注册到应用框架
2. 在需要读写 HDF5 文件时，获取组件实例并调用接口
3. 配合 `FITKFlowOF` 模块完成 OpenFOAM 仿真数据的持久化

## 使用示例

### 1. 使用接口类读取 HDF5 文件

```cpp
#include "FITKFlowOFIOHDF5Interface.h"

// 创建接口实例
IO::FITKFlowOFIOHDF5Interface* ioInterface = new IO::FITKFlowOFIOHDF5Interface();

// 设置文件名
ioInterface->setFileName("case_data.h5");

// 打开文件流（1=读取模式）
if (ioInterface->openFileStream(1)) {
    // 执行读取操作
    bool success = ioInterface->exec(1);
    
    if (success) {
        // 读取成功，数据已加载到数据仓库
        qDebug() << "HDF5 file read successfully";
    }
    
    // 关闭文件流
    ioInterface->closeFileStream();
}

// 释放资源
delete ioInterface;
```

### 2. 使用接口类写出 HDF5 文件

```cpp
#include "FITKFlowOFIOHDF5Interface.h"

// 创建接口实例
IO::FITKFlowOFIOHDF5Interface* ioInterface = new IO::FITKFlowOFIOHDF5Interface();

// 设置文件名
ioInterface->setFileName("case_data.h5");

// 打开文件流（2=写出模式）
if (ioInterface->openFileStream(2)) {
    // 执行写出操作
    bool success = ioInterface->exec(2);
    
    if (success) {
        qDebug() << "HDF5 file write successfully";
    }
    
    // 关闭文件流
    ioInterface->closeFileStream();
}

// 释放资源
delete ioInterface;
```

### 3. 使用读写器直接操作（高级用法）

```cpp
#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"

// 创建读取器
IO::FITKFlowOFHDF5Reader* reader = new IO::FITKFlowOFHDF5Reader();
reader->setFileName("input.h5");

// 设置数据对象指针（从数据仓库获取）
reader->setDataObject(FITKDATAREPO->getDataById("physics_data"));

// 执行读取
reader->run();

// 创建写入器
IO::FITKFlowOFHDF5Writer* writer = new IO::FITKFlowOFHDF5Writer();
writer->setFileName("output.h5");
writer->setDataObject(FITKDATAREPO->getDataById("physics_data"));

// 执行写入
writer->run();

// 释放资源
delete reader;
delete writer;
```

### 4. 线程化执行模式

```cpp
#include "FITKFlowOFIOHDF5Interface.h"

IO::FITKFlowOFIOHDF5Interface* ioInterface = new IO::FITKFlowOFIOHDF5Interface();

// 设置文件名
ioInterface->setFileName("large_case.h5");

// 设置为线程化模式（默认开启）
ioInterface->setRunInThread(true);

// 连接完成信号
QObject::connect(ioInterface, &IO::FITKFlowOFIOHDF5Interface::ioThreadFinishedSig,
                 []() {
                     qDebug() << "IO operation finished";
                 });

// 打开并执行写出
if (ioInterface->openFileStream(2)) {
    ioInterface->exec(2);
}

// 注意：线程化模式下，exec() 会立即返回，实际操作在后台线程执行
```

## 注意事项

- FITKFlowOFIOHDF5 是 OpenFOAM 流程组件的底层支撑模块，通常不直接由应用层调用，而是通过 `FITKFlowOF` 模块间接使用
- HDF5 文件操作涉及大量数据读写，建议使用线程化模式避免阻塞主线程
- 使用完成后需确保调用 `closeFileStream()` 释放文件资源
- 数据对象的生命周期由数据仓库管理，适配器仅负责数据转换，不持有对象所有权
- 读写操作依赖 `FITKAbstractDataObject` 派生类的序列化接口实现

最后更新时间：2026-04-30