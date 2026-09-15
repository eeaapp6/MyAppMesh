# FITKOFMeshIO

## 项目描述

FITKOFMeshIO 是 FITK 框架的 OpenFOAM 网格输入输出组件，专门用于读取和处理 OpenFOAM 格式的网格文件。该组件提供了完整的 OpenFOAM 网格数据解析功能，支持各种类型的网格数据。

## 主要功能

- **OpenFOAM 网格读取**：读取 OpenFOAM 格式的网格文件
- **网格数据解析**：解析网格的拓扑结构和几何信息
- **多格式支持**：支持 polyMesh、points、faces 等多种网格格式
- **数据转换**：将 OpenFOAM 网格数据转换为内部格式

## 技术栈

- **编程语言**：C++
- **框架**：Qt
- **外部依赖**：OpenFOAM
- **构建系统**：qmake

## 项目结构

```
FITKOFMeshIO/
├── FITKOFMeshReader.cpp/.h              # OpenFOAM 网格读取器基类
├── FITKOpenFOAMMeshReader.cpp/.h        # OpenFOAM 网格读取器实现
├── FITKOFMeshIOAPI.h                    # API 导出宏定义
├── FITKOFMeshIO.pri                     # qmake 项目文件
└── FITKOFMeshIO.pro                     # 主项目文件
```

## 核心类

### FITKOFMeshReader
OpenFOAM 网格读取器的抽象基类，定义了网格读取的通用接口。

### FITKOpenFOAMMeshReader
OpenFOAM 网格读取器的具体实现类，提供以下功能：
- **网格拓扑读取**：读取点和面的连接关系
- **边界条件读取**：解析边界条件定义
- **区域信息读取**：读取网格区域（cellZone）信息
- **多面体支持**：支持 OpenFOAM 的多面体网格

## 主要特性

- **完整的 OpenFOAM 支持**：支持 OpenFOAM 的所有网格格式
- **高性能解析**：优化的解析算法，支持大型网格文件
- **错误处理**：完善的错误检测和报告机制
- **内存优化**：高效的内存管理，减少内存占用
- **数据验证**：网格数据的完整性验证

## 支持的网格格式

### 基础网格文件
- **points**：网格顶点坐标文件
- **faces**：网格面定义文件
- **owner**：面所属单元文件
- **neighbour**：相邻单元文件
- **boundary**：边界条件定义文件

### 高级网格文件
- **cells**：单元定义文件
- **pointZones**：点区域文件
- **faceZones**：面区域文件
- **cellZones**：单元区域文件

### 特殊网格格式
- **polyMesh**：多面体网格格式
- **extendedFeatureEdgeMesh**：扩展特征边网格
- **sets**：网格集合文件

## 依赖关系

- **OpenFOAM**：CFD 网格和求解器框架
- **FITK_Kernel**：依赖 FITK 核心模块
- **Qt Core**：基础 Qt 功能

## 编译要求

- Qt 5.x 或更高版本
- OpenFOAM 安装环境
- C++11 或更高标准
- Linux 环境（OpenFOAM 主要平台）

## 使用示例

```cpp
#include "FITKOpenFOAMMeshReader.h"

// 创建 OpenFOAM 网格读取器
FITKOpenFOAMMeshReader* meshReader = new FITKOpenFOAMMeshReader();

// 设置案例目录
meshReader->setCaseDirectory("/path/to/OpenFOAM/case");

// 设置时间步
meshReader->setTimeStep("0");

// 读取网格
bool success = meshReader->readMesh();

if (success) {
    // 获取网格数据
    const std::vector<Point>& points = meshReader->getPoints();
    const std::vector<Face>& faces = meshReader->getFaces();
    const std::vector<Cell>& cells = meshReader->getCells();
}
```

## 数据结构

### 网格数据类型
```cpp
// 点数据
struct Point {
    double x, y, z;
};

// 面数据
struct Face {
    std::vector<int> pointIds;
    int ownerCell;
    int neighbourCell;
};

// 单元数据
struct Cell {
    std::vector<int> faceIds;
    std::string zoneName;
};
```

## 性能优化

- **流式读取**：支持大型网格文件的流式读取
- **内存映射**：使用内存映射技术提高读取性能
- **并行处理**：支持多线程网格解析
- **缓存机制**：智能缓存减少重复读取

## 错误处理

### 常见错误类型
- **文件不存在**：网格文件缺失或路径错误
- **格式错误**：网格文件格式不正确
- **数据不一致**：网格数据拓扑关系错误
- **内存不足**：大型网格文件内存不足

### 错误处理策略
- **详细错误信息**：提供具体的错误位置和原因
- **恢复机制**：支持部分数据恢复
- **日志记录**：详细的错误日志记录


