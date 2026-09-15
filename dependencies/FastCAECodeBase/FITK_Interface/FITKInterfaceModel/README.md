# FITKInterfaceModel - 模型数据接口模块

## 项目简介

FITKInterfaceModel 是 FITK 的模型数据接口层，提供网格模型、几何模型、装配实例、模型组件与单元类型等基础数据结构。

该模块的定位是"数据模型与基础能力"，为上层组件（如几何内核、网格生成器、后处理器）提供统一的数据抽象接口。该模块不直接等同于完整求解器或前后处理业务流程实现，而是提供底层数据结构支持。

模块采用分层设计，从抽象基类到具体实现，从节点/单元到装配结构，形成完整的模型数据体系。

## 架构与目录

```
FITKInterfaceModel/
├── 模型基类
│   ├── FITKAbstractModel.h              # 模型抽象基类
│   ├── FITKAbstractMesh.h              # 网格抽象基类
│   ├── FITKAbstractGeoModel.h          # 几何模型抽象基类
│   ├── FITKAbstractPart.h              # 部件抽象基类
│   └── FITKModelEnum.h                 # 模型枚举定义
├── 网格模型
│   ├── FITKUnstructuredMesh.h          # 非结构化网格
│   ├── FITKStructuredMesh.h           # 结构化网格
│   ├── FITKParticalMesh.h             # 粒子网格
│   ├── FITKNodeList.h                 # 节点列表
│   └── FITKElementList.h              # 单元列表
├── 单元类型
│   ├── FITKAbstractElement.h          # 单元抽象基类
│   ├── FITKElementVertex.h            # 点单元
│   ├── FITKElementLine.h              # 线单元
│   ├── FITKElementTri.h               # 三角形单元
│   ├── FITKElementQuad.h              # 四边形单元
│   ├── FITKElementTet.h               # 四面体单元
│   ├── FITKElementHex.h               # 六面体单元
│   ├── FITKElementWedge.h             # 楔形体单元
│   ├── FITKElementPolygon.h          # 多边形单元
│   └── FITKElementFactory.h          # 单元工厂
├── 装配与实例
│   ├── FITKAssembly.h                # 装配体
│   ├── FITKAbstractAssInstance.h      # 装配实例抽象基类
│   └── FITKAssElement.h              # 装配元素
├── 组件与集合
│   ├── FITKComponentManager.h        # 组件管理器
│   ├── FITKComponentCreator.h        # 组件创建器
│   ├── FITKAbstractModelComponent.h  # 模型组件抽象
│   └── FITKModelSet.h               # 集合组件
├── 表面与映射
│   ├── FITKModelSurface.h           # 模型表面
│   ├── FITKGeometrySurface.h         # 几何表面
│   └── FITKGeoToMeshMapper.h        # 几何到网格映射
├── 算法与工具
│   ├── FITKAbsAlgorithmTools.h      # 算法工具抽象
│   ├── FITKAbsInterfaceFactory.h    # 接口工厂抽象
│   └── FITKUnstructuredMeshTopo.h   # 非结构网格拓扑
├── 后处理数据
│   └── FITKPostFieldsData.h         # 后处理场数据
├── 其他
│   ├── FITKCoordinateSystem.h       # 坐标系
│   └── FITKInterfaceModelAPI.h     # API宏定义
└── CMakeLists.txt
```

## 核心功能

### 1. 模型基类体系

#### FITKAbstractModel

所有模型对象的抽象基类：

- **模型类型获取**：getAbsModelType()
- **集合有效性判断**：isComponentValid()
- **文件读写**：writeToFile()
- **组件管理器获取**：getComponentManager()

#### FITKModelEnum

模型类型、网格维度、集合类型、单元类型等统一枚举定义：

- **AbsModelType**：模型类型（Mesh、GeoModel、Assembly）
- **FITKMeshDim**：网格维度（1D、2D、3D）
- **FITKModelSetType**：集合类型
- **FITKEleType**：单元类型

### 2. 网格模型体系

#### FITKUnstructuredMesh

非结构化网格模型（继承自 FITKAbstractMesh、FITKNodeList、FITKElementList）：

- 网格维度获取（getMeshDim）
- 网格质量检查
- 拓扑构建（buildTopo）
- 几何映射

#### FITKStructuredMesh

结构化网格模型：

- I/J/K 维度设置与访问
- 结构化数据存储

#### FITKParticalMesh

粒子网格模型类型。

### 3. 节点与单元管理

#### FITKNodeList

节点容器，支持：

- 节点增删（addNode、removeNode）
- 按索引/ID访问（getNodeAt、getNodeByID）
- ID-索引映射维护

#### FITKElementList

单元容器，支持：

- 单元增删（addElement、removeElement）
- 按索引/ID访问（getElementAt、getElementByID）
- 邻接查询
- 节点替换

#### 单元类型

