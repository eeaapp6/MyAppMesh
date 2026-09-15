# PythonInterface

## 简介

PythonInterface 是 CADOCC 应用中面向 PythonQt 的业务包装层模块。

它不负责嵌入 Python 解释器本身。

解释器和脚本环境由 FITKPython 模块提供。

PythonInterface 的职责是在此基础上，把当前应用的几何对象、文件操作与局部建模动作包装成 PythonQt 可见的类和 decorator。

它解决的是“脚本层如何以较稳定的类接口访问应用中的建模能力，而不是直接暴露复杂底层 C++ 类型”的问题。

## 模块定位

- 所在目录：PythonInterface
- 构建产物：共享库 PythonInterface
- 主要使用者：CADOCCApp 中的 PyRegister、FITKPython 脚本注册流程
- 主要上游依赖：FITKCore、FITKPython、OperatorsInterface、OperatorsModel、OperatorsGUI、FITKAppFramework、GUIFrame、GUIWidget、FITKInterfaceGeometry、FITKGeoCompOCC、Python37、PythonQt、Qt

## 架构与目录

从当前目录结构看，模块主要包含三类内容：

```text
PythonInterface
├── 公共包装基础
│   ├── WrapperTools
│   ├── WrapperDataObject
│   └── WrapperDataObjectWrapper
├── 面向脚本的数据包装类
│   ├── Point / PointWapper
│   ├── Solid / SolidWapper
│   ├── Box / BoxWrapper
│   ├── Chamfer / ChamferWrapper
│   ├── AppendCompound / AppendCompoundWrapper
│   └── RemoveCompound / RemoveCompoundWrapper
├── 文件相关包装
│   └── Files / FilesWapper
└── 构建文件
```

从 PyRegister.cpp 当前实现可以确认，真正被注册到脚本环境的类包括：

- Point
- Solid
- Box
- Chamfer
- AppendCompound
- RemoveCompound

而 FilesWapper 目前存在于模块中，但未在当前 PyRegister 中注册。

## 依赖关系

根据 PythonInterface/CMakeLists.txt，当前显式链接：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- FITKPython
- OperatorsInterface
- OperatorsModel
- OperatorsGUI
- FITKAppFramework
- GUIFrame
- GUIWidget
- FITKInterfaceGeometry
- FITKGeoCompOCC
- Python37::Python37
- PythonQt::PythonQt

这说明 PythonInterface 同时依赖：

- Python 运行与绑定层
- GUI/操作器体系
- 几何命令体系

## 主要类与职责

### WrapperTools

WrapperTools 是装饰器公用工具类。

当前可确认接口包括：

- Interface::FITKAbsGeoCommand* getGeoCommand(QString enumName)
- Interface::FITKAbsGeoCommand* getGeoCommand(Interface::FITKGeoEnum::FITKGeometryComType t)
- void updateExecutionStatus(bool isStatus)
- void printConsoleLog(const QString& str)

它用于在脚本包装类中复用命令获取、执行状态更新和日志输出逻辑。

### WrapperDataObject

WrapperDataObject 是脚本可操作数据对象的公共基类。

当前确认接口包括：

- virtual bool update()
- virtual const QString getClassName() = 0
- virtual Interface::FITKAbsGeoCommand* getCommand()
- virtual void setName(const QString& name)
- virtual QString getName()

它持有 m_command 与 WrapperTools 实例，说明脚本对象最终仍然围绕几何命令对象展开。

### Point 与 PointWapper

Point 是点对象封装类，当前接口包括：

- Point(int id = 0)
- Point(Interface::FITKAbsGeoModelPoint* p)
- int getPointID()
- QString getName() const
- void reName(const QString& name)
- Point* getPoint(const QString& pName)

PointWapper 公开的 PythonQt decorator 接口包括：

- new_Point
- delete_Point
- static_Point_SetCurrentPoint
- static_Point_GetCurrentPoint
- static_Point_CreatePoint
- static_Point_GetPoint
- reName
- getName
- getPoint

### Solid 与 SolidWapper

Solid 是实体对象/命令包装类。

当前接口包括：

- Solid(int id = 0)
- Solid(Interface::FITKAbsGeoCommand* command)
- bool boxParameterInit(...)
- bool sphereParameterInit(...)
- bool chamferParameterInit(...)
- QString getName()

SolidWapper 暴露的装饰接口包括：

- static_Solid_CreateBox
- static_Solid_CreateSphere
- delete_Solid
- getName
- static_Solid_CreateChamfer

### Box 与 BoxWrapper

Box 继承自 WrapperDataObject。

当前接口包括：

- getClassName
- setPoint1
- setPoint2
- setDimensions

BoxWrapper 当前装饰接口包括：

- new_Box
- delete_Box
- deleteCmd
- setPoint1
- setPoint2
- setDimensions
- setPoints
- update
- static_Box_Create

### Chamfer / AppendCompound / RemoveCompound

这三个类都继承自 WrapperDataObject，并分别提供对应几何编辑操作的脚本包装。

当前确认的关键 decorator 接口包括：

- ChamferWrapper::static_Chamfer_Create
- AppendCompoundWrapper::static_AppendCompound_Create
- RemoveCompoundWrapper::static_RemoveCompound_Create

以及若干 setInputCmdName、setDistance、setAppendCmdName、setRemoveIndex、update 等成员。

## 构建与集成

PythonInterface 通过 CMake 构建为共享库：

- project(PythonInterface VERSION 1.0.0 LANGUAGES CXX)
- add_library(${PROJECT_NAME} SHARED)
- 依赖 Python37::Python37 与 PythonQt::PythonQt
- 输出目录为 output/bin_d 与 output/bin

这说明它属于“脚本包装模块”，不是独立 Python 程序。

## 使用示例

### 场景 1：在 C++ 侧包装并更新 Box

```cpp
#include "PythonInterface/PrimitivesBox.h"

Box* box = nullptr;
BoxWrapper wrapper;

box = wrapper.new_Box();
wrapper.setPoint1(box, {0.0, 0.0, 0.0});
wrapper.setPoint2(box, {10.0, 20.0, 30.0});
wrapper.update(box);
```

### 场景 2：在 C++ 侧配置 Chamfer 包装对象

```cpp
#include "PythonInterface/Chamfer.h"

ChamferWrapper wrapper;
Chamfer* chamfer = wrapper.static_Chamfer_Create();

wrapper.setInputCmdName(chamfer, "Box-1");
wrapper.setEdgeIndice(chamfer, {1, 2});
wrapper.setDistance(chamfer, 2.0);
wrapper.update(chamfer);
```

### 场景 3：在脚本注册器中暴露包装类

下面的示例直接对应当前 PyRegister.cpp 的真实用法：

```cpp
_pyInterface->addDecorator(new BoxWrapper());
_pyInterface->registerCPPClass("Box", "WrapperDataObject", "Global");
```

## 注意事项

- PointWapper 类名拼写沿用现有代码中的 Wapper，而不是 Wrapper，README 不能擅自改写。
- 是否能在脚本中直接调用某个类，不能只看头文件是否存在，还要看 PyRegister 是否实际注册。
- Python 包装对象最终仍依赖底层几何命令和 UI 更新链路，因此并非完全脱离 GUI 的纯数据 API。

最后更新时间：2026-04-23