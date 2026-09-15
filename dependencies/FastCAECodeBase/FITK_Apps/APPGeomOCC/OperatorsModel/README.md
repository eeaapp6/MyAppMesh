# OperatorsModel

## 简介

OperatorsModel 是 CADOCC 应用中负责建模、文件读写、导入导出、撤销重做以及参数对话框驱动的操作器实现模块。

它与 OperatorsGUI 的区别在于：

- OperatorsGUI 更关注图形与界面交互。
- OperatorsModel 更关注数据对象、几何命令和业务执行。

该模块是当前应用业务动作最集中的地方之一。

它解决的是“菜单动作或参数窗口确认后，究竟由哪些具体操作器去创建、编辑、删除、导入、导出或更新几何命令”的问题。

## 模块定位

- 所在目录：OperatorsModel
- 构建产物：共享库 OperatorsModel
- 主要使用者：CADOCCApp 组件工厂、GUIDialog、工作台处理器、主界面动作系统
- 主要上游依赖：FITKCore、FITKAppFramework、FITKInterfaceModel、FITKInterfaceGeometry、FITKInterfaceGeoIOHDF5、FITKGeoOCCIOHDF5、FITKRenderWindowOCC、OperatorsInterface、GUIWidget、GUIFrame、GUIDialog、PickDataProvider、Qt

## 架构与目录

当前目录包含多个操作器家族，可按职责分组理解：

```text
OperatorsModel
├── 模块入口
│   └── OpersModelInterface
├── 公共基类
│   ├── OperManagerBase
│   └── OperCreateGeoManagerBase
├── 文件与工程操作
│   ├── OperImportManager
│   ├── OperExportManager
│   ├── OperOpenHDF5
│   └── OperSaveHDF5
├── 几何创建/编辑/布尔/特征
│   ├── OperPrimitiveManager
│   ├── OperPointManager
│   ├── OperCurveManager
│   ├── OperSurfaceManager
│   ├── OperSolidManager
│   ├── OperBoolManager
│   ├── OperComponentManager
│   ├── OperFeatureManager
│   ├── OperCompoundManager
│   └── 多个辅助管理器
├── 工具与状态控制
│   ├── OperUndoRedo
│   ├── OperFaceHidden
│   ├── OperSketchManager
│   └── OperTransformationManager
└── 构建文件
```

这说明 OperatorsModel 并不是单一操作器，而是一整套业务操作器集合。

## 依赖关系

根据 OperatorsModel/CMakeLists.txt，当前显式链接：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- FITKAppFramework
- FITKInterfaceModel
- FITKInterfaceGeometry
- FITKInterfaceGeoIOHDF5
- FITKGeoOCCIOHDF5
- FITKRenderWindowOCC
- OperatorsInterface
- GUIWidget
- GUIFrame
- GUIDialog
- PickDataProvider

这表明该模块同时依赖：

- 数据对象层
- 几何命令层
- 工程 IO 层
- 参数窗口层
- 图形与拾取层

## 主要类与职责

### OpersModelInterface

OpersModelInterface 继承自 AppFrame::FITKComponentInterface。

它是模块组件入口。

当前真实接口包括：

- void init() override
- QString getComponentName() override

当前实现里 getComponentName() 返回：

- TianjianModelOpers

### OperManagerBase

OperManagerBase 继承自 EventOper::ParaWidgetInterfaceOper。

它是多数业务操作器的基类。

当前头文件中可确认的公共/受保护能力包括：

- printLog(int type, QString msg)
- deleteCmd(int id)
- exitSketchMode()
- updateTree()
- preArgs() override
- preEmitter()
- preItemArgs()

它还维护：

- m_OperType
- m_MainWindow
- m_Id
- m_AddtionalInfo

因此它承担了“参数预处理、主窗口上下文、日志输出和操作类型识别”的公共职责。

### OperCreateGeoManagerBase

OperCreateGeoManagerBase 继承自 OperManagerBase，是几何创建相关操作器基类。

当前确认存在的关键接口包括：

- execGUI() override
- execProfession() override
- finalize(bool updateCommand = false) override
- updateCommandRecursively(Interface::FITKAbsGeoCommand* cmd)
- setRefEnableRecursively(Interface::FITKAbsGeoCommand* cmd, bool flag, bool recDown = true)
- showDialog()
- template<typename T, typename T2 = Interface::FITKAbsGeoCommand> T* initDialog()

此外，它还定义了 DefineGeometryOperator 宏，自动为某一类几何操作器注册：

- thisClass
- actionName
- actionNameCreate
- actionNameEdit
- actionNameDelete
- actionNameHide
- actionNameShow
- actionNameRename

这是当前模块的重要实现模式。

### OperOpenHDF5

OperOpenHDF5 继承自 OperManagerBase。

它通过以下注册项暴露给系统：

- actionOpenProject

并重写：

- execGUI()
- execProfession()

## 构建与集成

OperatorsModel 通过 CMake 构建为共享库：

- project(OperatorsModel VERSION 1.0.0 LANGUAGES CXX)
- add_library(${PROJECT_NAME} SHARED)
- 启用 Qt 自动生成工具
- 包含翻译文件
- 输出目录为 output/bin_d 与 output/bin

## 使用示例

### 场景 1：通过仓库获取打开工程操作器

```cpp
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

auto* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionOpenProject");
if (oper)
{
    oper->setArgs("FileName", "demo.hdf5");
    oper->execProfession();
}
```

### 场景 2：通过仓库获取保存工程操作器

```cpp
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

auto* oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionSaveProject");
if (oper)
{
    oper->setArgs("FileName", "demo_out.hdf5");
    oper->execProfession();
}
```

### 场景 3：几何创建类操作器初始化参数窗口

下面示例展示 OperCreateGeoManagerBase 的真实扩展点，只使用已确认存在的接口名。

```cpp
class MyCreateOper : public ModelOper::OperCreateGeoManagerBase
{
public:
    bool execProfession() override
    {
        return OperCreateGeoManagerBase::execProfession();
    }

protected:
    void showDialog() override
    {
        // 具体对话框类型由派生类决定
    }
};
```

## 注意事项

- OperatorsModel 中很多动作是通过宏批量注册的，文档编写时必须以头文件中的真实宏展开规则为准。
- OperOpenHDF5 与 OperSaveHDF5 使用的参数键分别是 FileName 等字符串，若文档示例要写 setArgs，需要保持和实际调用一致。
- OperCreateGeoManagerBase::initDialog 会主动关闭其他对话框并清理拾取状态，这说明它不仅是参数窗口工厂，还承担交互状态协调。

最后更新时间：2026-04-23