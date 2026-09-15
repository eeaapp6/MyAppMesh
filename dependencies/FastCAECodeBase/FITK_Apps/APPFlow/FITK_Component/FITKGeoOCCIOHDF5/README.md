# FITKGeoOCCIOHDF5

## 项目描述

FITKGeoOCCIOHDF5 是 FITK 框架的几何 OpenCASCADE HDF5 输入输出组件，专门用于处理基于 OpenCASCADE 的几何数据的高效存储和读取。该组件结合了 OCC 的几何建模能力和 HDF5 的高效数据存储特性。

## 主要功能

- **OCC 几何转换**：将 OpenCASCADE 几何数据转换为 HDF5 格式
- **高效存储**：利用 HDF5 的高效存储和索引特性
- **几何数据读取**：快速读取复杂几何模型数据
- **版本管理**：支持几何模型的版本控制和历史记录
- **压缩存储**：支持几何数据的压缩存储

## 技术栈

- **编程语言**：C++
- **框架**：Qt
- **几何内核**：OpenCASCADE Technology (OCC)
- **数据格式**：HDF5
- **构建系统**：qmake

## 项目结构

```
FITKGeoOCCIOHDF5/
├── FITKGeoOCCIOHDF5API.h           # API 导出宏定义
├── FITKGeoOCCIOHDF5.pri            # qmake 项目文件
├── FITKGeoOCCIOHDF5.pro            # 主项目文件
└── [其他源文件]
```

## 核心功能

### 几何数据转换
- **拓扑结构**：几何拓扑结构的转换存储
- **几何实体**：点、线、面、体等几何实体转换
- **曲面数据**：NURBS 曲面和其他曲面数据转换
- **装配结构**：装配层次结构的存储

### 存储管理
- **分块存储**：大型几何模型的分块存储
- **索引优化**：快速几何检索的索引结构
- **元数据管理**：完整的几何元数据信息
- **压缩算法**：几何数据的高效压缩

### 读取功能
- **选择性加载**：按需加载特定几何部件
- **渐进加载**：大型模型的渐进式加载
- **缓存机制**：几何数据的智能缓存
- **版本回溯**：历史版本的几何数据回溯

## 数据结构

### HDF5 文件结构
```
geometry_data.h5
├── /topology/
│   ├── solids          # 实体拓扑
│   ├── shells          # 壳体拓扑
│   ├── faces           # 面拓扑
│   ├── edges           # 边拓扑
│   └── vertices        # 顶点拓扑
├── /geometry/
│   ├── surfaces        # 曲面几何
│   ├── curves          # 曲线几何
│   ├── points          # 点几何
│   └── transformations # 变换矩阵
├── /assembly/
│   ├── components      # 装配组件
│   ├── constraints     # 装配约束
│   └── hierarchy       # 装配层次
├── /attributes/
│   ├── properties      # 几何属性
│   ├── materials       # 材料属性
│   └── colors          # 颜色属性
└── /metadata/
    ├── version         # 版本信息
    ├── created_by      # 创建信息
    └── modified_by     # 修改信息
```

## 使用示例

```cpp
#include "FITKGeoOCCIOHDF5.h"

// 创建 OCC HDF5 IO 对象
FITKGeoOCCIOHDF5* occHDF5 = new FITKGeoOCCIOHDF5();

// 设置 OCC 几何模型
occHDF5->setOCCGeometry(occShape);

// 保存为 HDF5 格式
bool success = occHDF5->saveToHDF5("geometry_model.h5");

if (success) {
    // 读取几何模型
    occHDF5->loadFromHDF5("geometry_model.h5");
    
    // 获取特定部件
    TopoDS_Shape component = occHDF5->getComponent("component_name");
}
```

## 几何类型支持

### 基础几何
- **点几何**：3D 空间点
- **线几何**：直线、圆弧、样条曲线
- **面几何**：平面、圆柱面、球面、NURBS 曲面
- **体几何**：实体、壳体

### 复杂几何
- **自由曲面**：复杂的自由形状曲面
- **雕塑实体**：复杂的雕塑实体
- **装配体**：多层次装配结构
- **参数化几何**：参数化几何模型

## 性能特性

- **高效压缩**：几何数据的高效压缩算法
- **快速检索**：基于索引的快速几何检索
- **内存优化**：大几何模型的内存优化
- **并行处理**：多线程几何数据处理

## 应用场景

- **CAD 数据交换**：不同 CAD 系统间的数据交换
- **几何版本管理**：产品设计版本的管理
- **大规模几何处理**：大型装配体的处理
- **几何数据备份**：重要几何数据的备份存储

