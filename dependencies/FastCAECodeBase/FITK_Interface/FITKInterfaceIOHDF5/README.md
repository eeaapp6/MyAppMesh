# FITKInterfaceIOHDF5 - HDF5数据读写接口适配器层

## 简介

FITKInterfaceIOHDF5 是 FITK_Interface 抽象接口层中专门负责 HDF5 文件格式数据读写的模块，提供了面向多种业务数据类型（网格、材料、边界条件、装配、坐标系等）的 HDF5 适配器接口。该模块基于 FITK 的适配器工厂机制，为上层业务组件提供统一的 HDF5 数据持久化能力，使得不同数据类型在 HDF5 文件中的存储格式能够保持一致性。

FITKInterfaceIOHDF5 的核心价值在于“封装 HDF5 操作细节，提供统一的数据序列化接口”。上层业务代码只需要了解抽象的适配器接口，而无需关心 HDF5 库的底层调用细节。同时，通过适配器工厂机制，可以方便地注册和获取不同数据类型的 HDF5 读写器。

## 模块定位

FITKInterfaceIOHDF5 位于 `FITK_Interface` 下，属于抽象接口层的 IO 适配子模块，主要承担以下职责：

- 定义 HDF5 文件读写适配器的抽象基类（`FITKAbstractHDF5Adaptor`）。
- 提供各类业务数据的 HDF5 读写适配器实现，包括网格、材料、边界条件、装配、坐标系等。
- 封装 HDF5 属性（Attribute）和数据集（Dataset）的读写接口。
- 与 FITK 的 IO 工厂机制（`FITKIOAdaptorFactory`）协同工作，支持运行时动态创建适配器。

从架构层级来看，FITKInterfaceIOHDF5 位于 FITKCore 基础对象层和 FITKAdaptor 适配器抽象层之上，是 IO 能力在 HDF5 文件格式上的具体实现。

## 架构与目录

从当前目录中的头文件和源文件看，FITKInterfaceIOHDF5 可以概括为以下几组能力：

```text
FITKInterfaceIOHDF5
├── 适配器基类
│   ├── FITKAbstractHDF5Adaptor
│   └── FITKAbstractHDF5AdaptorMesh
├── 业务数据适配器
│   ├── FITKInterfaceHDF5AdaptorMesh
│   ├── FITKInterfaceHDF5AdaptorMaterial
│   ├── FITKInterfaceHDF5AdaptorMaterialAssign
│   ├── FITKInterfaceHDF5AdaptorSection
│   ├── FITKInterfaceHDF5AdaptorSectionAssign
│   ├── FITKInterfaceHDF5AdaptorBoundaryLocation
│   ├── FITKInterfaceHDF5AdaptorCoordinateSystem
│   ├── FITKInterfaceHDF5AdaptorSet
│   ├── FITKInterfaceHDF5AdaptorSurface
│   ├── FITKInterfaceHDF5AdaptorFiled
│   ├── FITKInterfaceHDF5AdaptorAssembly
│   └── FITKInterfaceHDF5AdaptorAssElement
└── API导出
    └── FITKInterfaceIOHDF5API
```

## 依赖关系

### 编译依赖

根据 `FITK_Interface/FITKInterfaceIOHDF5/CMakeLists.txt`，该模块当前显式依赖：

- Qt5::Core
- HDF5 库（通过 hdf5.pri 配置）
- FITKCore（FITKAbstractDataObject、FITKAbstractIOAdaptor）
- FITKAdaptor（FITKIOAdaptorFactory）

### 模块层级关系

- FITKInterfaceIOHDF5 直接依赖 `FITKCore` 和 `FITKAdaptor`，是接口层的 HDF5 IO 能力扩展。
- 上层组件（如物理组件、装配组件）通过适配器工厂获取 HDF5 读写适配器实例，实现数据的持久化。
- HDF5 库作为底层依赖，被封装在适配器实现内部，上层业务代码无需直接链接。

## 主要类与功能说明

### 适配器基类

#### `FITKAbstractHDF5Adaptor`

HDF5 文件读写适配器的抽象基类，继承自 `FITKAbstractIOAdaptor`。该类封装了 HDF5 操作的基本接口，为具体业务适配器提供通用的读写框架。

主要功能包括：

- 读写器管理：提供 `setFileReader()` 和 `setFileWriter()` 方法设置 HDF5 读写器。
- H5 组管理：提供 `setH5GroupData()` 方法设置当前操作的 HDF5 组对象。
- 属性读写：封装了多种数据类型的属性读写接口，包括字符串、布尔值、双精度浮点数、整数等。
- 数据集读写：支持读写双精度浮点类型的数据集（`readDoubleDataSet()`）。

