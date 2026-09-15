# FITKVTKAlgorithm - VTK算法与可视化辅助模块

## 项目简介

FITKVTKAlgorithm 是 FITK 体系的 VTK 算法与可视化辅助模块，提供对 VTK 常用能力的工程化封装。

该模块位于 FITK_Interface 层，被 FITK_Component 中的渲染与可视化组件依赖，为图形显示、数据处理和交互变换提供基础工具类。

模块按功能分为四大类：数据处理与过滤、坐标与变换、渲染与图元、读写与对比。

## 架构与目录

```
FITKVTKAlgorithm/
├── 数据处理与过滤
│   ├── FITKDataSetDealer.h           # 外部数据集预处理
│   ├── FITKDeformFilter.h            # 位移变形过滤器
│   ├── FITKExtractGeometry.h         # 几何提取过滤器
│   ├── FITKExtractCellsByDim.h       # 按维度提取单元
│   ├── FITKSurfaceFilter.h            # 表面提取过滤器
│   ├── FITKPolyDataNormals.h         # 法向计算过滤器
│   ├── FITKShellFeatureEdges.h        # 特征边提取过滤器
│   ├── FITKVertexGlyphFilter.h       # 点元字形过滤
│   ├── FITKIdTypeArrayFilter.h       # 数组处理辅助
│   ├── FITKMaxMinValueFilter.h       # 最值计算辅助
│   └── FITKGridContinuityAlg.h       # 网格连续性判断
├── 坐标与变换相关
│   ├── FITKCoordinateAlgorithm.h      # 坐标轴网格与标注算法
│   ├── FITKCoordinateAlgorithmEX.h    # 坐标轴网格扩展算法
│   ├── FITKTransformWidget.h          # VTK 交互变换组件封装
│   └── FITKTransformRepresentation.h # 变换表示层封装
├── 渲染与图元辅助
│   ├── FITKGraphActor.h               # VTK Actor 封装
│   ├── FITKGraphActor2D.h             # 二维显示 Actor
│   ├── FITKGraphActorImage.h          # 图像 Actor
│   ├── FITKGraphActorBillboard3D.h    # Billboard 3D Actor
│   ├── FITKGraphAxisActor2D.h         # 二维坐标轴 Actor
│   ├── FITKGraphLegendScaleActor.h    # 比例尺图例 Actor
│   ├── FITKActorClipTool.h           # Actor 裁切工具
│   ├── FITKPolyPlacementMapper.h     # PolyData 定位 Mapper
│   ├── FITKPolyRenderStrategy.h     # PolyData 渲染策略
│   ├── FITKPolygonSource.h           # 多边形几何源
│   └── FITKPolyDataTool.h            # PolyData 辅助工具
├── 读写与对比工具
│   ├── FITKTecplotDatReader.h        # Tecplot DAT 数据读取器
│   ├── FITKXMLCompositeDataWriter.h  # 复合数据 XML 写出
│   ├── FITKXMLMultiBlockDataWriter.h # 多块数据 XML 写出
│   └── FITKCompareTool.h             # VTK 数据集一致性对比工具
├── API定义
│   └── FITKVTKAlgorithmAPI.h         # API宏定义
└── CMakeLists.txt
```

## 核心功能

### 1. 数据处理与过滤

#### FITKDeformFilter

位移变形过滤器，用于显示有限元分析结果变形：

- 支持设置位移分量（如 U1、U2、U3）
- 支持设置缩放系数
- 实时计算变形后的几何形状

#### FITKSurfaceFilter

表面提取过滤器（重写 vtkSurfaceFilter）：

- 提取几何表面
- 携带单元 ID 映射关系
- `getOriginCellIdBySurfaceCellId()`：获取原始单元 ID
- `getOriginPointIdBySurfacePointId()`：获取原始点 ID
- 支持多表面索引追踪

#### 其他过滤器

| 类名 | 职责 |
|------|------|
| FITKExtractGeometry | 几何提取过滤器 |
| FITKExtractCellsByDim | 按维度（点/线/面/体）提取单元 |
| FITKPolyDataNormals | 计算 PolyData 法向量 |
| FITKShellFeatureEdges | 提取壳体特征边 |
| FITKVertexGlyphFilter | 点元字形渲染 |
| FITKIdTypeArrayFilter | 数组 ID 处理辅助 |
| FITKMaxMinValueFilter | 计算数组最值 |
| FITKGridContinuityAlg | 网格连续性判断算法 |

### 2. 坐标与变换

#### FITKTransformWidget

VTK 交互变换组件封装（继承自 vtkAbstractWidget）：

- **TranslationEnabled**：平移变换开关
- **SetRepresentation()**：设置变换表示
- 支持鼠标交互式变换对象

#### FITKTransformRepresentation

变换表示层封装：

- 管理变换手柄
- 控制变换约束
- 显示变换结果

### 3. 渲染与图元

#### FITKGraphActor

VTK Actor 封装（继承自 vtkActor 与 FITKActorClipTool）：

- **ActorType**：SolidActor、SurfaceActor、EdgeActor、VertexActor
- **DataType**：GeometryData、DatumData、MeshData、PostData
- `setRelativeCoincidentTopologyPolygonOffsetParameters()`：设置多边形偏移参数
- `setAutoRemoveNormals()`：自动移除法向量

#### FITKActorClipTool

Actor 裁切工具：

