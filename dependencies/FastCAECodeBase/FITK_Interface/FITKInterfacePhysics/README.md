# FITKInterfacePhysics - 物理属性与物理指派接口层

## 简介

FITKInterfacePhysics 是 FITK_Interface 中负责物理属性抽象和物理对象关联关系表达的接口模块。它面向前处理阶段的物理建模需求，提供材料、截面、材料指派、截面指派、边界位置、载荷、约束、场数据以及若干配套管理器，使上层组件可以在不依赖具体求解器格式的前提下组织物理信息。

这个模块并不是单一的大型接口，而是一组围绕“物理属性”和“物理施加位置”展开的数据对象集合。其中既包含抽象基类，也包含管理器和关系对象。上层组件通常基于这些抽象类派生出具体材料、截面、载荷/约束，然后把对象放入相应管理器，供模型树、属性面板和求解器转换链路共同使用。

FITKInterfacePhysics 强调“物理意义”和“模型关联”。它解决的是材料属于谁、截面施加到哪里、载荷与约束定位到哪个模型/组件、场数据以何种形式表达等问题。

## 模块定位

FITKInterfacePhysics 位于 `FITK_Interface` 抽象接口层，主要承担以下职责：

- 定义材料对象与材料管理器的基础抽象。
- 定义截面对象、截面附加信息和截面管理器的基础抽象。
- 定义材料指派、截面指派及批量分组指派对象。
- 定义模型与组件上的边界位置描述对象。
- 定义载荷与边界条件的抽象基类及管理器。
- 定义离散场和解析场的抽象表达方式。
- 为上层前处理组件提供一套与求解器无关的物理组织接口。

该模块以共性物理对象、属性关系和施加位置关系为核心，负责稳定承载材料、截面、载荷、约束与场数据的基础数据模型，并为上层组件提供统一的组织接口。

## 架构与目录

结合当前目录，可以把模块划分为以下几部分：

```text
FITKInterfacePhysics
├── 材料体系
│   ├── FITKAbstractMaterial
│   ├── FITKMaterialManager
│   └── FITKMaterialAssign / Manager
├── 截面体系
│   ├── FITKAbstractSectionInfo
│   ├── FITKAbstractSection
│   ├── FITKAbstractSectionManager
│   ├── FITKSectionAssign / Manager
│   └── FITKSectionAssignGroup / Manager
├── 定位与施加对象
│   ├── FITKBoundaryLocation
│   ├── FITKInteractionLocation
│   ├── FITKAbstractLoad
│   ├── FITKAbstractBC
│   ├── FITKLoadManager
│   └── FITKBCManager
└── 场与辅助对象
  ├── FITKAbstractFiled
  ├── FITKFieldsManager
  ├── FITKFieldDiscrete
  ├── FITKFieldAanlyticalExpress
  └── FITKModelWire / Manager
```

这个结构体现了两个核心思想：

- 物理属性本身与施加关系分离。
- 物理对象本身与管理器分离。

因此，上层业务既可以独立编辑材料/截面内容，也可以独立维护“属性指派到哪里”的关系对象，便于支持更复杂的模型树与装配流程。

## 依赖关系

### 编译依赖

根据 `FITK_Interface/FITKInterfacePhysics/CMakeLists.txt`，该模块当前显式依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- FITKInterfaceModel

### 依赖含义

- `FITKCore` 提供基础数据对象、命名对象以及数据管理模板，是本模块所有对象体系的根基。
- `FITKInterfaceModel` 提供模型、装配、组件等抽象对象，材料指派、截面指派和边界位置都依赖这些模型上下文才能成立。
- Qt 主要用于 `QObject`、`QString`、容器类型和枚举元信息注册。

### 模块边界

- 本模块聚焦共性物理对象、属性关系和位置关系的抽象，可作为不同求解器方向材料类、截面类和物理对象派生实现的公共基础。
- 本模块承载前处理阶段最基础的物理数据组织，工况、分析步和求解控制通常由更高层的物理建模模块继续扩展。
- 本模块向上层提供统一的数据对象与关联关系基础，便于后续的求解器格式转换、业务校核和界面接入。

## 主要类与功能说明

### 材料体系

#### `FITKAbstractMaterial`

这是材料对象的抽象基类，继承自 `Core::FITKAbstractNDataObject`。从头文件可确认，它提供：

- 材料内部 ID。
- 材料描述字符串。
- 获取与设置材料描述的接口。

需要特别注意的是，头文件注释明确说明其 `materialID` 与管理器内部使用的对象 ID 不是同一概念，也不应直接作为外部关联主键使用。也就是说，调用方更应依赖管理器中的数据对象 ID 和对象关系，而不是把这个内部字段当成全局唯一标识。

#### `FITKMaterialManager`

材料管理器通过 `FITKAbstractDataManager<FITKAbstractMaterial>` 管理材料对象集合。它自身较薄，但意义很明确：材料对象的生命周期和检索逻辑应统一收敛到管理器，而不是由各个 UI 或算子分别持有。

