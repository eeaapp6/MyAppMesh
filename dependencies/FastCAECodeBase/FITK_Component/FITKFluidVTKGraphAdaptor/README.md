# FITKFluidVTKGraphAdaptor - 流体VTK图形适配组件

## 简介

FITKFluidVTKGraphAdaptor 是 FastCAE 集成工具包（FITK）中负责流体仿真数据 VTK 可视化的图形适配组件。它提供了从流体数据对象到 VTK 图形对象的完整转换能力，支持几何模型、网格、材料点等多种数据类型的三维可视化。

该模块解决的核心问题是：如何将流体仿真过程中的各类数据对象（几何模型、边界网格、区域网格、材料点等）高效地转换为 VTK 可渲染的图形对象，并提供统一的交互能力（拾取、高亮、选择等）。

## 模块定位

FITKFluidVTKGraphAdaptor 位于 `FITK_Component` 下，属于功能组件层，主要承担以下职责：

- 提供数据对象到 VTK 图形对象的适配转换
- 实现几何模型、网格、材料点等的三维可视化
- 支持拾取、高亮、选择等交互操作
- 提供统一的视图模式控制（着色、线框、顶点显示）
- 作为流体仿真模块与渲染模块之间的桥梁

## 架构与目录

从当前目录中的头文件和源文件看，FITKFluidVTKGraphAdaptor 可以概括为以下几组能力：

```text
FITKFluidVTKGraphAdaptor
├── 核心接口
│   ├── FITKFluidVTKGraphAdaptorAPI    # API声明宏
│   └── FITKFluidVTKCommons            # 通用工具类（枚举、颜色常量）
├── 图形对象基类
│   ├── FITKFluidVTKGraphObject3D      # 3D图形对象基类
│   └── FITKFluidVTKGraphObjectModelBase # 模型图形对象基类
├── 具体图形对象
│   ├── FITKFluidVTKGraphObjectModelShape   # 模型形状图形对象
│   ├── FITKFluidVTKGraphObjectModelCmd     # 模型命令图形对象
│   ├── FITKFluidVTKGraphObjectBoundMesh    # 边界网格图形对象
│   ├── FITKFluidVTKGraphObjectRegionMesh   # 区域网格图形对象
│   ├── FITKFluidVTKGraphObjectMaterialPoints # 材料点图形对象
│   ├── FITKFluidVTKGraphObjectFixedSizeBase # 固定尺寸基类
│   ├── FITKFluidVTKGraphObjectMeasureTools # 测量工具图形对象
│   ├── FITKFluidVTKGraphObjectSelect       # 选择器图形对象
│   ├── FITKFluidVTKGraphObjectPick         # 拾取图形对象
│   └── FITKFluidVTKGraphObjectPickPreview  # 拾取预览图形对象
├── 视图适配器
│   ├── FITKFluidVTKViewAdaptorBase          # 视图适配器基类
│   ├── FITKFluidVTKViewAdaptorModelShape    # 模型形状适配器
│   ├── FITKFluidVTKViewAdaptorModelCmd      # 模型命令适配器
│   ├── FITKFluidVTKViewAdaptorBoundMesh     # 边界网格适配器
│   ├── FITKFluidVTKViewAdaptorRegionMesh    # 区域网格适配器
│   └── FITKFluidVTKViewAdaptorMaterialPoints # 材料点适配器
└── 辅助工具
    └── FITKFluidVTKShapeColorMap            # 颜色映射管理器
```

## 依赖关系

### 编译依赖

根据模块源码中的头文件引用，该模块当前显式依赖：

- Qt5::Core
- VTK（Visualization Toolkit）
- FITKCore（基础对象、数据对象）
- FITKAdaptor（视图适配器基类）
- FITKRenderWindowVTK（VTK渲染窗口组件）
- FITKInterfaceGeometry（几何接口）

### 模块层级关系

- FITKFluidVTKGraphAdaptor 依赖 `FITKCore`、`FITKAdaptor` 和 `FITKRenderWindowVTK`
- 上层流体仿真模块通过该组件实现数据的可视化展示
- 该组件通过 `FITKGraphObjectVTK` 基类与渲染窗口集成