- **ClipType**：Clip、Cut、ExtractGeometry
- **ClipInside**：裁切方向控制
- `setInputDataObject()`：设置输入数据
- `setClipFunction()`：设置裁切函数
- 支持平面、圆柱、球等裁切

#### 其他 Actor 类

| 类名 | 职责 |
|------|------|
| FITKGraphActor2D | 二维显示 Actor |
| FITKGraphActorImage | 图像显示 Actor |
| FITKGraphActorBillboard3D | Billboard 3D Actor |
| FITKGraphAxisActor2D | 二维坐标轴显示 |
| FITKGraphLegendScaleActor | 比例尺图例显示 |

#### 辅助工具

| 类名 | 职责 |
|------|------|
| FITKPolyPlacementMapper | PolyData 定位 Mapper |
| FITKPolyRenderStrategy | PolyData 渲染策略 |
| FITKPolygonSource | 多边形几何源生成 |
| FITKPolyDataTool | PolyData 辅助工具集 |

### 4. 读写与对比

#### FITKTecplotDatReader

Tecplot DAT 格式数据读取器：

- 支持读取 Tecplot 二进制和文本格式
- 解析场数据数组

#### FITKCompareTool

VTK 数据集一致性对比工具：

- `CompareVTKData()`：从点和单元角度对比两个数据集
- `CompareVTKDataPoints()`：仅从点角度对比
- `CompareVTKDataCells()`：仅从单元角度对比

## 关键类

### 数据处理

| 类名 | 说明 |
|------|------|
| FITKDataSetDealer | 外部数据集预处理，补充数组名称与单元 ID |
| FITKDeformFilter | 位移变形过滤器，支持分量设置与缩放 |
| FITKSurfaceFilter | 表面提取，携带单元/点 ID 映射 |
| FITKExtractCellsByDim | 按维度提取单元 |

### 坐标与变换

| 类名 | 说明 |
|------|------|
| FITKCoordinateAlgorithm | 坐标轴网格与标注算法 |
| FITKCoordinateAlgorithmEX | 坐标轴扩展算法 |
| FITKTransformWidget | VTK 交互变换组件封装 |
| FITKTransformRepresentation | 变换表示层封装 |

### Actor 封装

| 类名 | 说明 |
|------|------|
| FITKGraphActor | VTK Actor 封装，继承 vtkActor 与 FITKActorClipTool |
| FITKGraphActor2D | 二维显示 Actor |
| FITKGraphActorImage | 图像 Actor |
| FITKGraphActorBillboard3D | Billboard 3D Actor |
| FITKGraphAxisActor2D | 二维坐标轴 Actor |
| FITKGraphLegendScaleActor | 比例尺图例 Actor |
| FITKActorClipTool | Actor 裁切工具，支持多种裁切模式 |

### 读写与对比

| 类名 | 说明 |
|------|------|
| FITKTecplotDatReader | Tecplot DAT 数据读取器 |
| FITKCompareTool | VTK 数据集一致性对比工具 |

## 使用示例

### 示例1：使用 FITKGraphActor 封装 Actor

```cpp
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"

void setupActor(vtkPolyData* data, vtkMapper* mapper)
{
    auto* actor = FITKGraphActor::New();
    actor->SetInputData(data);
    actor->SetMapper(mapper);
    actor->setRelativeCoincidentTopologyPolygonOffsetParameters(1.0);
    actor->GetProperty()->SetColor(1.0, 0.5, 0.0);
}
```

### 示例2：使用 FITKDeformFilter 显示变形结果

```cpp
#include "FITK_Interface/FITKVTKAlgorithm/FITKDeformFilter.h"

void applyDeformFilter(vtkDataSet* input)
{
    auto* deform = FITKDeformFilter::New();
    deform->SetInputData(input);
    deform->setComponent("U");  // 设置位移分量
    deform->setScale(10.0);     // 设置缩放系数
    deform->Update();
    return deform->GetOutput();
}
```

### 示例3：使用对比工具验证数据集一致性

```cpp
#include "FITK_Interface/FITKVTKAlgorithm/FITKCompareTool.h"

bool checkDataConsistency(vtkDataSet* data1, vtkDataSet* data2)
{
    // 完整对比（点和单元）
    if (!FITKCompareTool::CompareVTKData(data1, data2)) {
        return false;
    }

    // 仅点对比
    if (!FITKCompareTool::CompareVTKDataPoints(data1, data2)) {
        return false;
    }

    return true;
}
```

## 依赖关系

### 外部依赖
- Qt5：Core、Gui、Widgets
- VTK

### 内部依赖
- FITK_Kernel/FITKCore

## 构建与集成

通过 CMake 接入：

```cmake
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# VTK 配置（需先配置 TOOLS_DIR）
include(${TOOLS_DIR}/cmake/VTKConfig.cmake)

add_library(FITKVTKAlgorithm SHARED)

target_link_libraries(FITKVTKAlgorithm
    PUBLIC
        Qt5::Core Qt5::Widgets Qt5::Gui
        VTK::VTK
        FITKCore
)
```

输出目录为 output/bin（Release）和 output/bin_d（Debug）。

## 注意事项

1. FITKGraphActor 同时继承 vtkActor 和 FITKActorClipTool，可直接使用裁切功能
2. FITKSurfaceFilter 保留原始单元 ID 映射，便于选择和查询
3. FITKDeformFilter 依赖输入数据中的位移场变量，需确保数据完整
4. FITKCompareTool 提供静态方法，可直接调用无需实例化

---

最后更新时间：2026-04-21