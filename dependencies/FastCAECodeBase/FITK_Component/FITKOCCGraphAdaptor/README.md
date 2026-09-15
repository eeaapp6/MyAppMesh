# FITKOCCGraphAdaptor

## 简介

FITKOCCGraphAdaptor 是 FITK 组件层中的 OCC 可视化适配模块，负责把几何、网格、测量、拾取和辅助构造等数据对象转换为 OCC 图形对象，供 FITKRenderWindowOCC 在三维场景中显示与交互。

该模块本身不直接提供渲染窗口，也不直接维护业务数据模型，而是承担“数据对象到图形对象”的中间适配职责。从分层角度看，它位于 FITKInterfaceModel、FITKInterfaceGeometry 等数据抽象与 FITKRenderWindowOCC 显示组件之间，是典型的视图适配层。

## 模块定位

FITKOCCGraphAdaptor 位于 FITK_Component 下，主要承担以下职责：

- 提供统一的 OCC 视图适配入口，把不同数据域映射为统一的渲染对象。
- 通过适配器工厂与注册宏，把 graph key、数据类型与具体适配实现解耦。
- 为上层业务代码提供稳定的“输入对象、更新、输出图形对象”调用方式。
- 为 FITKRenderWindowOCC 提供统一的 Exchange::FITKOCCGraphObject3D 对象。

该模块不负责组件注册、窗口生命周期、应用命令调度以及几何建模算法本身，也不承担工程读写与脚本管理能力。

## 架构与目录

结合当前目录与 CMakeLists.txt，模块内部可以概括为以下几组代码：

1. 公共基础文件

- FITKOCCCommons.h / .cpp
- FITKOCCGraphAdaptorAPI.h
- FITKAIS_ObjectBase.h / .cpp

2. 图形对象基类与公共对象层

- FITKOCCGraphObject3D.h / .cpp
- FITKOCCGraphObjectTopoBase.h / .cpp
- FITKOCCGraphObjectToolBase.h / .cpp
- FITKOCCGraphObjectDimensionToolBase.h / .cpp
- FITKOCCGraphObjectPickToolBase.h / .cpp

3. 几何、草绘和预览图形对象

- FITKOCCGraphObjectModelBase.h / .cpp
- FITKOCCGraphObjectModelCommand.h / .cpp
- FITKOCCGraphObjectModelShape.h / .cpp
- FITKOCCGraphObjectModelCommandPreview.h / .cpp
- FITKOCCGraphObjectModelShapePreview.h / .cpp
- FITKOCCGraphObjectSketchCommand.h / .cpp
- FITKOCCGraphObjectSketchShape.h / .cpp

4. 基准、拓扑和辅助显示对象

- FITKOCCGraphObjectDatumPoint.h / .cpp
- FITKOCCGraphObjectDatumLine.h / .cpp
- FITKOCCGraphObjectDatumPlane.h / .cpp
- FITKOCCGraphObjectTopoShape.h / .cpp
- FITKOCCGraphObjectTopoDirection.h / .cpp
- FITKOCCGraphObjectTopoIndexLabels.h / .cpp
- FITKOCCGraphObjectDimensionDistance.h / .cpp
- FITKOCCGraphObjectDimensionAngle.h / .cpp
- FITKOCCGraphObjectDimensionArea.h / .cpp
- FITKOCCGraphObjectDimensionVolume.h / .cpp
- FITKOCCGraphObjectPickDirection.h / .cpp
- FITKOCCGraphObjectPickProjPos.h / .cpp
- FITKOCCGraphObjectGlobalComponent.h / .cpp

5. 适配器基类与具体适配器

