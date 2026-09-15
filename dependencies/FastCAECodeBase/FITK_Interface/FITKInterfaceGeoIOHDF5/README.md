# FITKInterfaceGeoIOHDF5 - 几何数据HDF5读写接口适配器层

## 简介

FITKInterfaceGeoIOHDF5 是 FITK_Interface 抽象接口层中专门负责几何数据 HDF5 文件读写的模块，提供了面向几何建模各类操作（基础体素、曲线曲面、布尔运算、变换、分割等）的 HDF5 适配器接口。该模块是 FITKInterfaceIOHDF5 在几何领域的具体扩展，专注于几何数据的序列化和反序列化。

FITKInterfaceGeoIOHDF5 的核心价值在于"为几何建模全流程提供标准化的数据持久化能力"。通过定义统一的 HDF5 关键字和适配器接口，该模块确保几何操作序列（如拉伸、旋转、布尔、分割等）的参数能够被完整保存和恢复。

## 模块定位

FITKInterfaceGeoIOHDF5 位于 `FITK_Interface` 下，属于几何领域专用的 HDF5 IO 适配子模块，主要承担以下职责：

- 定义几何数据 HDF5 读写的关键字常量（`FITKGeoHDF5KeyWords`）。
- 提供几何命令的抽象适配器基类（`FITKAdaptorGeoCommand`）。
- 实现各类几何操作的 HDF5 读写适配器，包括基础体素、曲线曲面、变换、分割、布尔等。

## 架构与目录

```text
FITKInterfaceGeoIOHDF5
├── 适配器基类
│   ├── FITKAbstractGeoHDF5Adaptor
│   └── FITKAdaptorGeoCommand
├── 基准点适配器
│   ├── FITKAdaptorGeoDatum
│   └── FITKAdaptorGeoDatumModel
├── 基础几何适配器
│   ├── FITKAdaptorGeoModelPoint
│   ├── FITKAdaptorGeoModelCurve
│   ├── FITKAdaptorGeoModelBox
│   ├── FITKAdaptorGeoModelCylinder
│   └── FITKAdaptorGeoModelSphere
├── 操作几何适配器
│   ├── FITKAdaptorGeoModelTransform
│   ├── FITKAdaptorGeoModelPartition
│   └── FITKAdaptorGeoModelSplitter
├── 曲线曲面适配器
│   ├── FITKAdaptorGeoModelSketch
│   └── FITKAdaptorGeoModelThreePointsCurve
├── 变换适配器
│   ├── FITKAdaptorGeoModel
│   └── FITKAdaptorGeoModelBool
└── 关键字定义
    └── FITKGeoHDF5KeyWords
```

## 依赖关系

### 编译依赖

- Qt5::Core
- HDF5 库
- FITKCore（FITKAbstractDataObject、FITKAbstractIOAdaptor）
- FITKAdaptor（FITKIOAdaptorFactory）

### 模块层级关系

- FITKInterfaceGeoIOHDF5 直接依赖 `FITKCore` 和 `FITKAdaptor`，并在 `FITKInterfaceIOHDF5` 的 HDF5 适配器框架上构建。
- 几何组件（如 FITKComponentGeometry）依赖本模块实现几何数据的持久化。
- 本模块定义的关键字（`FITKGeoHDF5KeyWords`）被多个组件共享，确保 HDF5 文件格式的一致性。

## 主要类与功能说明

### 适配器基类

#### `FITKAbstractGeoHDF5Adaptor`

几何数据 HDF5 适配器的抽象基类，继承自 `FITKAbstractIOAdaptor`。该类在通用 HDF5 适配器基础上增加了几何领域特有的读写器和组管理接口。

#### `FITKAdaptorGeoCommand`

几何命令的抽象适配器基类，继承自 `FITKAbstractGeoHDF5Adaptor`。几何命令是几何建模操作的基本单元，如创建拉伸体、布尔运算等。

主要功能包括：

- 参数写入：提供 `writeParameters()` 虚函数，由具体适配器实现参数到 HDF5 的序列化。
- 参数读取：提供 `readParameters()` 虚函数，由具体适配器实现 HDF5 到参数的逆序列化。

### 关键字定义

#### `FITKGeoHDF5KeyWords`

几何数据 HDF5 读写的关键字常量定义文件。该文件定义了 HDF5 文件中几何数据存储所使用的标准关键字，包括：

- **数据对象关键字**：DataObjectID、DataObjectName、DataObjectEnable、DataObjectParentID 等。
- **几何命令关键字**：GeometoryCommandType、CommandStatus、EnableReferenceList 等。
- **参数关键字**：Point、Length、Location、Radius、Direction、Distance、Angle 等。
- **变换关键字**：Vector、SPoint、EPoint、Factors、BasePoint 等。
- **布尔操作关键字**：Target、ToolArray 等。
- **基准点关键字**：Origin、XDirection、YDirection、ZDirection 等。
- **虚拓扑关键字**：vTopoShapeManager、_ChildVTopo、VirtualTopoID 等。

### 基准点适配器

#### `FITKAdaptorGeoDatum`

基准点（Datum）的 HDF5 读写适配器。基准点是几何建模中的参考元素，用于定义坐标系、测量基准等。

