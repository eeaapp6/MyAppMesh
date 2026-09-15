# FITKGeoCompOCC - OCC 几何建模组件

## 简介

FITKGeoCompOCC 是 `FITK_Component` 层中的 OCC 几何建模组件，负责为 `FITKInterfaceGeometry` 抽象层提供基于 OpenCascade 的具体几何实现。

该模块本身不是“几何数据管理器”，也不是“HDF5/文件 IO 模块”。它的核心职责是在组件加载时向 `FITKInterfaceGeometryFactory` 和 `FITKInterfaceAlgorithmFactory` 注册 OCC 几何命令、基准元素和几何查询工具，使上层应用、数据模块和几何 IO 模块能够创建并执行 OCC 几何命令。

## 架构

从当前代码组织看，FITKGeoCompOCC 主要由以下几部分组成：

```text
FITKGeoCompOCC
├── 组件入口
│   ├── FITKGeoCompOCCInterface
│   └── FITKGeoCompOCCAPI
├── 公共 OCC 形状抽象
│   ├── FITKAbstractOCCModel
│   └── OCCShapeAgent
├── 几何工具与算法工具
│   ├── FITKOCCGeomTools
│   ├── FITKOCCAlgTools
│   └── FITKOCCVirtualTopoCreator
├── 基准元素
│   ├── FITKOCCReferencePoint
│   ├── FITKOCCReferenceLine
│   └── FITKOCCReferencePlane
├── 草图与部件
│   ├── FITKOCCModelSketch2D
│   └── FITKOCCModelPart
├── 导入导出与辅助
│   ├── FITKOCCModelImport
│   ├── FITKOCCModelImportFlattenAssembly
│   ├── FITKOCCModelExport
│   ├── FITKOCCModelExportTopos
│   ├── FITKOCCDXFReader
│   └── FITKOCCModelFromMesh
├── 基础几何与造型
│   ├── FITKOCCModelSimpleShape
│   ├── FITKOCCModelCurve
│   ├── FITKOCCModelSurface
│   └── FITKOCCModelSolid
├── 布尔、特征与变换
│   ├── FITKOCCOperBool
│   ├── FITKOCCNBodyUnite
│   ├── FITKOCCFeature
│   ├── FITKOCCTransformation
│   └── FITKOCCModelTransformPart
└── 分割、修复与编辑
    ├── FITKOCCSplitter
    ├── FITKOCCModelPartitionEdge
    ├── FITKOCCModelPartitionFace
    ├── FITKOCCModelPartitionSolid
    ├── FITKOCCModelStitch
    ├── FITKOCCModelMergeEdges
    ├── FITKOCCModelMergePart
    ├── FITKOCCGeoSketchDeleter
    └── FITKShapeFix_FixSmallFace
```

模块内的控制关系比较明确：

1. `OCC::FITKGeoCompOCCInterface` 在构造函数中完成注册。
2. `Interface::FITKInterfaceGeometryFactory` 保存“几何命令类型 -> OCC 派生类”的映射。
3. 各个 `FITKOCCModel*` / `FITKOCCReference*` 类作为 `FITKInterfaceGeometry` 抽象命令的 OCC 实现，通过 `update()` 真正生成或修改 `TopoDS_Shape`。
4. `FITKAbstractOCCModel` 统一处理形状、维度、包围盒、虚拓扑与三角化等公共逻辑。

## 依赖

- 内部依赖：`FITKCore`、`FITKAppFramework`、`FITKInterfaceModel`、`FITKInterfaceGeometry`
- 外部依赖：`Qt5::Core`、`Qt5::Gui`、`Qt5::Widgets`、`OCC::OCC`、`DIME::DIME`
- 构建前置：`TOOLS_DIR` 必须可用，用于加载 `OCCConfig.cmake` 与 `DimeConfig.cmake`
- 运行依赖：部署环境需要可用的 OCC 与 DIME 动态库；若缺失，组件虽可编译通过，但运行期几何能力不可用

## 主要类与功能说明