#### `FITKMaterialAssign`

材料指派对象负责建立“模型/集合”与“材料”之间的关系。当前头文件显示它至少提供：

- 获取模型 ID 与材料 ID。
- 执行 `assign(modelID, materialID)` 关联。
- 检查指派是否有效。
- 判断指派对象是否作用于模型部件。
- 获取被关联材料对象并做类型转换。

这说明模块已经把材料内容和材料作用范围拆开处理。上层可以先定义材料，再用独立对象描述材料被施加到哪些模型实体上。

#### `FITKMaterialAssignManager`

材料指派管理器负责集中保存全部材料指派对象，使模型检查、属性树显示和求解器导出都能从同一个地方读取材料关联关系。

### 截面体系

#### `FITKAbstractSectionInfo`

这是截面附加信息的抽象基类，当前头文件中有 `SectionType` 枚举，至少包含 `STNone` 和 `STNasBDF`。这意味着截面对象本身之外，还允许挂接一份类型化的附加信息，用来表达某类截面特有的参数集合。

#### `FITKAbstractSection`

这是截面对象的核心抽象。根据头文件，它提供的能力明显比材料对象更多，主要包括：

- 维护一个材料 ID 列表，可支持多材料截面场景。
- 根据索引设置材料 ID 或材料对象。
- 获取材料对象、材料名称、材料数量。
- 删除某个索引位置的材料。
- 提供截面类型字符串。
- 持有并设置 `FITKAbstractSectionInfo`。
- 暴露截面内部 ID。

从这些接口可以判断，模块设计者已经预留了比“单材料单截面”更复杂的截面关系表达能力，因此该抽象适合被梁、壳、实体或求解器专用截面继续派生。

#### `FITKAbstractSectionManager`

截面管理器负责集中保存截面对象集合，是截面编辑器、属性浏览器和导出器的共用数据入口。

#### `FITKSectionAssign`

截面指派对象的作用与材料指派类似，但它除了基本的模型 ID 和截面 ID 关系外，还提供了更多辅助查询能力，例如：

- `assign(modelID, sectionID, checkValid)` 建立关联。
- 获取截面名称。
- 获取截面类型字符串。
- 获取模型名称。
- 查询数据对象是否被当前指派使用。

这些接口说明截面指派不仅是一个简单的二元关系对象，还承担了一部分模型一致性检查与显示辅助职责。

#### `FITKSectionAssignManager`

用于集中管理截面指派对象，并支持按模型集合 ID 查询对应指派关系。这对模型集合驱动的属性施加流程尤其重要。

#### `FITKSectionAssignGroup` 与 `FITKSectionAssignGroupManager`

这组对象用于把多个截面指派组合管理。虽然当前 README 不展开其全部细节，但从命名和继承关系看，它更适合承载批量指派、分组组织或装配场景下的集中管理需求。

### 定位与施加对象

#### `FITKBoundaryLocation`

这是整个模块中非常关键的基础类。它统一描述物理对象施加的位置，包括：

- 模型 ID 或模型对象。
- 组件 ID 或组件对象。
- 位置有效性检查。
- 获取位置文本标签。

统一定位对象的好处是，载荷、约束以及其他需要“施加到某个模型/组件”的数据都可以共享同一套位置描述逻辑，避免不同物理对象各自实现一套近似但不一致的定位字段。

#### `FITKInteractionLocation`

从命名看，它用于比普通边界位置更复杂的交互关系表达，通常适用于两组对象之间的相互作用场景。当前目录中保留该类型，说明模块不仅面向单点施加，也为接触、连接等多对象关系预留了接口基础。

#### `FITKAbstractLoad` 与 `FITKAbstractBC`

这两个抽象基类都继承自 `FITKBoundaryLocation`，意味着载荷和边界条件天然带有模型/组件定位信息。它们负责为具体的力、位移、温度、转角等物理对象提供统一基座，使不同类型的载荷和约束都可以沿用一致的定位与管理方式。

#### `FITKLoadManager` 与 `FITKBCManager`

两个管理器分别用于集中管理载荷对象和边界条件对象，使工况组织与模型检查可以按统一接口处理不同类型的施加对象。

### 场与辅助对象

#### `FITKAbstractFiled`

这是场数据的抽象基类。头文件中定义了 `FITKFieldType` 枚举，当前支持：

- `FFTDiscrete` 离散场。
- `FFTAnalytical` 解析场。

它还提供描述字符串和深拷贝入口，说明场对象会被不同步骤或不同模型副本复用。

#### `FITKFieldsManager`

场管理器可集中保存多种场对象，并支持按场类型筛选。对于温度场、初始应力场、预定义场等场景，这种管理模式可以减少各类场对象散落在业务代码中的问题。

#### `FITKFieldDiscrete`

离散场对象是当前目录中最清晰可见的具体场实现。根据头文件，它支持：

- 节点场或单元场位置枚举。
- 默认值开关与默认值设置。
- 多条离散数据的添加、插入、删除与清空。
- 按索引读取场数据条目。

