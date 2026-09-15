# GUIDialog

## 简介

GUIDialog 是 CADOCC 应用的参数对话框集合模块。

它集中承载建模、布尔、基准元素、曲线、曲面、实体、修复、拆分、组合等操作所需的 Qt 对话框。

和 GUIWidget 提供的通用基类不同，GUIDialog 更接近“具体业务参数界面实现”。

该模块不直接注册应用组件，也不创建主窗口。

它解决的是“具体几何操作需要哪些参数输入界面，以及这些界面如何统一嵌入本应用的操作器、图形预览和参数编辑机制”的问题。

## 模块定位

- 所在目录：GUIDialog
- 构建产物：共享库 GUIDialog
- 主要使用者：OperatorsModel、主界面相关业务流程
- 主要上游依赖：FITKAppFramework、FITKCore、FITKPython、FITKEasyParam、FITKWidget、GUIWidget、GUIFrame、FITKInterfaceGeometry、FITKInterfaceModel、PickDataProvider、Qt

从目录规模可以直接看出，GUIDialog 是应用中体量最大的界面参数模块之一。

## 架构与目录

当前模块由大量具体对话框类组成，按功能大致可以分为以下几类：

```text
GUIDialog
├── 基础与公共设置
│   └── ComponentInfoDialog
├── 基础几何与基准元素
│   ├── PrimitivesBoxInfoDialog
│   ├── PrimitivesCylinderInfoDialog
│   ├── PrimitivesSphereInfoDialog
│   ├── ReferencePointInfoDialog
│   ├── ReferenceLineInfoDialog
│   └── ReferencePlaneInfoDialog
├── 曲线相关
│   ├── Curve3DBSplineInfoDialog
│   ├── CurveBridgeCurveInfoDialog
│   ├── CurveOffsetCurveInfoDialog
│   ├── CurveTwoPointsSegmentInfoDialog
│   └── 多个三点曲线与投影/交线对话框
├── 曲面与实体相关
│   ├── SurfaceExtrudeInfoDialog
│   ├── SurfaceSweepInfoDialog
│   ├── SurfaceOffsetInfoDialog
│   ├── SolidExtrudeInfoDialog
│   ├── SolidRevolInfoDialog
│   ├── SolidSweepInfoDialog
│   └── MultiSection / Splitter 等对话框
├── 编辑与修复相关
│   ├── ChamferInfoDialog
│   ├── FilletInfoDialog
│   ├── MergeEdgeDialog
│   ├── DeleteFloatingEdgeDialog
│   ├── FillFaceInfoDialog
│   └── DefeatureInfoDialog
├── 布尔与组合操作
│   ├── BoolOperatorInfoDialog
│   ├── AppendCompoundDialog
│   ├── RemoveCompoundDialog
│   └── NBodyUniteInfoDialog
└── 其他功能
    ├── FaceHiddenDialog
    ├── TransformationInfoDialog
    └── ImprintSolidDialog
```

模块中的每个具体对话框通常都有对应的：

- .h
- .cpp
- .ui

这种组织方式说明 GUIDialog 以“界面类 + UI 文件”作为主要交付形态。

## 依赖关系

根据 GUIDialog/CMakeLists.txt，模块显式链接：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKAppFramework
- FITKCore
- FITKPython
- FITKEasyParam
- FITKWidget
- GUIWidget
- GUIFrame
- FITKInterfaceGeometry
- PickDataProvider
- FITKInterfaceModel

这与模块职责高度一致：

- 依赖 FITKEasyParam，说明参数编辑控件复用底层参数系统。
- 依赖 GUIWidget 和 GUIFrame，说明对话框直接接入主界面。
- 依赖 PickDataProvider，说明拾取参数可直接回填到界面。

## 主要类与职责

GUIDialog 当前没有单独的“总控类”，而是由大量具体业务对话框组成。

在编写和理解本模块 README 时，更合适的方式不是逐个罗列所有 50 多个窗口，而是按职责归组。

### 基础原语与基准元素对话框

这类对话框用于创建或编辑基础几何与基准元素：

- PrimitivesBoxInfoDialog
- PrimitivesCylinderInfoDialog
- PrimitivesSphereInfoDialog
- ReferencePointInfoDialog
- ReferenceLineInfoDialog
- ReferencePlaneInfoDialog

