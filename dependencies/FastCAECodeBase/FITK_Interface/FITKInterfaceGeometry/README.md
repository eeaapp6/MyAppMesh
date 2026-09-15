# FITKInterfaceGeometry - 几何接口层模块

## 项目简介

FITKInterfaceGeometry 是 FITK 的几何接口层，提供几何命令抽象、命令工厂注册/创建、几何数据管理与虚拓扑相关的基础能力。

该模块本身主要定义接口与通用基础类，具体几何内核实现由上层或适配模块提供。模块采用分层架构设计，从命令抽象到具体操作接口再到虚拓扑支持，形成完整的能力覆盖。

该模块为几何建模应用提供统一的数据抽象层，使得上层 UI 层和业务逻辑层无需关心底层几何内核的具体实现细节。

## 架构与目录

```
FITKInterfaceGeometry/
├── 命令抽象层
│   ├── FITKAbsGeoCommand.h              # 几何命令抽象基类
│   ├── FITKGeoCommandList.h             # 命令列表管理
│   ├── FITKGeoCommandProp.h             # 命令属性定义
│   └── FITKGeoCommandManager.h          # 命令管理器
├── 几何操作接口层
│   ├── FITKAbsGeoModelPoint.h          # 点创建接口
│   ├── FITKAbsGeoModelCurve.h          # 曲线创建接口
│   ├── FITKAbsGeoModelSurface.h        # 曲面创建接口
│   ├── FITKAbsGeoModelSolid.h          # 实体创建接口
│   ├── FITKAbsGeoModelBox.h            # 盒子创建
│   ├── FITKAbsGeoModelCylinder.h       # 圆柱创建
│   ├── FITKAbsGeoModelCone.h           # 圆锥创建
│   ├── FITKAbsGeoModelSphere.h         # 球体创建
│   ├── FITKAbsGeoModelFrustum.h         # 平截头体创建
│   ├── FITKAbsGeoModelParaboloid.h     # 抛物面创建
│   ├── FITKAbsGeoModelFilledEllipse.h  # 椭圆创建
│   ├── FITKAbsGeoModelFilledPolygon.h  # 多边形创建
│   ├── FITKAbsGeoModelFilledRectangle.h # 矩形创建
│   ├── FITKAbsGeoOperBool.h           # 布尔运算接口
│   ├── FITKAbsGeoTransformation.h     # 变换接口
│   ├── FITKAbsGeoFeature.h             # 特征操作接口
│   ├── FITKAbsGeoSplitPart.h           # 分裂部件接口
│   ├── FITKAbsGeoStitch.h              # 缝合接口
│   ├── FITKAbsGeoDelete.h             # 删除操作接口
│   ├── FITKAbsGeoCoverEdges.h         # 覆盖边接口
│   ├── FITKAbsGeoMergeEdges.h         # 合并边接口
│   ├── FITKAbsGeoRepairSmallEdges.h    # 修复小边
│   ├── FITKAbsGeoRepairSmallFaces.h    # 修复小面
│   ├── FITKAbsGeoReplaceFaces.h        # 替换面
│   ├── FITKAbsGeoSampleCurve.h         # 曲线采样
│   └── FITKAbsGeoSplitter.h            # 分割器
├── 结构与辅助接口层
│   ├── FITKAbsGeoPart.h                # 部件抽象
│   ├── FITKAbsGeoInstance.h           # 实例抽象
│   ├── FITKAbsGeoImportedPart.h       # 导入部件
│   ├── FITKAbsGeoDatum.h              # 基准元素基类
│   ├── FITKAbsGeoReferencePlane.h     # 参考平面
│   ├── FITKAbsGeoReferenceLine.h      # 参考线
│   ├── FITKAbsGeoReferencePoint.h     # 参考点
│   ├── FITKAbsGeoSketch2D.h           # 二维草图
│   ├── FITKAbsGeoSketchFillet.h       # 草图倒角
│   ├── FITKAbsGeoSketchDeleter.h      # 草图删除
│   ├── FITKAbsGeoImportBase.h         # 导入基类
│   ├── FITKAbsGeoImport.h             # 导入接口
│   ├── FITKAbsGeoExport.h             # 导出接口
│   └── FITKAbsGeoModelExport.h        # 模型导出
├── 虚拓扑层
│   ├── FITKAbsVirtualTopo.h           # 虚拓扑抽象基类
│   ├── FITKAbsVirtualTopoMapper.h     # 虚拓扑映射器
│   ├── FITKVirtualTopoManager.h       # 虚拓扑管理器
│   ├── FITKShapeVirtualTopoManager.h  # 形状虚拓扑管理器
├── 工厂与工具
│   ├── FITKGeoInterfaceFactory.h      # 几何接口工厂（单例）
│   ├── FITKGeoEnum.h                  # 几何枚举定义
│   ├── FITKGeoModelManager.h          # 几何模型管理器
│   ├── FITKGeoCommandProp.h          # 命令属性
│   └── FITKAbsGeomTools.h            # 几何工具抽象
├── 数据与线程
│   ├── FITKImportFileThread.h         # 文件导入线程
│   ├── FITKExportFileThread.h        # 文件导出线程
│   ├── FITKGeoCommandList.h          # 命令列表
│   └── FITKGeoModelManager.h         # 模型管理器
└── Python绑定
    ├── PyFITKGeoBehavior.h           # Python几何行为
    └── PyFITKSketch2D.h              # Python草图
```