通过这些通用接口，具体业务适配器可以专注于实现其特有的数据读写逻辑，而无需重复实现 HDF5 的底层调用代码。

#### `FITKAbstractHDF5AdaptorMesh`

网格数据 HDF5 适配器的抽象基类，继承自 `FITKAbstractHDF5Adaptor`。它定义了网格数据 HDF5 读写的标准接口框架，包括节点读取、单元读取、节点写出和单元写出等方法。

该类的存在使得不同网格类型的 HDF5 适配器可以遵循统一的接口规范，便于扩展和维护。

### 业务数据适配器

#### `FITKInterfaceHDF5AdaptorMesh`

非结构化网格的 HDF5 读写适配器，负责将 `FITKUnstructuredMesh` 对象序列化到 HDF5 文件或从 HDF5 文件反序列化。

主要功能包括：

- 节点读写：通过 `readNode()` 和 `writeNode()` 方法读写网格节点数据。
- 单元读写：通过 `readElement()` 和 `writeElement()` 方法读写网格单元数据。
- 数据转换：将 FITK 内部的网格表示转换为 HDF5 兼容的数据格式。

该适配器是网格数据持久化的核心组件，支持网格数据在 HDF5 文件中的长期存储和跨平台交换。

#### `FITKInterfaceHDF5AdaptorMaterial`

材料数据的 HDF5 读写适配器，负责读写材料属性定义。材料数据在 CAE 仿真中定义了物理模型的材料力学、热学等属性。

#### `FITKInterfaceHDF5AdaptorMaterialAssign`

材料分配的 HDF5 读写适配器，负责读写材料分配信息。在复杂模型中，材料需要分配到特定的几何实体或网格区域。

#### `FITKInterfaceHDF5AdaptorSection`

截面属性的 HDF5 读写适配器，负责读写梁单元、壳单元等的截面属性定义。

#### `FITKInterfaceHDF5AdaptorSectionAssign`

截面分配的 HDF5 读写适配器，负责读写截面属性分配到具体模型元素的信息。

#### `FITKInterfaceHDF5AdaptorBoundaryLocation`

边界条件定位的 HDF5 读写适配器，负责读写边界条件的作用位置信息。边界条件包括位移约束、力载荷、压力载荷等多种类型。

#### `FITKInterfaceHDF5AdaptorCoordinateSystem`

坐标系的 HDF5 读写适配器，负责读写各种坐标系定义（如笛卡尔坐标系、柱坐标系、球坐标系等）。

#### `FITKInterfaceHDF5AdaptorSet`

集合的 HDF5 读写适配器，负责读写网格或几何的集合定义。集合是 CAE 软件中组织和管理模型元素的常用机制。

#### `FITKInterfaceHDF5AdaptorSurface`

曲面数据的 HDF5 读写适配器，负责读写曲面相关的几何或物理数据。

#### `FITKInterfaceHDF5AdaptorFiled`

场数据的 HDF5 读写适配器，负责读写标量场或矢量场数据。场数据广泛用于结果后处理和物理量可视化。

#### `FITKInterfaceHDF5AdaptorAssembly`

装配数据的 HDF5 读写适配器，负责读写装配层级结构和装配实例信息。

#### `FITKInterfaceHDF5AdaptorAssElement`

装配单元的 HDF5 读写适配器，负责读写装配中的连接单元或约束单元数据。

### API导出

#### `FITKInterfaceIOHDF5API`

接口导出宏定义文件，通过 Qt 的 `Q_DECL_EXPORT` 和 `Q_DECL_IMPORT` 宏控制动态库的导入导出。

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `project(FITKInterfaceIOHDF5 VERSION 1.0.0 LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`
- 通过 `link_fitk_library(${PROJECT_NAME} FITKCore)` 链接底层模块 `FITKCore`
- 通过 `link_fitk_library(${PROJECT_NAME} FITKAdaptor)` 链接适配器模块 `FITKAdaptor`
- 通过 HDF5Config.cmake 配置链接 HDF5 库

### qmake

当前目录中同时保留了 `FITKInterfaceIOHDF5.pro` 和 `FITKInterfaceIOHDF5.pri` 文件，说明该模块需要同时兼容现有的多套工程组织方式。

### 在上层工程中的参与方式

FITKInterfaceIOHDF5 作为接口层的 IO 适配模块，一般不独立使用，而是由数据管理组件或文件 IO 组件进行链接和调用。典型的接入方式是通过适配器工厂注册 HDF5 适配器，然后在数据保存或加载时通过工厂获取对应的适配器实例。

## 使用示例