- FITKOCCViewAdaptorBase.h / .cpp
- FITKOCCViewAdaptorModelShape.h / .cpp
- FITKOCCViewAdaptorModelCommand.h / .cpp
- FITKOCCViewAdaptorModelShapePreview.h / .cpp
- FITKOCCViewAdaptorModelCommandPreview.h / .cpp
- FITKOCCViewAdaptorMesh.h / .cpp
- FITKOCCViewAdaptorDatumPoint.h / .cpp
- FITKOCCViewAdaptorDatumLine.h / .cpp
- FITKOCCViewAdaptorDatumPlane.h / .cpp
- FITKOCCViewAdaptorDimensionTool.h / .cpp
- FITKOCCViewAdaptorPickTool.h / .cpp
- FITKOCCViewAdaptorTopoShape.h / .cpp
- FITKOCCViewAdaptorTopoDirection.h / .cpp
- FITKOCCViewAdaptorTopoIndexLabels.h / .cpp
- FITKOCCViewAdaptorSketchCommand.h / .cpp
- FITKOCCViewAdaptorSketchShape.h / .cpp
- FITKOCCViewAdaptorGlobalComponent.h / .cpp

模块的典型调用链是：上层业务持有 Core::FITKAbstractDataObject 派生对象，业务层通过 FITKVIEWADAPTORFACTORY 创建对应适配器，适配器输出 Exchange::FITKOCCGraphObject3D 或其派生对象，随后由 FITKRenderWindowOCC 完成显示与交互。

## 依赖关系

### 外部依赖

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- OCC::OCC

### FITK 内部依赖

- FITKCore
- FITKAdaptor
- FITKInterfaceModel
- FITKInterfaceGeometry
- FITKGeoCompOCC
- FITKRenderWindowOCC

从依赖关系可以看出，该模块向上承接 FITKInterfaceModel 与 FITKInterfaceGeometry 的数据抽象，向下复用 FITKAdaptor 的工厂与抽象基类，并通过 FITKRenderWindowOCC 的图形对象体系接入 OCC 显示能力。

## 主要类与功能说明

### Exchange::FITKOCCViewAdaptorBase

FITKOCCViewAdaptorBase 继承自 Adaptor::FITKAbstractViewAdaptor，是全部 OCC 视图适配器的公共基类。当前头文件中可以确认的核心接口包括：

- setDataObject(Core::FITKAbstractDataObject*)
- setDetails(QVariant)
- getOutputData()
- update()

该类统一了“设置输入对象或细节参数、执行更新、读取输出图形对象”的基本适配流程。

### Exchange::FITKOCCGraphObject3D

FITKOCCGraphObject3D 继承自 Render::FITKGraphObjectOCC，是 OCC 图形对象公共基类。它负责统一处理图形对象的可见性、更新、颜色、透明度、线宽和交互状态。当前头文件可直接确认的典型接口包括：

- getGraphInfo()
- getDataId()
- getDataObj()
- setVisible(bool)
- setViewMode(...)
- clearCache()
- update(bool forceUpdate = false)
- updateVisibility()
- updateInteractiveObjs()
- setColor(...)
- setEnableTransparency(bool)
- setPointMark(...)
- setPointSizeScale(double)
- setLineWidth(double)

### 典型适配器与图形对象

- Exchange::FITKOCCViewAdaptorModelShape：把 Interface::FITKAbsGeoShapeAgent 转换为 OCC 形体显示对象。
- Exchange::FITKOCCViewAdaptorModelCommand：处理建模命令对应的图形对象表示。
- Exchange::FITKOCCViewAdaptorModelShapePreview：处理预览态几何对象显示。
- Exchange::FITKOCCViewAdaptorMesh：处理 OCC::FITKUnstructuredMesh 到 MeshVS 对象的适配。
- Exchange::FITKOCCViewAdaptorDimensionTool：处理测量工具相关的 OCC 辅助显示对象。
- Exchange::FITKOCCViewAdaptorPickTool：处理拾取过程中的辅助图形表示。
- Exchange::FITKOCCGraphObjectModelShape：形体显示对象。
- Exchange::FITKOCCGraphObjectMesh：网格显示对象。
- Exchange::FITKOCCGraphObjectDatumPoint / DatumLine / DatumPlane：几何基准对象显示。
- Exchange::FITKOCCGraphObjectDimensionDistance / Angle / Area / Volume：测量结果对象显示。
- Exchange::FITKOCCGraphObjectTopoShape / TopoDirection / TopoIndexLabels：虚拓扑与辅助标注显示。

### 注册机制

FITKOCCGraphAdaptor 通过 Register2FITKViewAdaptorFactory 宏把具体实现注册到 FITKViewAdaptorFactory。当前源码中已经确认的一组 key 包括：