## 核心功能

### 1. 命令抽象与工厂模式

#### FITKAbsGeoCommand

几何命令的抽象基类，定义所有几何操作的公共接口：

- **生命周期管理**：update()
- **状态管理**：getCommandStatus()、setCommandStatus()
- **形状代理访问**：getShapeAgent()

#### FITKInterfaceGeometryFactory

几何接口工厂（单例模式），负责命令的注册与创建：

- **insertCommandCreateFun()**：注册命令创建函数
- **createCommand()**：根据枚举类型创建命令
- **insertDatumCreateFun()**：注册基准元素创建函数
- **createDatum()**：创建基准元素

### 2. 几何操作接口

#### 点/线/面/体创建

- **Point**：参考点创建
- **Curve**：曲线创建（包含 ThreePointsCurve 等）
- **Surface**：曲面创建
- **Solid**：实体创建（拉伸、旋转、扫略等）

#### 基础几何体

- **Box**：盒子（长方体）
- **Cylinder**：圆柱
- **Cone**：圆锥
- **Sphere**：球体
- **Frustum**：平截头体（截头锥）
- **Paraboloid**：抛物面
- **FilledEllipse**：填充椭圆
- **FilledPolygon**：填充多边形
- **FilledRectangle**：填充矩形

#### 操作与变换

- **Bool**：布尔运算（并、交、减）
- **Transformation**：变换操作（移动、旋转、缩放）
- **Feature**：特征操作（倒角、圆角、抽壳等）
- **SplitPart**：分裂部件
- **Stitch**：缝合操作

### 3. 基准元素

支持多种基准元素类型：

- **ReferencePlane**：参考平面
- **ReferenceLine**：参考线
- **ReferencePoint**：参考点
- **Sketch2D**：二维草图

### 4. 虚拓扑管理

#### FITKVirtualTopoManager

虚拓扑管理器，负责管理点/边/面/体等虚拓扑对象：

- **setRootObj()**：设置根对象
- **addRootObj()**：添加根对象
- **getRootObj()**：获取根对象
- **clearRootTopos()**：清空根节点数据
- 按类型管理虚拓扑、按 ID/形状查询虚拓扑

#### VirtualShape

虚拓扑形状数据结构，包含：
- CmdId：命令 ID
- VirtualTopoId：虚拓扑 ID
- VirtualTopoIndex：虚拓扑索引
- Type：虚拓扑类型

### 5. 几何模型管理

#### FITKGeoModelManager

几何模型管理器（继承自 FITKAbstractDataManager）：

- 统一管理 FITKAbstractGeoModel 对象集合
- **getBoundaryBox()**：获取全部模型包围盒
- **getDataByIndexT<T>()**：模板方法获取并转换数据

## 关键类

### 命令与工厂

| 类名 | 说明 |
|------|------|
| FITKAbsGeoCommand | 几何命令抽象基类 |
| FITKInterfaceGeometryFactory | 几何接口工厂（单例） |
| FITKGeoEnum | 几何枚举定义 |