因此它适合表示“局部节点/单元上给定具体数值”的场数据，例如节点温度、单元初始应力或其他离散属性分布。

#### `FITKFieldAanlyticalExpress`

该类表示解析表达式场，用于用公式而不是离散点表来描述场分布。它通常适合规则分布、连续表达或需要在后续求值时动态计算的场。

#### `FITKModelWire` 与 `FITKModelWireManager`

这组对象用于表达模型中的线特征或线型辅助物理信息。

## 构建与集成

### CMake

当前模块通过 `add_library(${PROJECT_NAME} SHARED)` 构建为共享库，并启用了：

- `CMAKE_AUTOMOC`
- `CMAKE_AUTORCC`
- `CMAKE_AUTOUIC`

输出目录配置为：

- Debug: `output/bin_d`
- Release: `output/bin`

内部库链接通过本目录 `CMakeLists.txt` 中的 `link_fitk_library` 宏完成，说明该模块按应用总体工程统一输出到公共目录，供上层组件动态链接或静态引用。

### qmake

目录中同时存在 `.pro` 与 `.pri` 文件，说明本模块仍需兼容历史 qmake 工程组织，与现有 CMake 流程并存。

### 典型接入方式

上层组件通常按以下顺序使用本模块：

1. 创建具体材料和截面派生类对象并加入对应管理器。
2. 通过材料指派和截面指派对象，把属性与模型或集合关联起来。
3. 创建具体载荷或约束对象，并通过 `FITKBoundaryLocation` 设置施加位置。
4. 根据需要创建离散场或解析场对象，加入场管理器。
5. 在求解器转换阶段，从这些管理器和关系对象中提取统一物理数据。

## 使用示例

以下示例展示当前接口层的典型使用方式。

### 1. 建立截面指派关系

```cpp
#include "FITK_Interface/FITKInterfacePhysics/FITKSectionAssign.h"

void assignSectionToModel(int modelId, int sectionId)
{
  Interface::FITKSectionAssign assign;
  assign.assign(modelId, sectionId, true);

  if (assign.isValid() == false)
  {
    return;
  }

  const int sid = assign.getSectionID();
  Q_UNUSED(sid);
}
```

### 2. 建立材料指派关系

```cpp
#include "FITK_Interface/FITKInterfacePhysics/FITKMaterialAssign.h"

void assignMaterialToModel(int modelId, int materialId)
{
  Interface::FITKMaterialAssign assign;
  assign.assign(modelId, materialId);

  if (assign.isValid() == false)
  {
    return;
  }
}
```

### 3. 设置边界施加位置

```cpp
#include "FITK_Interface/FITKInterfacePhysics/FITKBoundaryLocation.h"

void setupBoundaryLocation(Interface::FITKBoundaryLocation& location, int modelId, int componentId)
{
  location.setModel(modelId);
  location.setComponent(componentId);

  if (location.isValid() == false)
  {
    return;
  }
}
```

### 4. 创建离散场

```cpp
#include "FITK_Interface/FITKInterfacePhysics/FITKFieldDiscrete.h"

void buildDiscreteField()
{
  Interface::FITKFieldDiscrete field;
  field.setDescribe("Temperature");
  field.setDisFieldLoc(Interface::FITKFieldDiscrete::FDFLNode);
  field.hasDefaultValue(true);
  field.setDefaultValue(20.0);
  field.appendFieldData(1001, 23.5);
  field.appendFieldData(1002, 24.1);
}
```

## 使用建议

- 材料、截面、载荷、边界条件都应优先通过管理器集中保存，不建议让界面层长期持有裸对象列表。
- 使用 `FITKMaterialAssign` 和 `FITKSectionAssign` 时，应确保被引用的模型对象和属性对象已经存在于统一的数据上下文中。
- 边界、载荷等需要定位的对象，建议统一复用 `FITKBoundaryLocation` 逻辑，避免重复维护模型 ID 与组件 ID。
- 场数据如果具有明确的节点/单元离散值，优先使用 `FITKFieldDiscrete`；如果是公式驱动场，更适合使用解析场对象。
- 由于 `FITKAbstractMaterial` 和 `FITKAbstractSection` 都是抽象基类，真正的业务模块应通过派生类补充求解器或学科特有字段。

## 注意事项

- `FITKAbstractMaterial`、`FITKAbstractSection`、`FITKAbstractLoad`、`FITKAbstractBC`、`FITKAbstractFiled` 均为抽象基类，不能直接承担完整业务数据。
- 材料和截面的内部 ID 字段不应被误用为管理器对象 ID 或跨模块统一主键。
- 指派对象的有效性依赖模型对象和物理属性对象的外部上下文，若上下文缺失，`isValid()` 可能返回 false。
- 当前模块目录下没有更细的独立设计 README，因此本说明主要依据头文件和 CMake 配置整理；对求解器专用派生类的约束未在此展开。
- 若上层模块需要批量处理指派关系，建议优先复用现有 Group/Manager 体系，而不是重新定义一套并行容器。

最后更新时间：2026-04-22