## 主要类与功能说明

### 核心工具类

#### `FITKFluidVTKCommons`

通用工具类，提供图形对象所需的枚举定义、颜色常量和辅助方法。

**枚举类型：**
- `ShapeAbsEnum`：形状类型枚举（复合、实体、壳、面、线、边、顶点）
- `ShapeMeshViewMode`：视图模式（着色、线框、顶点）
- `ShapePickMode`：拾取模式（顶点、边、面、实体、全部）
- `ShapeType`：形状网格类型（模型顶点、边、面、实体、网格节点、单元）

**颜色常量：**
- `s_colorList`：默认颜色列表
- `s_geoVertexColor` / `s_geoEdgeColor` / `s_geoFaceColor`：几何元素颜色
- `s_meshFaceColor`：网格面颜色
- `s_regionMeshFaceColor`：区域网格颜色
- `s_matPointsColor`：材料点颜色

**辅助方法：**
- `GetColorByIndex()`：按索引获取颜色
- `GetRandomColor()`：获取随机颜色
- `QColorToDouble3()` / `QColorToInt3()`：颜色格式转换

### 图形对象基类

#### `FITKFluidVTKGraphObject3D`

所有流体3D图形对象的基类，继承自 `FITKGraphObjectVTK`。提供统一的图形对象接口。

**核心功能：**
- 视图模式控制：`setViewMode()`
- 透明度设置：`setTransparent()`
- 颜色设置：`setColor()`
- 拾取模式控制：`setPickMode()`
- 高亮操作：`highlight()` / `preHighlight()` / `advanceHighlight()`
- 可见性控制：`setVisible()`
- 渲染层级管理：`getRenderLayer()` / `getRenderLayerHighlight()`

#### `FITKFluidVTKGraphObjectModelBase`

模型图形对象基类，继承自 `FITKFluidVTKGraphObject3D`。专门处理几何模型数据的可视化。

**核心功能：**
- 模型网格数据生成：`meshVSToVtkDataSet()`
- 顶点/边/面网格创建：`createVertexGrid()` / `createEdgeGrid()` / `createFaceGrid()`
- 颜色映射管理：通过 `FITKFluidVTKShapeColorMap` 管理各元素颜色
- 拾取ID映射：维护VTK单元ID与形状ID之间的映射关系

### 视图适配器类

#### `FITKFluidVTKViewAdaptorBase`

视图适配器基类，继承自 `FITKAbstractViewAdaptor`。负责将数据对象转换为图形对象。

**主要方法：**
- `setDataObject()`：设置输入数据对象
- `getOutputData()`：获取输出图形对象
- `update()`：执行数据转换

### 具体图形对象类

| 类名 | 功能说明 |
|------|---------|
| `FITKFluidVTKGraphObjectModelShape` | 几何模型形状图形对象 |
| `FITKFluidVTKGraphObjectModelCmd` | 模型命令图形对象 |
| `FITKFluidVTKGraphObjectBoundMesh` | 边界网格图形对象 |
| `FITKFluidVTKGraphObjectRegionMesh` | 区域网格图形对象 |
| `FITKFluidVTKGraphObjectMaterialPoints` | 材料点图形对象 |
| `FITKFluidVTKGraphObjectSelect` | 选择器图形对象 |
| `FITKFluidVTKGraphObjectPick` | 拾取图形对象 |
| `FITKFluidVTKGraphObjectMeasureTools` | 测量工具图形对象 |

### 辅助工具类

#### `FITKFluidVTKShapeColorMap`

颜色映射管理器，负责管理VTK数据集的颜色数组。

**主要方法：**
- `setInputData()`：设置输入数据集
- `setCellsColor()`：设置指定单元的颜色
- `setColor()`：设置所有单元的颜色
- `update()`：更新颜色数组

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库，需链接 VTK 库及 FITK 核心模块：