这类窗口通常会和对应的建模操作器配对，负责采集位置、尺寸、方向和命名参数。

### 曲线、曲面、实体对话框

这类对话框负责较复杂的几何构造操作，例如：

- Curve3DBSplineInfoDialog
- CurveBridgeCurveInfoDialog
- CurveCurveProjSurfaceInfoDialog
- SurfaceExtrudeInfoDialog
- SurfaceSweepInfoDialog
- SolidExtrudeInfoDialog
- SolidRevolInfoDialog
- SolidSweepInfoDialog

它们通常需要同时依赖：

- 参数输入
- 几何拾取
- 预览更新

### 修复、编辑、组合类对话框

这类对话框包括：

- ChamferInfoDialog
- FilletInfoDialog
- MergeEdgeDialog
- DeleteFloatingEdgeDialog
- FillFaceInfoDialog
- AppendCompoundDialog
- RemoveCompoundDialog
- NBodyUniteInfoDialog

这一类窗口往往会和已选实体、拓扑索引、长度/半径等操作参数一起使用。

### 隐藏、变换与辅助对话框

例如：

- FaceHiddenDialog
- TransformationInfoDialog
- ComponentInfoDialog

这些窗口通常承担“局部编辑”或“辅助信息配置”的职责。

## 构建与集成

GUIDialog 通过 CMake 构建为共享库：

- project(GUIDialog VERSION 1.0.0 LANGUAGES CXX)
- add_library(${PROJECT_NAME} SHARED)
- 启用 AUTOMOC、AUTOUIC、AUTORCC
- 编译输入包含大量 .ui 文件与翻译文件
- 输出目录为 output/bin_d 与 output/bin

该模块同样保留了 qmake 与 VS 工程文件，属于现有应用兼容多工程体系的一部分。

## 模块层级关系

当前仓库中，GUIDialog 所处的位置可以概括为：

1. OperatorsModel 负责业务操作器与参数逻辑。
2. GUIDialog 负责对应操作器的用户输入窗口。
3. PickDataProvider 为对话框提供拾取结果。
4. GUIWidget 的 GUIDialogBase 提供公共窗口行为。
5. GraphEventOperator / GraphToolOperator / GraphInteractionOperator 支持预览与拾取联动。

## 使用示例

### 场景 1：在具体业务对话框中复用公共执行入口

由于 GUIDialog 下具体窗口众多，这里示例使用经过代码确认的公共基类接口。

```cpp
#include "GUIWidget/GUIDialogBase.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

GUI::GUIDialogBase* dialog = /* 某个具体业务对话框 */ nullptr;
EventOper::ParaWidgetInterfaceOper* oper = /* 对应操作器 */ nullptr;

dialog->setOperator(oper);
bool ok = dialog->execProfession();

Q_UNUSED(ok)
```

### 场景 2：在参数变化后刷新预览

```cpp
#include "GUIWidget/GUIDialogBase.h"

class PreviewDialog : public GUI::GUIDialogBase
{
public:
    void refreshPreview(int dataId)
    {
        previewGraph(dataId, EventOper::PT_Default);
    }

    void clearAllPreview()
    {
        clearPreview();
    }
};
```

### 场景 3：输出日志并驱动图形更新

```cpp
class LoggingDialog : public GUI::GUIDialogBase
{
public:
    void onCommandFinished(int cmdId)
    {
        printLog(1, "命令执行完成");
        updateGraph(cmdId);
    }
};
```

这些示例之所以采用 GUIDialogBase，是因为本模块的大量具体类最终都围绕它展开，而这部分接口已被直接确认存在。

## 注意事项

- GUIDialog 的具体对话框数量很多，README 不适合机械列出全部成员函数，否则会退化成 UI 清单。
- 模块依赖 PickDataProvider 和 FITKEasyParam，说明对话框逻辑通常包含拾取与参数控件双重状态，需要注意窗口关闭时的清理。
- 若新增对话框，应优先沿用 GUIWidget 中的 GUIDialogBase，而不是新建一套并行基类。
- 当前仅核对了接口存在性和构建事实，具体窗口字段与业务规则仍应以各 .ui 与 .cpp 实现为准。

最后更新时间：2026-04-23