| 类名 | 职责 | 备注 |
| --- | --- | --- |
| `OCC::FITKGeoCompOCCInterface` | OCC 组件入口，负责注册几何命令、基准元素和工具创建器 | `getComponentName()` 返回 `FITKGeoCompOCC`；`exec()` 当前仅返回 `true`，实际核心逻辑在构造函数内 |
| `OCC::FITKAbstractOCCModel` | OCC 几何命令公共基类 | 统一管理 `TopoDS_Shape`、虚拓扑、三角化、包围盒与维度信息 |
| `OCC::FITKOCCGeomTools` / `FITKOCCToolCreator` | OCC 几何查询工具创建器与实现 | 为点、边、面、通用查询提供 OCC 后端实现 |
| `OCC::FITKOCCModelPart` | OCC 部件命令 | 管理部件形状、子命令追加与形状回退 |
| `OCC::FITKOCCModelSketch2D` | OCC 二维草图命令 | 支持草图闭合性、自相交性、连贯性检查与草图形状合并 |
| `OCC::FITKOCCModelImport` / `FITKOCCModelExport` | 几何导入导出命令 | 对应接口层导入/导出抽象的 OCC 实现 |
| `OCC::FITKOCCModelBox`、`FITKOCCModelCylinder`、`FITKOCCModelSphere` 等 | 基础几何体创建 | 来自 `FITKOCCModelSimpleShape` |
| `OCC::FITKOCCOperBool`、`FITKOCCNBodyUnite` | 布尔与多体求和 | 对应布尔建模操作 |
| `OCC::FITKOCCModelTransform`、`FITKOCCModelRotate`、`FITKOCCModelScale` 等 | 几何变换与阵列 | 来自 `FITKOCCTransformation` |
| `OCC::FITKOCCCurveSplitter`、`FITKOCCSurfaceSplitter`、`FITKOCCSolidSplitter` | 几何分割器 | 提供曲线/曲面/实体分割能力 |

## 调用关系（仓库内）

基于当前仓库检索，FITKGeoCompOCC 的典型使用关系包括：

1. `FITKGeoOCCIOHDF5` 读取 HDF5 几何命令时，调用 `FITKInterfaceGeometryFactory::createCommand(...)` 和 `createDatum(...)` 恢复命令对象。
2. `FITKGeoOCCIOHDF5` 中的形状序列化辅助类直接依赖 `FITKAbstractOCCModel` 与 `FITKOCCVirtualTopoCreator`。
3. `FITKAbaqusPart::initializeGeomPart(...)` 通过 `FITKInterfaceGeometryFactory::createCommand(geomType)` 创建几何部件命令，再加入 `FITKGeoCommandList`。
4. 应用层真正使用的是接口层工厂和命令列表；FITKGeoCompOCC 负责提供 OCC 后端实现，而不是让上层直接 new 每个命令类。

## 构建或集成方式

该模块通过 `FITK_Component/FITKGeoCompOCC/CMakeLists.txt` 构建为共享库：

1. `project(FITKGeoCompOCC VERSION 1.0.0 LANGUAGES CXX)`
2. `add_library(${PROJECT_NAME} SHARED)`
3. 启用 `CMAKE_AUTOMOC`、`CMAKE_AUTORCC`、`CMAKE_AUTOUIC`
4. 链接 Qt、OCC、DIME 以及 FITK 内部基础模块
5. 输出目录与仓库统一放在 `output/bin_d` 与 `output/bin`

作为组件模块，它通常不单独运行，而是由应用框架加载后完成几何工厂注册。上层真正依赖的是：

1. 组件名 `FITKGeoCompOCC`
2. `FITKInterfaceGeometryFactory` 中被注册的 OCC 命令创建函数
3. `FITKInterfaceGeometryFactory` 中被注册的 OCC 基准元素创建函数
4. `FITKInterfaceGeometryFactory` / `FITKInterfaceAlgorithmFactory` 中被注册的 OCC 工具创建器

## 使用方法

1. 确保应用已加载 `FITKGeoCompOCC` 组件。
2. 通过 `FITKInterfaceGeometryFactory` 创建几何命令或基准元素，而不是直接依赖某个具体实现类。
3. 设置命令参数后调用 `update()` 生成或更新几何形状。
4. 将生成的命令对象加入 `FITKGeoCommandList`，纳入统一几何数据管理、撤销重做和后续显示流程。
5. 如需几何查询能力，通过 `getGeomToolsCreator()` 获取 OCC 查询工具。

## 使用示例

### 示例 1：在应用程序组件工厂中注册 OCC 组件

标准接入方式不是直接操作组件管理器，而是在应用程序自己的 `FITKComponentFactory` 派生类里返回组件列表，再由框架通过 `regComponentsFactory(...)` 和 `create()` 完成统一初始化与注册。