以下示例展示 FITKInterfaceIOHDF5 的典型使用方式。

### 1. 通过适配器工厂获取HDF5适配器

```cpp
#include "FITKIOAdaptorFactory.h"
#include "FITKInterfaceHDF5AdaptorMesh.h"

// 通过工厂获取网格HDF5适配器
Adaptor::FITKIOAdaptorFactory* factory = Adaptor::FITKIOAdaptorFactory::getInstance();
IO::FITKInterfaceHDF5AdaptorMesh* meshAdaptor = factory->getAdaptorT<IO::FITKInterfaceHDF5AdaptorMesh>();
```

### 2. 使用适配器进行数据读写

```cpp
#include "FITKInterfaceHDF5AdaptorMesh.h"
#include "FITKUnstructuredMesh.h"

// 创建适配器并设置数据
IO::FITKInterfaceHDF5AdaptorMesh adaptor;
adaptor.setDataObject(meshData);

// 设置HDF5组
H5::Group h5Group = ...;
adaptor.setH5GroupData(h5Group);

// 执行读取
if (adaptor.adaptR())
{
    // 读取成功
    Interface::FITKUnstructuredMesh* loadedMesh = adaptor.getDataObject();
}
```

### 3. 自定义HDF5适配器实现

```cpp
#include "FITKAbstractHDF5Adaptor.h"

class MyHDF5Adaptor : public IO::FITKAbstractHDF5Adaptor
{
public:
    explicit MyHDF5Adaptor() = default;
    virtual ~MyHDF5Adaptor() = default;

    QString getAdaptorClass() override
    {
        return "MyData";
    }

protected:
    bool readData() override
    {
        // 读取HDF5属性
        std::string name;
        if (readStrAttribute(*_h5Group, "Name", name))
        {
            m_data->setName(QString::fromStdString(name));
        }
        
        // 读取HDF5数据集
        QList<double> coords;
        if (readDoubleDataSet(*_h5Group, "Coordinates", coords))
        {
            m_data->setCoordinates(coords);
        }
        
        return true;
    }

    bool writeData() override
    {
        // 写出数据到HDF5
        H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
        H5::StrType datatype = H5::StrType(H5T_C_S1, m_data->getName().length() + 1);
        h5Group->createAttribute("Name", datatype, dataspace)
               .write(datatype, m_data->getName().toStdString().c_str());
               
        return true;
    }

private:
    MyDataObject* m_data = nullptr;
};
```

### 4. 在上层组件中的集成

```cpp
void MyDataComponent::saveToHDF5(const QString& filePath)
{
    // 创建HDF5文件
    H5::H5File h5File(filePath.toStdString(), H5F_ACC_TRUNC);
    
    // 创建根组
    H5::Group rootGroup = h5File.createGroup("/Data");
    
    // 通过工厂获取各类适配器
    auto factory = Adaptor::FITKIOAdaptorFactory::getInstance();
    
    // 保存网格
    auto meshAdaptor = factory->getAdaptorT<IO::FITKInterfaceHDF5AdaptorMesh>();
    meshAdaptor->setDataObject(m_meshData);
    meshAdaptor->setH5GroupData(rootGroup.createGroup("Mesh"));
    meshAdaptor->adaptW();
    
    // 保存材料
    auto materialAdaptor = factory->getAdaptorT<IO::FITKInterfaceHDF5AdaptorMaterial>();
    materialAdaptor->setDataObject(m_materialData);
    materialAdaptor->setH5GroupData(rootGroup.createGroup("Material"));
    materialAdaptor->adaptW();
}
```

## 使用建议

- 通过适配器工厂（`FITKIOAdaptorFactory`）获取适配器实例，不要直接实例化具体适配器类。
- 在读写数据前，确保 H5 组对象已正确设置，使用 `setH5GroupData()` 方法。
- 自定义适配器应继承 `FITKAbstractHDF5Adaptor`，并实现 `adaptR()` 和 `adaptW()` 方法。
- 属性读写优先使用基类提供的封装方法（如 `readStrAttribute()`、`readDoubleDataSet()` 等），以保持与 HDF5 文件格式的一致性。

## 注意事项

- HDF5 库需要正确初始化和关闭，确保在合适的生命周期内使用适配器。
- 适配器的 `setDataObject()` 方法需要在 `adaptR()` 或 `adaptW()` 调用前执行。
- 当前模块只负责数据的序列化和反序列化，不涉及 HDF5 文件的创建和管理。
- 并行 IO 场景下，需要考虑 HDF5 的并行特性，可能需要使用专门的并行 HDF5 适配器。

最后更新时间：2026-04-22