### 管理器

| 类名 | 说明 |
|------|------|
| FITKGeoModelManager | 几何模型管理器 |
| FITKVirtualTopoManager | 虚拓扑管理器 |
| FITKShapeVirtualTopoManager | 形状虚拓扑管理器 |

### 基准元素

| 类名 | 说明 |
|------|------|
| FITKAbsGeoDatum | 基准元素抽象基类 |
| FITKAbsGeoReferencePlane | 参考平面 |
| FITKAbsGeoReferenceLine | 参考线 |
| FITKAbsGeoReferencePoint | 参考点 |
| FITKAbsGeoSketch2D | 二维草图 |

### 几何操作

| 类名 | 说明 |
|------|------|
| FITKAbsGeoModelSolid | 实体几何操作接口 |
| FITKAbsGeoOperBool | 布尔运算接口 |
| FITKAbsGeoTransformation | 变换接口 |
| FITKAbsGeoFeature | 特征操作接口 |

## 使用示例

### 示例1：通过工厂创建几何命令

```cpp
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

void createGeoCommand()
{
    auto* factory = Interface::FITKInterfaceGeometryFactory::getInstance();

    // 注册命令创建函数
    factory->insertCommandCreateFun(
        Interface::FITKGeoEnum::FITKGeometryComType::GCTBox,
        []() -> Interface::FITKAbsGeoCommand* {
            return new MyBoxCommand();
        }
    );

    // 创建命令
    Interface::FITKAbsGeoCommand* cmd = factory->createCommand(
        Interface::FITKGeoEnum::FITKGeometryComType::GCTBox
    );
}
```

### 示例2：获取模型包围盒

```cpp
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoModelManager.h"

void getModelBounds(Interface::FITKGeoModelManager* mgr)
{
    if (mgr == nullptr) return;

    double minPt[3] = {0}, maxPt[3] = {0};
    mgr->getBoundaryBox(minPt, maxPt);

    qDebug() << "Min:" << minPt[0] << minPt[1] << minPt[2];
    qDebug() << "Max:" << maxPt[0] << maxPt[1] << maxPt[2];
}
```

### 示例3：使用虚拓扑管理器

```cpp
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"

void queryVirtualTopo(Interface::FITKVirtualTopoManager* manager)
{
    if (manager == nullptr) return;

    // 获取根对象列表
    auto rootTopos = manager->getRootObj();
    for (auto* topo : rootTopos) {
        // 查询拓扑信息
        auto id = topo->getID();
        auto type = topo->getTopoType();
    }

    // 通过管理器按类型查询虚拓扑
    auto* vertexMgr = manager->getShapeVirtualTopoManager(
        Interface::FITKGeoEnum::VTopoShapeType::VSVertex);
    auto* edgeMgr = manager->getShapeVirtualTopoManager(
        Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
    auto* faceMgr = manager->getShapeVirtualTopoManager(
        Interface::FITKGeoEnum::VTopoShapeType::VSFace);

    // 获取各类型拓扑数量
    int vertexCount = manager->getShapeVirtualTopoCount(
        Interface::FITKGeoEnum::VTopoShapeType::VSVertex);
    int edgeCount = manager->getShapeVirtualTopoCount(
        Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
    int faceCount = manager->getShapeVirtualTopoCount(
        Interface::FITKGeoEnum::VTopoShapeType::VSFace);

    // 获取当前所有形状类型
    auto topoTypes = manager->getTopoTypes();
}
```

## 依赖关系

### 外部依赖
- Qt5：Core

### 内部依赖
- FITK_Kernel/FITKCore
- FITK_Interface/FITKInterfaceModel（FITKAbstractGeoModel、FITKModelEnum）

## 注意事项

1. 几何命令的创建应通过工厂模式，避免直接实例化具体命令类
2. 虚拓扑管理器负责维护拓扑层级关系，使用时需注意线程安全
3. 几何操作接口的具体实现由适配模块（如 FITKGeoCompOCC）提供
4. 导入/导出操作通过异步线程执行，避免阻塞主线程

---

最后更新时间：2026-04-21