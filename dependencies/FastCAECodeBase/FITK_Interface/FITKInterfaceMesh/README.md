# FITKInterfaceMesh - 网格接口抽象层

## 简介

FITKInterfaceMesh 是 FITK_Interface 抽象接口层中专门负责网格数据抽象的模块，主要提供与 VTK 渲染引擎对接的网格接口定义以及分区并行计算所需的网格分区接口。该模块不实现具体的网格生成算法，而是为上层组件提供统一的网格数据抽象和 VTK 映射能力，使得不同网格生成引擎的适配工作能够在接口层面保持一致。

FITKInterfaceMesh 的核心价值在于“解耦网格数据结构与渲染实现”。上层组件（如几何接口、物理接口）只需要了解统一的网格抽象接口，而具体的 VTK 底层渲染细节被封装在接口实现层中。同时，分区接口的引入为并行计算场景下的网格数据传递提供了标准化的数据结构支持。

## 模块定位

FITKInterfaceMesh 位于 `FITK_Interface` 下，属于抽象接口层模块，主要承担以下职责：

- 定义与非结构化网格、结构化网格相关的抽象接口，供 VTK 适配器实现。
- 提供 FITK 内部单元类型与 VTK 单元类型之间的映射关系。
- 定义分区并行计算中分区面（Partition Interface）的数据结构和管理机制。
- 为网格数据的序列化和反序列化提供基础类型支持。

从架构层级来看，FITKInterfaceMesh 位于 FITKCore 基础对象层之上，但不直接依赖具体的网格生成组件。它通过接口抽象为上层几何、物理等业务模块提供网格数据的统一访问方式。

## 架构与目录

从当前目录中的头文件和源文件看，FITKInterfaceMesh 可以概括为以下几组能力：

```text
FITKInterfaceMesh
├── 网格接口与VTK实现
│   ├── FITKUnstructuredMeshVTK
│   ├── FITKStructuredMeshVTK
│   ├── FITKUnstructuredFluidMeshVTK
│   └── FITKMeshFeatureVTK
├── 单元类型映射
│   └── FITKMeshVTKMap
└── 分区接口管理
    ├── FITKPartitionInterface
    └── FITKPartitionInterfaceManager
```

## 依赖关系

### 编译依赖

根据 `FITK_Interface/FITKInterfaceMesh/CMakeLists.txt`，该模块当前显式依赖：

- Qt5::Core
- VTK（通过 vtk.pri 配置）
- FITKCore（FITKAbstractDataObject、FITKAbstractDataManager）
- FITKInterfaceModel（FITKUnstructuredMesh、FITKModelEnum）

### 模块层级关系

- FITKInterfaceMesh 直接依赖 `FITKCore` 和 `FITKInterfaceModel`，是接口层的网格能力扩展。
- VTK 渲染相关的适配器实现在本模块中定义接口，但具体渲染逻辑由 `FITKComponent` 层（如 FITKRenderWindowVTK）实现。
- 分区接口（PartitionInterface）主要服务于并行计算场景，与网格剖分组件协同工作。

## 主要类与功能说明

### 网格接口与VTK实现

#### `FITKUnstructuredMeshVTK`

非结构化网格的 VTK 实现类，继承自 `FITKUnstructuredMesh`。该类是网格数据与 VTK 数据结构之间的桥梁，负责将 FITK 内部的网格表示转换为 VTK 的 `vtkUnstructuredGrid` 对象，以便进行可视化渲染。

主要功能包括：

- 节点管理：支持添加节点到 VTK 网格对象，包括普通节点和与几何关联的原生节点。
- 单元管理：支持添加各类非结构化单元（三角形、四边形、四面体、六面体、棱柱等）。
- 数据更新：提供 `update()` 方法用于刷新 VTK 渲染数据。
- 维度查询：提供 `getCellIndiceByDim()` 方法根据单元维度获取对应的单元索引列表。
- 孤立网格标记：支持为孤立网格（Orphan Mesh）设置标记数组，用于识别特殊网格单元。

#### `FITKStructuredMeshVTK`

