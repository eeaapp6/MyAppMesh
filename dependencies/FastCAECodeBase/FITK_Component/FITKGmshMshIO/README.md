# FITKGmshMshIO - Gmsh Msh网格文件读写组件

## 简介

FITKGmshMshIO 是 FITK_Component 组件层中专门负责读写 Gmsh Msh 格式网格文件的模块。Gmsh 是一款开源的三维有限元网格生成器，其输出的 Msh 文件格式（特别是版本 2 格式）被广泛应用于科学计算和工程仿真领域。

组件核心价值：
- 灵活的数据过滤选项，支持按维度、单元类型等条件筛选网格数据
- 适配器模式设计，读写逻辑与业务逻辑分离，便于维护和扩展
- 支持自动过滤低维单元和未使用节点，提高内存利用效率

## 模块定位

FITKGmshMshIO 位于 `FITK_Component` 下，属于组件实现层模块，主要职责：

- 定义 Gmsh Msh 文件的读写接口
- 实现 Msh 格式的解析器
- 提供数据过滤选项（按维度、单元类型等）
- 提供适配器模式支持（节点、单元、物理标签等）

架构层级：位于 FITK_Interface 接口层之下，直接依赖 FITKAppFramework 和 FITKAdaptor 框架。

## 架构与目录

```
FITKGmshMshIO
├── 组件接口层
│   ├── FITKGmshMshIOInterface      # 主接口类
│   ├── ReaderGmshMshConfig          # 读取配置结构体
│   └── FITKGmshMshIOAPI            # 导出宏定义
├── 文件读取引擎
│   ├── FITKGmshMshIOReader          # Msh文件读取器
│   ├── FITKGmshMshData              # Gmsh Msh数据类
│   └── FITKGmshMshDataProcessor     # 数据处理器
├── 适配器实现
│   ├── FITKGmshAbstractAdaptor      # 适配器抽象基类
│   ├── FITKGmshAdaptorNodes         # 节点数据适配器
│   ├── FITKGmshAdaptorElements      # 单元数据适配器
│   └── FITKGmshAdaptorObject        # 对象数据适配器
└── 构建配置
    ├── CMakeLists.txt
    ├── FITKGmshMshIO.pro
    └── FITKGmshMshIO.pri
```

## 依赖关系

### 编译依赖

- Qt5::Core、Qt5::Gui、Qt5::Widgets
- FITKAppFramework（FITKComponentInterface）
- FITKCore（FITKAbstractDataObject、FITKAbstractObjectContainer）
- FITKAdaptor（FITKAbstractIOAdaptor、FITKIOAdaptorFactory）
- FITKInterfaceIO（FITKAbstractTextReader、FITKAbstractTextWriter）
- FITKInterfaceModel（FITKModelEnum、FITKAbstractElement）
- FITKInterfaceMesh（FITKUnstructuredMesh）

### 模块层级

- 直接依赖 FITKAppFramework，是组件层的具体实现
- 适配器层依赖 FITKAdaptor 框架，通过工厂机制注册和管理适配器实例
- 读取引擎依赖 FITKInterfaceIO 的文本读取基类
- 数据处理依赖 FITKInterfaceModel 和 FITKInterfaceMesh，将 Gmsh 数据映射到 FITK 标准数据结构

## 主要类与功能

### FITKGmshMshIOInterface

组件主接口类，继承自 `FITKComponentInterface`，是上层应用程序与 Msh 读取引擎之间的桥梁。

主要功能：
- `setFileName()` - 设置待读取的 Msh 文件路径
- `getGmshMshData()` - 获取读取后的 Gmsh Msh 数据对象
- `setReaderConfig()` - 设置读取配置
- 组件名称返回 "GmshMshIO"

### ReaderGmshMshConfig

读取配置结构体，用于控制读取行为：

| 配置项 | 类型 | 说明 |
|--------|------|------|
| `_isExtractData` | bool | 是否提取数据到 meshData（默认 true） |
| `_filterDim` | QList<int> | 需要过滤的维度列表 |
| `_isFilterHighToLowCells` | bool | 是否过滤高维单元包含的低维单元 |
| `_isFilterUnusedNode` | bool | 是否过滤未使用的节点 |

### FITKGmshMshIOReader

Msh 文件读取器，继承自 `FITKAbstractTextReader`。逐行解析 Msh 文件，识别不同区块（$Nodes、$Elements 等）。

### FITKGmshMshData

Gmsh Msh 数据类，继承自 `FITKAbstractDataObject`。

主要数据成员：
- `_node_coordX/Y/Z` - 节点坐标数组
- `_element_type` - 单元类型数组
- `_element` - 单元连接数组（QList<int>）
- `_array_dim` / `_array` - 按维度组织的单元编号

主要方法：`init()` 将 meshData 初始化到 Gmsh Msh 数据结构，`update()` 将 Gmsh Msh 数据更新到 meshData。

### FITKGmshMshDataProcessor

数据处理器，负责读取过程中的预处理和过滤。提供 `addNode()`、`addElement()`、`addElementSet()` 等方法。

### 适配器类

