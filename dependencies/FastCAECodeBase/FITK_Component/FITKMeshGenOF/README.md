# FITKMeshGenOF - OpenFOAM 网格生成组件

## 简介

FITKMeshGenOF 是 FastCAE 集成工具包（FITK）中负责 OpenFOAM 网格生成的功能组件。它封装了 OpenFOAM 的网格生成工具链（blockMesh、snappyHexMesh、createPatch），提供统一的网格生成接口，支持结构化和非结构化网格生成。

该模块解决的核心问题是：如何将用户定义的几何模型和网格参数转换为 OpenFOAM 可执行的网格生成命令，并管理网格生成的完整流程。

## 模块定位

FITKMeshGenOF 位于 `FITK_Component` 下，属于功能组件层，主要承担以下职责：

- 提供 OpenFOAM 网格生成的统一接口
- 封装 blockMesh、snappyHexMesh、createPatch 等网格生成工具
- 管理网格尺寸信息的生成与配置
- 协调网格生成流程的执行与状态管理
- 作为流体仿真流程中网格生成阶段的核心支撑模块

## 架构与目录

从当前目录中的头文件和源文件看，FITKMeshGenOF 可以概括为以下几组能力：

```text
FITKMeshGenOF
├── 核心接口
│   ├── FITKMeshGenOFAPI          # API声明宏
│   └── FITKMeshGenOFInterface    # 网格生成组件接口
├── 网格驱动器
│   └── FITKOFMesherDriver        # OpenFOAM网格驱动器
├── 网格处理器
│   └── FITKOFMeshProcessor       # 网格处理器
└── 网格尺寸生成器
    ├── FITKOFMeshSizeGenerator           # 网格尺寸生成器
    └── FITKOFGeometryMeshSizeGenerator   # 几何区域网格尺寸生成器
```

## 依赖关系

### 编译依赖

根据模块源码中的头文件引用，该模块当前显式依赖：

- Qt5::Core
- FITKCore（基础对象、数据对象）
- FITKAppFramework（组件接口）
- FITKInterfaceMeshGen（网格生成接口）

### 模块层级关系

- FITKMeshGenOF 依赖 `FITKCore`、`FITKAppFramework` 和 `FITKInterfaceMeshGen`
- 上层流体仿真模块通过该组件实现网格生成功能
- 该组件通过 `FITKComponentInterface` 接口被应用框架加载和管理

## 主要类与功能说明

### 核心接口类

#### `FITKMeshGenOFInterface`

OpenFOAM 网格生成组件的对外接口，继承自 `FITKComponentInterface`。作为组件注册和执行的入口。

**主要方法：**
- `getComponentName()`：返回组件名称 "FITKMeshGenOF"
- `exec()`：执行网格生成操作

### 网格驱动器类

#### `FITKOFMesherDriver`

OpenFOAM 网格驱动器，继承自 `FITKAbstractMesherDriver`。负责调用 OpenFOAM 的网格生成命令。

**主要方法：**
- `startMesher()`：启动网格划分程序
- `stopMesher()`：结束网格划分程序

**槽函数：**
- `blockMeshSlot()`：blockMesh 命令执行结束处理
- `snappyHexMeshSlot()`：snappyHexMesh 命令执行结束处理
- `createPatchSlot()`：createPatch 命令执行结束处理

### 网格处理器类

#### `FITKOFMeshProcessor`

网格处理器，继承自 `FITKAbstractMeshProcessor`。负责网格数据的处理和转换。

**主要方法：**
- `start()`：开始执行网格处理

### 网格尺寸生成器类

#### `FITKOFMeshSizeGenerator`

网格尺寸生成器，继承自 `FITKAbstractMeshSizeInfoGenerator`。负责生成全局和区域网格尺寸信息。

**主要方法：**
- `generateGlobalMeshSizeInfo()`：生成全局尺寸信息
- `createRegionMeshSize()`：创建局部区域网格尺寸

#### `FITKOFGeometryMeshSizeGenerator`

几何区域网格尺寸生成器，继承自 `FITKAbstractGeometryMeshSizeGenerator`。负责生成几何相关的网格尺寸信息。