| 单元类型 | 说明 |
|---------|------|
| FITKElementVertex | 点单元 |
| FITKElementLine | 线单元（杆、梁） |
| FITKElementTri | 三角形单元（2D/3D壳） |
| FITKElementQuad | 四边形单元（2D/3D壳） |
| FITKElementTet | 四面体单元 |
| FITKElementHex | 六面体单元 |
| FITKElementWedge |楔形体单元 |
| FITKElementPolygon | 多边形单元 |

#### FITKElementFactory

单元工厂，根据 FITKModelEnum::FITKEleType 创建单元对象。

### 4. 装配与实例

#### FITKAssembly

装配模型（继承自 FITKAbstractModel 与 FITKAbstractDataManager）：

- 管理多个装配实例
- **getInstanceTAt<T>()**：模板方法获取并转换实例
- **removeInstanceByModel()**：按模型ID移除实例

#### FITKAbstractAssInstance

抽象装配实例：

- 实例构造（支持指定模型ID、平移、旋转参数）
- 变换操作（rotate、setTransform）
- 变换工具获取（getInternalTransformTool）

### 5. 组件与集合

#### FITKComponentManager

模型组件管理器，负责管理集合和表面。

#### FITKModelSet

集合组件：

- 成员管理（addMember、removeMember）
- 集合类型管理
- 复合集合展开

### 6. 几何与映射

#### FITKAbstractGeoModel

几何模型抽象基类：

- 形状计数（getShapeCount）
- 边界盒（getBoundaryBox）
- 三角网格获取
- STL写出接口

#### FITKGeoToMeshMapper

几何到网格的映射关系能力。

### 7. 其他基础能力

#### FITKCoordinateSystem

坐标系数据，支持多种坐标系类型定义。

#### FITKPostFieldsData

后处理场数据对象。

## 关键类

### 模型基类

| 类名 | 说明 |
|------|------|
| FITKAbstractModel | 所有模型对象的抽象基类 |
| FITKAbstractMesh | 抽象网格模型基类 |
| FITKAbstractGeoModel | 几何模型抽象基类 |
| FITKModelEnum | 模型枚举定义 |

### 网格模型

| 类名 | 说明 |
|------|------|
| FITKUnstructuredMesh | 非结构化网格模型 |
| FITKStructuredMesh | 结构化网格模型 |
| FITKNodeList | 节点容器 |
| FITKElementList | 单元容器 |

### 单元类型

| 类名 | 说明 |
|------|------|
| FITKElementFactory | 单元工厂 |
| FITKElementVertex | 点单元 |
| FITKElementLine | 线单元 |
| FITKElementTri | 三角形单元 |
| FITKElementQuad | 四边形单元 |
| FITKElementTet | 四面体单元 |
| FITKElementHex | 六面体单元 |

### 装配与组件

| 类名 | 说明 |
|------|------|
| FITKAssembly | 装配体 |
| FITKAbstractAssInstance | 装配实例抽象基类 |
| FITKComponentManager | 组件管理器 |
| FITKModelSet | 集合组件 |

## 使用示例

### 示例1：创建非结构化网格

```cpp
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"

void createMesh()
{
    auto* mesh = new Interface::FITKUnstructuredMesh();

    // 添加节点
    mesh->addNode(0.0, 0.0, 0.0);
    mesh->addNode(1.0, 0.0, 0.0);
    mesh->addNode(0.5, 1.0, 0.0);

    // 添加单元
    auto* elem = Interface::FITKElementFactory::createElement(
        Interface::FITKModelEnum::FITKEleType::Tri3);
    if (elem) {
        elem->setNodeList({0, 1, 2});
        mesh->addElement(elem);
    }

    // 更新网格
    mesh->update();
}
```

### 示例2：管理装配实例

```cpp
#include "FITK_Interface/FITKInterfaceModel/FITKAssembly.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractAssInstance.h"

void manageInstances(Interface::FITKAssembly* assembly)
{
    if (assembly == nullptr) return;

    // 获取所有实例数量
    int instanceCount = assembly->getDataCount();

    // 获取指定类型实例
    auto* instance = assembly->getInstanceTAt<Interface::FITKAbstractAssInstance>(0);

    // 按模型ID移除实例
    assembly->removeInstanceByModel(modelID);
}
```

### 示例3：访问集合成员

```cpp
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"

void accessSet(Interface::FITKModelSet* set)
{
    if (set == nullptr) return;

    // 获取所有成员
    auto members = set->getMembers();

    // 添加成员
    set->addMember(10);
    set->addMember(20);

    // 获取集合类型
    auto type = set->getSetType();
}
```

## 依赖关系

### 外部依赖
- Qt5：Core、Gui、Widgets

### 内部依赖
- FITK_Kernel/FITKCore

## 注意事项

1. 非结构化网格同时继承 FITKNodeList 和 FITKElementList，节点和单元共享ID空间
2. 单元创建应通过 FITKElementFactory 工厂，避免直接实例化具体单元类
3. 装配实例的坐标变换使用局部/全局坐标系统，需要注意坐标系转换
4. 组件管理器负责维护集合和表面的层级关系，使用时需注意线程安全

---

最后更新时间：2026-04-21