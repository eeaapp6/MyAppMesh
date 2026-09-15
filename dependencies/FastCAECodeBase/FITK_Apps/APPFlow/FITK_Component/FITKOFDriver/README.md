# FITKOFDriver

## 项目描述

FITKOFDriver 是 FITK 框架的 OpenFOAM 驱动组件，提供了与 OpenFOAM CFD 软件的完整集成接口。该组件支持 OpenFOAM 的各种操作，包括网格生成、求解器运行、后处理等。

## 主要功能

- **OpenFOAM 驱动**：提供 OpenFOAM 命令的统一接口
- **网格生成**：支持 blockMesh、snappyHexMesh 等网格生成工具
- **并行计算**：支持 decomposePar 和 reconstructPar 并行操作
- **求解器管理**：支持各种 OpenFOAM 求解器的调用
- **后处理**：支持 foamToVTK 等后处理工具

## 技术栈

- **编程语言**：C++
- **框架**：Qt
- **外部依赖**：OpenFOAM
- **构建系统**：qmake

## 项目结构

```
FITKOFDriver/
├── FITKOFBlockMeshDriver.cpp/.h              # blockMesh 驱动
├── FITKOFCreatePatchDriver.cpp/.h            # createPatch 驱动
├── FITKOFDecomposeParDriver.cpp/.h           # decomposePar 驱动
├── FITKOFFoamDictionaryDriver.cpp/.h         # 字典文件驱动
├── FITKOFFoamMultiRunDriver.cpp/.h           # 多次运行驱动
├── FITKOFFoamRunDriver.cpp/.h                # 单次运行驱动
├── FITKOFFoamToVTKDriver.cpp/.h              # foamToVTK 驱动
├── FITKOFInputInfo.cpp/.h                    # 输入信息管理
├── FITKOFInterface.cpp/.h                    # OF 接口类
├── FITKOFReconstructParDriver.cpp/.h         # reconstructPar 驱动
├── FITKOFSnappyHexMeshDriver.cpp/.h          # snappyHexMesh 驱动
├── FITKOFSnappyHexMeshParallelDriver.cpp/.h  # snappyHexMesh 并行驱动
├── FITKOpenFOAMDriver.cpp/.h                 # OpenFOAM 主驱动
├── FITKOpenFoamMeshingDriverBase.cpp/.h      # 网格生成驱动基类
├── FITKOFDriverAPI.h                         # API 导出宏定义
├── FITKOFDriver.pri                          # qmake 项目文件
└── FITKOFDriver.pro                          # 主项目文件
```

## 核心驱动类

### 网格生成驱动
- **FITKOFBlockMeshDriver**：处理 blockMesh 网格生成
- **FITKOFSnappyHexMeshDriver**：处理 snappyHexMesh 网格生成
- **FITKOFSnappyHexMeshParallelDriver**：并行 snappyHexMesh 驱动

### 并行计算驱动
- **FITKOFDecomposeParDriver**：处理域分解
- **FITKOFReconstructParDriver**：处理结果重构

### 求解器驱动
- **FITKOFFoamRunDriver**：单次求解器运行
- **FITKOFFoamMultiRunDriver**：多次求解器运行

### 后处理驱动
- **FITKOFFoamToVTKDriver**：转换为 VTK 格式
- **FITKOFCreatePatchDriver**：创建和修改边界

### 工具驱动
- **FITKOFFoamDictionaryDriver**：字典文件操作
- **FITKOFInputInfo**：输入信息管理

## 主要特性

- **统一接口**：为所有 OpenFOAM 工具提供一致的调用接口
- **并行支持**：完整的 OpenFOAM 并行计算支持
- **错误处理**：完善的错误检测和处理机制
- **进度监控**：支持长时间运行任务的进度监控
- **日志记录**：详细的操作日志记录
- **配置管理**：灵活的配置参数管理

## 依赖关系

- **OpenFOAM**：CFD 求解器框架
- **FITK_Kernel**：依赖 FITK 核心模块
- **Qt Core**：基础 Qt 功能

## 编译要求

- Qt 5.x 或更高版本
- OpenFOAM 安装环境
- C++11 或更高标准
- Linux 环境（OpenFOAM 主要平台）

## 使用示例

```cpp
#include "FITKOpenFOAMDriver.h"

// 创建 OpenFOAM 驱动
FITKOpenFOAMDriver* ofDriver = new FITKOpenFOAMDriver();

// 设置案例目录
ofDriver->setCaseDirectory("/path/to/case");

// 运行 blockMesh
FITKOFBlockMeshDriver* blockMesh = ofDriver->getBlockMeshDriver();
bool success = blockMesh->execute();

// 运行求解器
FITKOFFoamRunDriver* solver = ofDriver->getFoamRunDriver();
solver->setSolverName("simpleFoam");
success = solver->execute();
```

## 配置选项

### OpenFOAM 环境配置
- **WM_PROJECT_DIR**：OpenFOAM 安装目录
- **WM_PROJECT_VERSION**：OpenFOAM 版本
- **FOAM_RUN**：运行时目录

### 驱动配置
- **并行进程数**：设置并行计算的核心数
- **超时时间**：设置命令执行的超时时间
- **日志级别**：设置日志记录的详细程度

## 支持的 OpenFOAM 版本

- OpenFOAM 7.x
- OpenFOAM 8.x
- OpenFOAM 9.x
- OpenFOAM 10.x
- foam-extend 版本

## 错误处理

组件提供完善的错误处理机制：
- **命令执行错误**：检测 OpenFOAM 命令执行失败
- **环境错误**：检测 OpenFOAM 环境配置问题
- **文件错误**：检测输入文件和输出文件问题
- **权限错误**：检测文件和目录权限问题