**主要方法：**
- `createGeometryMeshSize()`：创建几何网格尺寸对象

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `add_library(FITKMeshGenOF SHARED)`
- 启用 `AUTOMOC`
- 链接依赖：`FITKCore`、`FITKAppFramework`、`FITKInterfaceMeshGen`

### 在上层工程中的参与方式

FITKMeshGenOF 作为组件层模块，通常通过以下方式接入应用：

1. 通过组件工厂注册到应用框架
2. 在网格生成阶段获取组件实例并调用执行
3. 配合流体仿真流程完成网格生成任务

## 使用示例

### 1. 使用接口类执行网格生成

```cpp
#include "FITKMeshGenOFInterface.h"

// 创建接口实例
OF::FITKMeshGenOFInterface* meshGenInterface = new OF::FITKMeshGenOFInterface();

// 执行网格生成
bool success = meshGenInterface->exec(1);

if (success) {
    qDebug() << "Mesh generation completed successfully";
}

// 释放资源
delete meshGenInterface;
```

### 2. 使用网格驱动器

```cpp
#include "FITKOFMesherDriver.h"

// 创建网格驱动器
OF::FITKOFMesherDriver* mesherDriver = new OF::FITKOFMesherDriver();

// 连接信号槽
QObject::connect(mesherDriver, &OF::FITKOFMesherDriver::blockMeshSlot,
                 []() {
                     qDebug() << "blockMesh completed";
                 });

QObject::connect(mesherDriver, &OF::FITKOFMesherDriver::snappyHexMeshSlot,
                 []() {
                     qDebug() << "snappyHexMesh completed";
                 });

QObject::connect(mesherDriver, &OF::FITKOFMesherDriver::createPatchSlot,
                 []() {
                     qDebug() << "createPatch completed";
                 });

// 启动网格生成
mesherDriver->startMesher();

// 释放资源（实际应用中应在任务完成后释放）
// delete mesherDriver;
```

### 3. 使用网格尺寸生成器

```cpp
#include "FITKOFMeshSizeGenerator.h"

// 创建网格尺寸生成器
OF::FITKOFMeshSizeGenerator* sizeGenerator = new OF::FITKOFMeshSizeGenerator();

// 生成全局网格尺寸信息
Interface::FITKGlobalMeshSizeInfo* globalSize = sizeGenerator->generateGlobalMeshSizeInfo();

// 设置全局网格尺寸参数
if (globalSize) {
    globalSize->setMaxCellSize(0.1);
    globalSize->setMinCellSize(0.01);
}

// 创建区域网格尺寸
Interface::FITKAbstractRegionMeshSize* regionSize = 
    sizeGenerator->createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::Internal);

// 释放资源
delete sizeGenerator;
delete globalSize;
delete regionSize;
```

### 4. 使用几何网格尺寸生成器

```cpp
#include "FITKOFGeometryMeshSizeGenerator.h"

// 创建几何网格尺寸生成器
OF::FITKOFGeometryMeshSizeGenerator* geoSizeGenerator = 
    new OF::FITKOFGeometryMeshSizeGenerator();

// 创建几何网格尺寸对象
Interface::FITKGeometryMeshSize* geoSize = geoSizeGenerator->createGeometryMeshSize();

// 设置几何网格尺寸参数
if (geoSize) {
    // 设置特定几何面的网格尺寸
    // geoSize->setMeshSize(faceId, 0.05);
}

// 释放资源
delete geoSizeGenerator;
delete geoSize;
```

### 5. 使用网格处理器

```cpp
#include "FITKOFMeshProcessor.h"

// 创建网格处理器
OF::FITKOFMeshProcessor* meshProcessor = new OF::FITKOFMeshProcessor();

// 设置处理参数
QStringList info;
info << "-input" << "mesh.raw" << "-output" << "mesh_processed.raw";

// 启动处理
meshProcessor->start(info);

// 释放资源
delete meshProcessor;
```

## 注意事项

- FITKMeshGenOF 是流体仿真流程的核心组件，依赖 OpenFOAM 工具链的正确安装
- 网格生成过程可能耗时较长，建议在后台线程中执行
- 网格尺寸参数需要根据具体几何模型和仿真需求合理设置
- 使用前需确保 OpenFOAM 环境变量已正确配置
- 网格驱动器执行的命令路径需要正确配置

最后更新时间：2026-04-30