- `add_library(FITKFluidVTKGraphAdaptor SHARED)`
- 启用 `AUTOMOC`
- 链接依赖：`FITKCore`、`FITKAdaptor`、`FITKRenderWindowVTK`、`VTK::RenderingCore`、`VTK::RenderingOpenGL2`

### 在上层工程中的参与方式

FITKFluidVTKGraphAdaptor 作为组件层模块，通常通过以下方式接入应用：

1. 通过视图适配器将数据对象转换为图形对象
2. 将图形对象添加到 VTK 渲染窗口进行显示
3. 通过图形对象接口实现交互操作（拾取、高亮等）

## 使用示例

### 1. 创建模型图形对象

```cpp
#include "FITKFluidVTKGraphObjectModelShape.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoShapeData.h"

// 获取几何数据对象
Core::FITKAbstractDataObject* geoData = FITKDATAREPO->getDataById("geometry");

// 创建图形对象
Exchange::FITKFluidVTKGraphObjectModelShape* shapeObj = 
    new Exchange::FITKFluidVTKGraphObjectModelShape(geoData);

// 设置视图模式（着色模式）
shapeObj->setViewMode(Exchange::FITKFluidVTKCommons::SMVM_Shade, true);

// 添加到渲染窗口
graphWidget->addGraphObject(shapeObj);
```

### 2. 使用视图适配器

```cpp
#include "FITKFluidVTKViewAdaptorModelShape.h"

// 创建适配器
Exchange::FITKFluidVTKViewAdaptorModelShape* adaptor = 
    new Exchange::FITKFluidVTKViewAdaptorModelShape();

// 设置输入数据
adaptor->setDataObject(geoData);

// 执行转换
if (adaptor->update()) {
    // 获取输出图形对象
    Exchange::FITKFluidVTKGraphObject3D* graphObj = adaptor->getOutputData();
    // 添加到渲染窗口
    graphWidget->addGraphObject(graphObj);
}
```

### 3. 高亮操作

```cpp
#include "FITKFluidVTKGraphObject3D.h"

// 获取图形对象
Exchange::FITKFluidVTKGraphObject3D* graphObj = ...;

// 设置高亮（面类型）
graphObj->highlight(Exchange::FITKFluidVTKCommons::ModelFace, QColor(255, 0, 0));

// 高级高亮（高亮指定索引的面）
QVector<int> faceIndices = {0, 1, 2};
graphObj->advanceHighlight(Exchange::FITKFluidVTKCommons::ModelFace, faceIndices, QColor(255, 0, 0));

// 取消高亮
graphObj->disHighlight();
```

### 4. 设置拾取模式

```cpp
#include "FITKFluidVTKGraphObject3D.h"

// 设置拾取模式为面拾取
graphObj->setPickMode(Exchange::FITKFluidVTKCommons::PickFace);

// 获取拾取的形状ID
int shapeId = graphObj->getShapeIdByVTKCellId(vtkCellId, 
    Exchange::FITKFluidVTKCommons::STA_FACE);
```

### 5. 颜色映射使用

```cpp
#include "FITKFluidVTKShapeColorMap.h"

// 创建颜色映射
Exchange::FITKFluidVTKShapeColorMap* colorMap = new Exchange::FITKFluidVTKShapeColorMap();

// 设置输入数据集
colorMap->setInputData(vtkDataSet);

// 设置所有单元颜色
colorMap->setColor("Default", QColor(100, 150, 200));

// 设置指定单元颜色
QList<int> indices = {0, 2, 4};
colorMap->setCellsColor("Selection", indices, QColor(255, 0, 0));

// 更新
colorMap->update();
```

## 注意事项

- FITKFluidVTKGraphAdaptor 是流体仿真可视化的核心组件，通常与 `FITKRenderWindowVTK` 配合使用
- 图形对象的生命周期由渲染窗口管理，不应在外部手动删除
- 高级高亮模式需要预先设置颜色映射，否则可能无法正确显示
- 拾取操作依赖 VTK 的拾取机制，需要确保渲染窗口已正确初始化
- 大模型建议使用 `setGraphSimplified()` 开启简化模式，提升交互性能

最后更新时间：2026-04-30