结构化网格的 VTK 实现类（当前实现较简单）。结构化网格在科学计算中常用于有限差分方法，其特点是网格节点排列具有规则性。

#### `FITKUnstructuredFluidMeshVTK`

专门针对流体计算场景的非结构化网格 VTK 实现，在标准非结构化网格基础上增加了流体计算特有的数据管理能力。

#### `FITKMeshFeatureVTK`

网格特征信息的 VTK 相关功能封装，用于处理网格特征（如边界、锋面等）的渲染相关操作。

### 单元类型映射

#### `FITKMeshVTKMap`

FITK 内部单元类型与 VTK 单元类型之间的映射表。这是实现接口层与渲染层解耦的关键组件，通过 QHash 数据结构维护了完整的单元类型对应关系。

当前支持的映射单元类型包括：

- **顶点类**：Vertex1 → VTK_VERTEX
- **线单元**：Line2 → VTK_LINE、Line3 → VTK_QUADRATIC_EDGE
- **三角形单元**：Tri3 → VTK_TRIANGLE、Tri6 → VTK_QUADRATIC_TRIANGLE
- **四边形单元**：Quad4 → VTK_QUAD、Quad8 → VTK_QUADRATIC_QUAD
- **四面体单元**：Tet4 → VTK_TETRA、Tet10 → VTK_QUADRATIC_TETRA
- **棱柱单元**：Wedge6 → VTK_WEDGE、Wedge15 → VTK_QUADRATIC_WEDGE
- **六面体单元**：Hex8 → VTK_HEXAHEDRON、Hex20 → VTK_QUADRATIC_HEXAHEDRON
- **多边形单元**：Polygon → VTK_POLYGON

通过这种映射机制，上层业务代码只需要使用 FITK 内部的单元类型枚举（`FITKModelEnum::FITKEleType`），而无需关心具体的 VTK 实现细节。

### 分区接口管理

#### `FITKPartitionInterface`

分区交界面的数据结构，用于记录并行计算中相邻分区之间的界面信息。每个分区界面对象包含以下核心信息：

- 分区面索引（InterfaceIndex）：界面在分区管理器中的唯一标识。
- 两个分区的标识：分别记录界面两侧的分区编号（PartitionID1 和 PartitionID2）。
- 单元与面索引：记录界面所在的单元索引（CellIndex）和单元面索引（SurfaceIndex）。

该类的设计使得在并行计算中进行网格数据传递时，可以精确地定位到需要交互的网格单元和面。

#### `FITKPartitionInterfaceManager`

分区交界面的管理器，负责集中管理所有分区界面对象以及单元和节点的分区归属信息。

主要功能包括：

- 分区信息设置：通过 `setPartitionInfo()` 设置分区面两侧的分区编号关系。
- 单元分区查询：通过 `setCellsPartitionInfo()` 和 `getCellPartitionIndex()` 管理和查询单元所属分区。
- 节点分区查询：通过 `setNodesPartitionInfo()` 和 `getNodePartitionIndex()` 管理和查询节点所属分区。
- 邻居分区查询：通过 `getNeighborPartition()` 获取指定分区的所有邻居分区。
- 分区面查询：通过 `getEleFaceListByFaceIndex()` 获取指定分区面上的单元对列表。
- 分区数量管理：提供 `getPartitionNum()` 获取总分区数量。

分区接口管理器的存在，使得并行计算中的网格数据交换能够以统一的方式进行，上层并行算法只需要依赖分区接口管理器提供的查询接口，而无需了解底层的分区数据结构细节。

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `project(FITKInterfaceMesh VERSION 0.5.0 LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`
- 通过 `link_fitk_library(${PROJECT_NAME} FITKCore)` 链接底层模块 `FITKCore`
- 通过 `link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)` 链接接口层模块 `FITKInterfaceModel`
- 通过 VTKConfig.cmake 配置链接 VTK 库

### qmake

当前目录中同时保留了 `FITKInterfaceMesh.pro` 和 `FITKInterfaceMesh.pri` 文件，说明该模块需要同时兼容现有的多套工程组织方式。

### 在上层工程中的参与方式