- GeoModelShapeOCC
- GeoModelCmdOCC
- GeoModelCmdPreviewOCC
- GeoModelShapePreviewOCC
- MeshOCC
- GeoDatumPointOCC
- GeoDatumLineOCC
- GeoDatumPlaneOCC
- MeasureToolOCC
- PickToolOCC
- TopoShapeOCC
- TopoDirectionOCC
- TopoIndexLabelsOCC
- GeoSketchCmdOCC
- GeoSketchShapeOCC
- GlobalGeoComp

其中，key 与 className 共同决定具体适配器实现。例如 GeoModelShapeOCC 对应 Interface::FITKAbsGeoShapeAgent，MeshOCC 对应 OCC::FITKUnstructuredMesh，MeasureToolOCC 对应 DimensionTools。

## 构建与集成

当前模块通过 CMake 构建为共享库：

- project(FITKOCCGraphAdaptor VERSION 1.0.1 LANGUAGES CXX)
- add_library(${PROJECT_NAME} SHARED)
- 启用 CMAKE_AUTOMOC、CMAKE_AUTOUIC、CMAKE_AUTORCC
- 输出目录为 output/bin_d 和 output/bin

在上层工程中，FITKOCCGraphAdaptor 一般不被独立启动，而是作为组件层库参与整体链接。典型接入方式是由业务层确定数据对象类型，再通过 FITKVIEWADAPTORFACTORY 创建对应适配器，成功 update 后将输出对象加入 FITKRenderWindowOCC。

## 使用示例

下面的示例仅使用当前源码中已经确认存在的 key、类型和接口。

### 1. 为几何形体数据创建 OCC 图形对象

```cpp
#include "FITK_Kernel/FITKAdaptor/FITKViewAdaptorFactory.h"
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCViewAdaptorBase.h"

Exchange::FITKOCCGraphObject3D* buildShapeObject(Core::FITKAbstractDataObject* dataObj)
{
    if (dataObj == nullptr) return nullptr;

    auto adaptor = FITKVIEWADAPTORFACTORY->createT<Exchange::FITKOCCViewAdaptorBase>("GeoModelShapeOCC", dataObj);
    if (adaptor == nullptr) return nullptr;

    adaptor->setDataObject(dataObj);
    if (!adaptor->update())
    {
        delete adaptor;
        return nullptr;
    }

    auto graphObj = adaptor->getOutputData();
    delete adaptor;
    return graphObj;
}
```

### 2. 按注册类名创建测量工具适配器

```cpp
#include "FITK_Kernel/FITKAdaptor/FITKViewAdaptorFactory.h"
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCViewAdaptorBase.h"

Exchange::FITKOCCViewAdaptorBase* createMeasureAdaptor()
{
    return FITKVIEWADAPTORFACTORY->createT<Exchange::FITKOCCViewAdaptorBase>("MeasureToolOCC", "DimensionTools");
}
```

### 3. 更新输出对象的显示状态

```cpp
void updateGraphState(Exchange::FITKOCCGraphObject3D* graphObj)
{
    if (graphObj == nullptr) return;

    graphObj->setVisible(true);
    graphObj->setEnableTransparency(false);
    graphObj->setLineWidth(2.0);
    graphObj->update(true);
}
```

## 注意事项

1. FITKViewAdaptorFactory 的匹配条件是 graphObjType 与 className 两段组合，示例中的字符串必须与注册宏保持一致。
2. MeasureToolOCC 当前在注册宏中的 className 是 DimensionTools，不是 Distance。
3. FITKOCCGraphObject3D 继承自 Render::FITKGraphObjectOCC，因此该模块与 FITKRenderWindowOCC 存在明确的编译期耦合。
4. 该模块输出的是图形对象，不负责对象所有权策略的统一收口；接入方需要根据使用场景管理生命周期。
5. 如果上游数据对象变化后未调用 update，窗口层只会继续显示旧缓存结果。
6. 出现链接问题时，应优先检查 Qt、OCC 以及相关 FITK 内部库在 output 目录中的产物是否完整。

## 文档信息

- 编写日期：2026-04-09
- 最后更新时间：2026-04-23