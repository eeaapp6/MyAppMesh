# FITKFlowOFIOHDF5

## 项目描述

FITKFlowOFIOHDF5 是 FITK 框架的流场数据 HDF5 输入输出组件，专门用于处理 OpenFOAM 流场数据的高效存储和读取。该组件结合了 OpenFOAM 和 HDF5 技术，提供了高性能的流场数据管理解决方案。

## 主要功能

- **OpenFOAM 数据转换**：将 OpenFOAM 流场数据转换为 HDF5 格式
- **高效存储**：利用 HDF5 的高效压缩和索引特性
- **流场数据读取**：快速读取大规模流场数据
- **时间序列管理**：支持时间序列流场数据的存储和管理
- **并行 IO**：支持并行读写操作

## 技术栈

- **编程语言**：C++
- **框架**：Qt
- **数据格式**：HDF5
- **外部依赖**：OpenFOAM
- **构建系统**：qmake

## 项目结构

```
FITKFlowOFIOHDF5/
├── FITKFlowOFIOHDF5API.h           # API 导出宏定义
├── FITKFlowOFIOHDF5.pri            # qmake 项目文件
├── FITKFlowOFIOHDF5.pro            # 主项目文件
└── [其他源文件]
```

## 核心功能

### 数据转换
- **场数据转换**：速度、压力、温度等场数据转换
- **网格数据转换**：OpenFOAM 网格到 HDF5 格式转换
- **边界条件转换**：边界条件数据的转换存储
- **时间序列转换**：多时间步数据的批量转换

### 存储优化
- **压缩存储**：支持多种压缩算法
- **分块存储**：优化大数据量的存储性能
- **索引管理**：快速数据检索的索引结构
- **元数据管理**：完整的元数据信息存储

### 读取功能
- **选择性读取**：按区域、时间步、变量选择读取
- **流式读取**：支持大文件的流式读取
- **缓存机制**：智能缓存提高读取性能
- **并行读取**：多线程并行数据读取

## 数据结构

### HDF5 文件结构
```
flow_data.h5
├── /mesh/
│   ├── points          # 网格节点
│   ├── faces           # 网格面
│   ├── cells           # 网格单元
│   └── boundary        # 边界信息
├── /fields/
│   ├── U/              # 速度场
│   ├── p/              # 压力场
│   ├── T/              # 温度场
│   └── ...             # 其他场变量
├── /time/
│   ├── 0.0/            # 时间步数据
│   ├── 0.1/            # 时间步数据
│   └── ...             # 其他时间步
└── /metadata/
    ├── case_info       # 案例信息
    ├── solver_info     # 求解器信息
    └── mesh_info       # 网格信息
```

## 使用示例

```cpp
#include "FITKFlowOFIOHDF5.h"

// 创建 HDF5 IO 对象
FITKFlowOFIOHDF5* hdf5IO = new FITKFlowOFIOHDF5();

// 设置 OpenFOAM 案例目录
hdf5IO->setOpenFOAMCase("/path/to/case");

// 转换为 HDF5 格式
bool success = hdf5IO->convertToHDF5("flow_data.h5");

if (success) {
    // 读取特定时间步的数据
    hdf5IO->setTimeStep(0.5);
    hdf5IO->readField("U");      // 读取速度场
    hdf5IO->readField("p");      // 读取压力场
}
```

## 性能特性

- **高速读写**：优化的 HDF5 IO 性能
- **内存效率**：低内存占用的大数据处理
- **压缩率**：高效的数据压缩算法
- **可扩展性**：支持 TB 级数据规模

## 应用场景

- **大规模 CFD 后处理**：处理大规模计算流体力学数据
- **时间序列分析**：流场数据的时序分析
- **数据可视化**：为可视化系统提供高效数据源
- **机器学习**：为 AI 训练提供结构化数据