FITKInterfaceMesh 作为接口层模块，一般不独立使用，而是由网格组件、渲染组件或并行计算组件进行链接和调用。典型的接入方式是由 `FITKComponent` 层的网格相关组件链接本模块，并在运行时通过工厂机制创建具体的 VTK 适配器实例。

## 使用示例

以下示例展示 FITKInterfaceMesh 的典型使用方式。

### 1. 非结构化网格创建与VTK映射

```cpp
#include "FITKUnstructuredMeshVTK.h"
#include "FITKMeshVTKMap.h"

// 创建非结构化网格VTK实例
Interface::FITKUnstructuredMeshVTK* mesh = new Interface::FITKUnstructuredMeshVTK();

// 添加节点
int nodeId1 = mesh->addNode(0.0, 0.0, 0.0);
int nodeId2 = mesh->addNode(1.0, 0.0, 0.0);
int nodeId3 = mesh->addNode(0.5, 1.0, 0.0);

// 添加三角形单元
QList<int> nodeIds;
nodeIds.append(nodeId1);
nodeIds.append(nodeId2);
nodeIds.append(nodeId3);
mesh->addElement(Interface::FITKModelEnum::FITKEleType::Tri3, nodeIds);

// 使用单元类型映射
int vtkCellType = Interface::eleTypeHash.value(Interface::FITKModelEnum::FITKEleType::Tri3);
// vtkCellType 现在为 VTK_TRIANGLE
```

### 2. 分区接口管理

```cpp
#include "FITKPartitionInterface.h"

// 创建分区接口管理器
FITKPartition::FITKPartitionInterfaceManager* partitionMgr = new FITKPartition::FITKPartitionInterfaceManager();

// 设置分区面邻居关系
QList<QSet<int>> faceNeighborRegion;
QSet<int> face1Neighbors;
face1Neighbors.insert(0);
face1Neighbors.insert(1);
faceNeighborRegion.append(face1Neighbors);
partitionMgr->setPartitionInfo(faceNeighborRegion);

// 设置单元分区信息
std::vector<int> cellPartitionInfo = {0, 0, 1, 1, 2};
partitionMgr->setCellsPartitionInfo(cellPartitionInfo);

// 查询单元所属分区
int partitionOfCell2 = partitionMgr->getCellPartitionIndex(2);
// partitionOfCell2 = 1

// 获取分区邻居
QList<int> neighborsOfPartition0 = partitionMgr->getNeighborPartition(0);
// neighborsOfPartition0 = {1}
```

### 3. 在上层组件中的集成

```cpp
// 在组件初始化时注册网格VTK工厂
void MyMeshComponent::init()
{
    // 注册非结构化网格VTK适配器
    registerMeshFactory<Interface::FITKUnstructuredMeshVTK>("UnstructuredMeshVTK");
    
    // 初始化分区管理器
    m_partitionManager = new FITKPartition::FITKPartitionInterfaceManager();
}

// 使用网格数据
void MyMeshComponent::updateMesh(Interface::FITKUnstructuredMesh* meshData)
{
    // 创建VTK实现
    Interface::FITKUnstructuredMeshVTK* vtkMesh = createVTKMesh(meshData);
    
    // 更新渲染数据
    vtkMesh->update();
}
```

## 使用建议

- 网格数据操作优先使用接口层的抽象类型（`FITKUnstructuredMesh` 等），而非直接操作 VTK 类型。
- 单元类型判断统一使用 `FITKModelEnum::FITKEleType` 枚举，避免直接使用 VTK 单元类型常量。
- 并行计算场景下，通过 `FITKPartitionInterfaceManager` 统一管理分区信息，不要在业务代码中直接操作分区数据结构。
- 如需扩展新的单元类型支持，需要同时在 `FITKMeshVTKMap` 中添加映射关系。

## 注意事项

- 当前模块是接口抽象层，不包含具体的网格生成算法或剖分逻辑。
- 分区接口设计针对的是 MPI 并行场景，不适用于共享内存并行。
- VTK 适配器的具体渲染性能取决于上层渲染组件的实现。

最后更新时间：2026-04-22