| 类名 | 功能 |
|------|------|
| FITKGmshAbstractAdaptor | 适配器抽象基类 |
| FITKGmshAdaptorNodes | 节点信息读取（$Nodes区块） |
| FITKGmshAdaptorElements | 单元信息读取（$Elements区块），支持 1-27 类型映射 |
| FITKGmshAdaptorObject | 物理标签和几何对象信息读取（$PhysicalNames区块） |

## 构建与集成

### CMake

```cmake
project(FITKGmshMshIO VERSION 2.0.0 LANGUAGES CXX)
add_library(${PROJECT_NAME} SHARED)
# 启用 AUTOMOC、AUTORCC、AUTOUIC
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
link_fitk_library(${PROJECT_NAME} ...)  # 链接 FITK 底层模块
```

### qmake

同时保留 `.pro` 和 `.pri` 文件，兼容现有工程组织方式。

### 在上层工程中的接入方式

通过组件工厂机制调用。在应用程序的组件注册阶段将 FITKGmshMshIOInterface 注册到组件工厂中，需要读取 Msh 文件时通过组件接口进行调用。

## 使用示例

### 1. 注册组件

```cpp
#include "ComponentFactory.h"
#include "FITK_Component/FITKGmshMshIO/FITKGmshMshIOInterface.h"

QList<AppFrame::FITKComponentInterface *> ComponentFactory::createComponents()
{
    QList<AppFrame::FITKComponentInterface *> componentList;
    componentList << new Gmsh::FITKGmshMshIOInterface;
    return componentList;
}
```

### 2. 读取Msh文件

```cpp
// 获取组件实例
Gmsh::FITKGmshMshIOInterface* mshIO = FITKAPP->getComponents()
    ->getComponentTByName<Gmsh::FITKGmshMshIOInterface>("GmshMshIO");

mshIO->setFileName(fileName);
mshIO->exec(1);  // indexPort=1 表示读取

// 获取数据
Gmsh::FITKGmshMshData* mshData = mshIO->getGmshMshData();
```

### 3. 读取Msh文件（带过滤配置）

```cpp
Gmsh::ReaderGmshMshConfig config;
config._isExtractData = true;           // 提取数据到 meshData
config._filterDim = {2, 3};             // 只读取二维和三维单元
config._isFilterHighToLowCells = true;  // 过滤低维单元
config._isFilterUnusedNode = true;       // 过滤未使用的节点

mshIO->setReaderConfig(config);
mshIO->setFileName(fileName);
mshIO->exec(1);
```

### 4. 数据转换到FITK网格

```cpp
Interface::FITKUnstructuredMeshVTK* mesh = new Interface::FITKUnstructuredMeshVTK();

// 转换节点
for (int i = 0; i < mshData->_node_coordX.size(); ++i) {
    mesh->addNode(mshData->_node_coordX[i], 
                  mshData->_node_coordY[i], 
                  mshData->_node_coordZ[i]);
}

// 转换单元
for (int i = 0; i < mshData->_element.size(); ++i) {
    Interface::FITKModelEnum::FITKEleType fitkType = 
        static_cast<Interface::FITKModelEnum::FITKEleType>(mshData->_element_type[i]);
    mesh->addElement(fitkType, mshData->_element[i]);
}
mesh->update();
```

## 支持的Msh格式特性

### 版本支持

- **版本 2 格式（推荐）**：完整支持，包含节点、单元、物理组等所有区块
- **版本 4 格式（部分支持）**：基础读取功能支持

### 支持的区块

| 区块 | 说明 |
|------|------|
| $Nodes | 节点坐标定义（编号、三维坐标） |
| $Elements | 单元定义（编号、类型、节点连接） |
| $PhysicalNames | 物理组名称定义 |
| $PartitionedRegions | 分区信息（可选） |

### 支持的单元类型

| Gmsh Type | FITK 单元类型 | 节点数 | 维度 |
|-----------|---------------|--------|------|
| 15 | Vertex1 | 1 | 0D |
| 1 | Line2 | 2 | 1D |
| 8 | Line3 | 3 | 1D（二次） |
| 2 | Tri3 | 3 | 2D |
| 9 | Tri6 | 6 | 2D（二次） |
| 3 | Quad4 | 4 | 2D |
| 10 | Quad8 | 8 | 2D（二次） |
| 4 | Tet4 | 4 | 3D |
| 11 | Tet10 | 10 | 3D（二次） |
| 5 | Hex8 | 8 | 3D |
| 12 | Hex20 | 20 | 3D（二次） |

## 使用建议

- 大型网格文件建议使用数据过滤配置减少内存占用
- 通过 `_filterDim` 配置项按维度筛选单元
- 使用 `_isFilterHighToLowCells` 自动过滤低维单元
- 通过 `_isFilterUnusedNode` 选项过滤未使用节点

## 注意事项

- 当前组件主要支持读取功能，写出功能尚未实现
- 某些高级特性（复杂物理组定义、参数化坐标等）可能需要扩展
- 线程安全：不应在多个线程中同时操作同一个组件实例
- 编码：Gmsh Msh 文件默认使用 UTF-8 编码
- 节点编号：组件保留原始编号，便于与原始模型对应

最后更新时间：2026-04-22