```cpp
#include "FITK_Kernel/FITKAppFramework/FITKComponentFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Component/FITKGeoCompOCC/FITKGeoCompOCCInterface.h"

class MyComponentFactory : public AppFrame::FITKComponentFactory
{
private:
    QList<AppFrame::FITKComponentInterface*> createComponents() override
    {
        QList<AppFrame::FITKComponentInterface*> componentList;
        componentList << new OCC::FITKGeoCompOCCInterface;
        return componentList;
    }
};

void registerOccComponentFactory(AppFrame::FITKApplication& app)
{
    app.regComponentsFactory(new MyComponentFactory);

    // 应用启动后，框架内部会调用 _componentsFactory->create()，
    // 并将 FITKGeoCompOCCInterface 加入 FITKAPP->getComponents()。
}
```

### 示例 2：确保 OCC 组件已加载并创建盒体命令

这个示例展示 FITKGeoCompOCC 的典型使用方式：先确保组件存在，再通过接口层工厂创建 OCC 盒体命令，最后加入几何命令列表。

```cpp
#include "FITK_Component/FITKGeoCompOCC/FITKGeoCompOCCInterface.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelBox.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

bool createOccBox()
{
    auto* comp = FITKAPP->getComponents()->
        getComponentTByName<OCC::FITKGeoCompOCCInterface>("FITKGeoCompOCC");
    if (comp == nullptr) return false;

    auto* factory = Interface::FITKInterfaceGeometryFactory::getInstance();
    if (factory == nullptr) return false;
    if (!factory->getCommandSupported(Interface::FITKGeoEnum::FGTBox)) return false;

    auto* box = factory->createCommandT<Interface::FITKAbsGeoModelBox>(Interface::FITKGeoEnum::FGTBox);
    if (box == nullptr) return false;

    double origin[3] = {0.0, 0.0, 0.0};
    double length[3] = {10.0, 20.0, 30.0};
    box->setPoint1(origin);
    box->setLength(length);

    if (!box->update())
    {
        delete box;
        return false;
    }

    auto* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
    if (geoList == nullptr)
    {
        delete box;
        return false;
    }

    geoList->appendDataObj(box);
    return true;
}
```

### 示例 3：通过工厂创建基准元素

FITKGeoCompOCC 在构造时同时注册了 OCC 基准点、基准线和基准面实现，因此上层也应通过接口层工厂创建。

```cpp
#include "FITK_Component/FITKGeoCompOCC/FITKGeoCompOCCInterface.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"

Interface::FITKAbsGeoDatum* createDatumPoint()
{
    auto* factory = Interface::FITKInterfaceGeometryFactory::getInstance();
    if (factory == nullptr) return nullptr;

    if (!factory->getDatumSupported(Interface::FITKGeoEnum::FDTPoint))
        return nullptr;

    return factory->createDatum(Interface::FITKGeoEnum::FDTPoint);
}
```

### 示例 4：获取 OCC 几何查询工具

该模块会向工厂注册 OCC 查询工具创建器，供上层进行点、边、面等几何查询。

```cpp
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"

bool hasOccGeomTools()
{
    auto* factory = Interface::FITKInterfaceGeometryFactory::getInstance();
    if (factory == nullptr) return false;

    auto* creator = factory->getGeomToolsCreator();
    if (creator == nullptr) return false;

    auto* pointTool = creator->createPointTool();
    auto* edgeTool = creator->createEdgeTool();
    auto* faceTool = creator->createFaceTool();

    const bool ok = pointTool && edgeTool && faceTool;

    delete pointTool;
    delete edgeTool;
    delete faceTool;
    return ok;
}
```

## 注意事项

1. `FITKGeoCompOCCInterface::exec()` 当前不承担主要业务逻辑；核心作用在于组件构造阶段完成注册。
2. 上层不应把该模块当作“文件 IO 组件”使用，它主要提供 OCC 几何命令实现和几何工具实现。
3. 推荐始终通过 `FITKInterfaceGeometryFactory` 创建命令与基准元素，以保持后端可替换性。
4. 该模块依赖 OCC 与 DIME；若部署缺失相关运行库，几何导入、造型或查询能力会失效。
5. 本模块提供大量具体命令实现，但 README 不逐个展开全部类；新增功能时应优先先看 `FITKGeoCompOCCInterface.cpp` 中的注册表，再定位对应头源文件。
6. 当前仓库中 `FITKGeoOCCIOHDF5`、`FITKAbaqusData` 等模块会间接依赖本模块注册出来的几何命令实现。

最后更新时间：2026-04-25