#### `FITKAdaptorGeoDatumModel`

基准点模型的 HDF5 读写适配器，负责读写基准点模型及其关联的参数信息。

### 基础几何适配器

#### `FITKAdaptorGeoModelPoint`

点几何的 HDF5 读写适配器，负责读写点的坐标和参数信息。

#### `FITKAdaptorGeoModelCurve`

曲线几何的 HDF5 读写适配器，负责读写曲线（如直线、圆弧、样条曲线等）的定义参数。

#### `FITKAdaptorGeoModelBox`

长方体的 HDF5 读写适配器，负责读写长方体的尺寸参数（长、宽、高）和位置信息。

#### `FITKAdaptorGeoModelCylinder`

圆柱体的 HDF5 读写适配器，负责读写圆柱体的尺寸参数（半径、高度）和位置信息。

#### `FITKAdaptorGeoModelSphere`

球体的 HDF5 读写适配器，负责读写球体的尺寸参数（半径）和位置信息。

### 操作几何适配器

#### `FITKAdaptorGeoModelTransform`

几何变换的 HDF5 读写适配器，负责读写平移、旋转、缩放等几何变换操作的参数。

#### `FITKAdaptorGeoModelBool`

布尔运算的 HDF5 读写适配器，负责读写布尔运算（合并、剪切、相交等）的目标几何和工具几何信息。

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `project(FITKInterfaceGeoIOHDF5 LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`
- 通过 `link_fitk_library(${PROJECT_NAME} FITKCore)` 链接底层模块
- 通过 HDF5Config.cmake 配置链接 HDF5 库

### 在上层工程中的参与方式

FITKInterfaceGeoIOHDF5 作为几何领域专用的 IO 适配模块，一般由几何组件或几何数据管理组件进行链接和调用。

## 使用示例

### 1. 几何命令适配器的标准实现模式

```cpp
#include "FITKAdaptorGeoCommand.h"
#include "FITKGeoHDF5KeyWords.h"

// 创建长方体适配器
class FITKAdaptorGeoModelBox : public FITKAdaptorGeoCommand
{
protected:
    bool writeParameters(H5::Group& g) override
    {
        writeAttribute(g, FGKW_DataObjectID, H5::PredType::NATIVE_INT, &m_commandID);
        writeAttribute(g, FGKW_Length, H5::PredType::NATIVE_DOUBLE, &m_length);
        writeAttribute(g, FGKW_Width, H5::PredType::NATIVE_DOUBLE, &m_width);
        writeAttribute(g, FGKW_Height, H5::PredType::NATIVE_DOUBLE, &m_height);
        return true;
    }

    bool readParameters() override
    {
        readAttribute(*_h5Group, FGKW_DataObjectID, H5::PredType::NATIVE_INT, &m_commandID);
        readAttribute(*_h5Group, FGKW_Length, H5::PredType::NATIVE_DOUBLE, &m_length);
        readAttribute(*_h5Group, FGKW_Width, H5::PredType::NATIVE_DOUBLE, &m_width);
        readAttribute(*_h5Group, FGKW_Height, H5::PredType::NATIVE_DOUBLE, &m_height);
        return true;
    }
};
```

### 2. 几何数据的保存和加载

```cpp
#include "FITKAdaptorGeoCommand.h"
#include "FITKGeoHDF5KeyWords.h"

// 保存几何命令到HDF5
void saveGeoCommand(Interface::FITKAbsGeoModelBox* boxCmd, H5::Group& parentGroup)
{
    // 创建适配器
    Interface::FITKAdaptorGeoModelBox adaptor;
    
    // 设置数据对象
    adaptor.setDataObject(boxCmd);
    
    // 设置HDF5组和写入器
    adaptor.setH5Group(&parentGroup);
    adaptor.setWriter(hdf5Writer);
    
    // 执行写出
    adaptor.adaptW();
}

// 从HDF5加载几何命令
void loadGeoCommand(Interface::FITKAbsGeoModelBox* boxCmd, H5::Group& group)
{
    // 创建适配器
    Interface::FITKAdaptorGeoModelBox adaptor;
    
    // 设置数据对象
    adaptor.setDataObject(boxCmd);
    
    // 设置HDF5组和读取器
    adaptor.setH5Group(&group);
    adaptor.setReader(hdf5Reader);
    
    // 执行读取
    adaptor.adaptR();
}
```

## 使用建议

- 几何适配器应继承 `FITKAdaptorGeoCommand` 基类，遵循参数读写分离的设计模式。
- HDF5 关键字使用 `FITKGeoHDF5KeyWords` 中定义的常量，避免硬编码字符串。
- 几何命令的参数结构应保持向后兼容，新增参数时只追加不删除或修改已有字段。

## 注意事项

- HDF5 关键字常量定义在 `FITKGeoHDF5KeyWords.h` 中，新增关键字时应遵守命名规范。
- 几何适配器的 `writeParameters()` 和 `readParameters()` 方法应成对实现。
- 虚拓扑（Virtual Topology）相关的参数使用专门的虚拓扑关键字，需要特殊处理。

最后更新时间：2026-04-22
