# FITKPython

## 简介

FITKPython 是应用程序中的 Python 脚本接入模块，负责把 PythonQt、Python 运行环境和应用程序框架连接起来，为上层应用提供脚本执行、C++ 类注册、装饰器注册、脚本控制台和脚本环境清理等能力。它本身不是一个独立脚本应用，而是 `FITKAppFramework` 的脚本扩展层。

相对于 `FITKCore` 和 `FITKAppFramework`，FITKPython 解决的是“如何在当前 Qt/C++ 应用中嵌入 Python 解释执行能力，并把业务对象暴露给脚本”的问题。

## 模块定位

FITKPython 位于 `FITK_Kernel` 下，属于脚本接入层模块，主要职责包括：

- 提供 `FITKPythonInterface` 单例作为应用侧 Python 入口。
- 使用 `PythonAgent` 封装 PythonQt 初始化、脚本执行和控制台交互。
- 支持向 PythonQt 注册 C++ 类和 decorator 对象。
- 提供脚本文件执行、单行脚本执行、历史记录保存和 Python 控制台界面。
- 与 `FITKAppFramework` 的 `FITKAppScriptHelper`、`FITKAbstractPythonRegister` 配合，把脚本能力接入应用程序生命周期。

## 架构与目录

从当前目录中的头文件和源文件看，FITKPython 的结构比较集中，主要包含以下内容：

```text
FITKPython
├── FITKPythonAPI.h
├── FITKPythonInterface.h / .cpp
├── PythonAgent.h / .cpp
├── FITKPythonQtScriptingConsole.h / .cpp
├── PythonCode.qrc
├── PythonCode/
│   └── clear.py
├── CMakeLists.txt
├── FITKPython.pro
└── FITKPython.pri
```

## 依赖关系

### 编译依赖

根据 `FITK_Kernel/FITKPython/CMakeLists.txt`，该模块当前显式依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- FITKAppFramework
- PythonQt
- Python 3.7

其中 `TOOLS_DIR` 是必需配置项，当前 CMake 会从环境变量或 `-DTOOLS_DIR=...` 中读取 PythonQt 和 Python 头文件、库文件路径。

### 平台差异

- Windows 下链接 `PythonQt-Qt5-Python3.7`、`PythonQt_QtAll-Qt5-Python3.7` 和 `python37.lib` / `python37_d.lib`。
- Linux 下链接 `libPythonQt-Qt5-Python.so` 和 `libPythonQt_QtAll-Qt5-Python.so`。
- 在 `FITKPythonInterface` 构造中，Linux 平台还会设置 `PYTHONHOME` 和 `PYTHONPATH`。

### 模块层级关系

- FITKPython 直接依赖 `FITKCore` 与 `FITKAppFramework`。
- 应用程序通过 `PyRegister` 把业务对象包装类注册到脚本环境。
- GUI 操作器、插件菜单和消息视图等上层代码会通过 `FITKPythonInterface` 执行脚本、清理环境或访问控制台。

## 主要类与功能说明

### `FITKPythonInterface`

这是模块最核心的对外入口类，采用单例方式提供 Python 能力。它负责：

- 获取唯一实例 `getInstance()`。
- 注册 decorator 对象 `addDecorator()`。
- 注册 C++ 类到 PythonQt `registerCPPClass()`。
- 执行单行脚本、脚本组和脚本文件 `submit()` / `submitFile()`。
- 保存脚本历史、导入 Python 包、获取脚本控制台界面。
- 清理 Python 环境 `claerPythonEnvoirment()`。
- 保存执行状态和错误信息。

它在首次创建时还会从 `FITKAPP->getScriptHelper()` 注册 `ExecScript` 回调，使应用框架可以通过脚本助手调度脚本执行。

### `PythonAgent`

这是 PythonQt 的代理封装类，负责模块内部的大部分实际执行逻辑。它完成：

- PythonQt 初始化。
- Python 控制台实例创建。
- `sys.path` 追加和脚本搜索路径设置。
- 脚本执行、脚本文件执行和带返回值脚本执行。
- 脚本参数注入 `sys.argv`。
- 脚本历史保存与环境清理。

从职责上看，`FITKPythonInterface` 更偏应用侧 API，`PythonAgent` 更偏 PythonQt 执行代理。

### `FITKPythonQtScriptingConsole`

这是对 `PythonQtScriptingConsole` 的定制封装，用于提供当前应用里的 Python 交互控制台，并重写键盘事件行为。

### `ExecScript`

这是暴露给框架层的脚本执行函数。当前实现非常直接：调用 `FITKPythonInterface::getInstance()->submitFile(file)`。

## 在仓库中的实际用法

### 1. 在 `StructuralApp` 启动时注册 Python 接口

`StructuralApp/main.cpp` 中，应用会把 `PyRegister` 注册到框架：

```cpp
app.regPythonRegister(new PyRegister);
```

这说明 FITKPython 在本项目中的典型接入方式，是通过 `FITKAppFramework` 的注册机制，把脚本能力作为应用扩展点接进去。

### 2. 在 `PyRegister` 中暴露业务包装类

`StructuralApp/PythonRegister.cpp` 中，会把多个包装类注册到 Python 环境：

```cpp
_pyInterface->addDecorator(new FilesWapper());
_pyInterface->registerCPPClass("Files", "", "Global");

_pyInterface->addDecorator(new  CaseWapper());
_pyInterface->registerCPPClass("Case", "", "Global");

_pyInterface->addDecorator(new  PartWapper());
_pyInterface->registerCPPClass("Part", "", "Part");

_pyInterface->importPython();
```

这说明 FITKPython 的一个关键用途，是把当前应用中的 C++ 包装类暴露给 PythonQt，使脚本可以访问全局文件、算例和部件等业务对象。

### 3. 在脚本操作器中清理 Python 环境

`OperatorsGUI/OperScript.cpp` 中，会在清理变量时主动调用：

```cpp
Python::FITKPythonInterface* py = Python::FITKPythonInterface::getInstance();
if (py)py->claerPythonEnvoirment();
```

这说明当前项目确实依赖 FITKPython 提供的环境清理和控制台状态重置能力。

### 4. 在插件菜单中执行 Python 插件脚本

`GUIFrame/MenuPlugin.cpp` 中，会直接调用：

```cpp
Python::FITKPythonInterface::getInstance()->submitFile(plugin,vas);
```

其中 `plugin` 指向 `Plugin/<插件名>/main.py`。这说明 FITKPython 也是当前项目中 Python 插件脚本执行的直接入口。

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `project(FITKPython VERSION 1.0.0 LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`、`AUTOUIC`、`AUTORCC`
- 资源文件包括 `PythonCode.qrc`
- 通过 `link_fitk_library(${PROJECT_NAME} FITKCore)` 和 `link_fitk_library(${PROJECT_NAME} FITKAppFramework)` 链接底层模块
- 依赖 `TOOLS_DIR` 中的 PythonQt 与 Python 运行时资源

### qmake

当前目录中同时保留了 `FITKPython.pro` 和 `FITKPython.pri`，说明该模块仍兼容 qmake 工程组织方式。

### 在仓库中的参与方式

- FITKPython 是 Kernel 层脚本模块，不独立启动。
- `StructuralApp` 通过 `PyRegister` 将业务对象注册到 Python 环境。
- GUI 和插件入口通过 `FITKPythonInterface` 执行脚本文件或清理环境。

## 使用方法

当前仓库中，FITKPython 的典型使用方式主要有三类：

1. 通过 `FITKPythonInterface::getInstance()` 获取 Python 单例入口。
2. 通过 `addDecorator()` 和 `registerCPPClass()` 把 C++ 包装类暴露给 PythonQt。
3. 通过 `submit()` / `submitFile()` 执行脚本，或通过 `getPyWidget()` 获取脚本控制台界面。

## 业务类包装与 Python 调用

这一部分对应当前项目里最重要的实际场景：把现有业务类包装成 PythonQt 可调用对象，然后在 Python 脚本里直接操作它们。

### 包装规则

FITKPython 当前采用的是 PythonQt decorator 机制。根据 `PythonQtDoc.h` 中的规则，以及本仓库 `FilesWapper`、`CaseWapper`、`PartWapper` 的写法，命名约定如下：

- `new_ClassName(...)`：为 `ClassName` 提供构造函数。
- `delete_ClassName(ClassName* obj)`：为 `ClassName` 提供析构函数。
- `static_ClassName_MethodName(...)`：为 `ClassName` 提供静态方法，Python 侧可通过类名调用。
- `methodName(ClassName* obj, ...)`：为 `ClassName` 提供实例方法，Python 侧可通过对象实例调用。

例如：

- `static_Files_ImportInpFile(...)` 会映射为 `Files.ImportInpFile(...)`。
- `static_Case_GetCurrentCase()` 会映射为 `Case.GetCurrentCase()`。
- `getName(Case* c)` 会映射为 `caseObj.getName()`。
- `getPart(Case* c, const QString& name)` 会映射为 `caseObj.getPart(name)`。
- `chamfer(Part* p, ...)` 会映射为 `partObj.chamfer(...)`。

### 包装一个业务类的完整步骤

如果要把现有业务能力暴露给 Python，当前仓库里的标准做法可以归纳为四步。

### 1. 先准备一个轻量业务封装类

不要直接把复杂底层对象原样暴露给脚本，通常先做一个面向脚本的轻量包装类。当前仓库里的 `Case` 和 `Part` 都是这种思路。

例如 `Case` 内部持有 `AbaqusData::FITKDataCase*`，对外只暴露脚本需要的少量接口：

```cpp
class Case
{
public:
	Case(int id = 0);
	Case(AbaqusData::FITKDataCase* caseD);

	int getCaseID();
	QString getName() const;
	void reName(const QString &name);
	Part* getPart(const QString & pName);

private:
	AbaqusData::FITKDataCase* _case{};
};
```

这里的关键点是：

- 脚本看到的是 `Case`，而不是整个 Abaqus 数据体系。
- 包装类负责把上层脚本请求翻译成真实业务调用。
- 返回给脚本的对象也继续使用包装类型，例如 `getPart()` 返回的是 `Part*`。

### 2. 再写一个 decorator 类暴露构造、静态方法和实例方法

当前仓库使用 `QObject + public slots` 的 decorator 类来做 PythonQt 映射。`CaseWapper` 是一个很直接的真实示例：

```cpp
class PythonInterfaceAPI CaseWapper : public QObject
{
	Q_OBJECT
public slots:
	Case* new_Case();
	void delete_Case(Case* c);

	void static_Case_SetCurrentCase(QString & caseName);
	Case* static_Case_GetCurrentCase();
	void static_Case_CreateCase(const QString& caseName);
	Case* static_Case_GetCase(const QString& caseName);

	void reName(Case* c, QString& name);
	QString getName(Case* C);
	Part* getPart(Case* c, const QString& name);
};
```

这段代码已经把 Python 端最常用的三类能力分开了：

- 构造与析构：`new_Case`、`delete_Case`
- 类级入口：`Case.CreateCase()`、`Case.GetCurrentCase()`、`Case.GetCase()`
- 对象级调用：`caseObj.getName()`、`caseObj.reName(...)`、`caseObj.getPart(...)`

### 3. 在 decorator 实现中调用真实业务逻辑

包装层不要重复实现业务，只负责转发、做必要的对象转换，以及在需要时刷新 UI。当前 `CaseWapper` 的实现就是这样：

```cpp
void CaseWapper::static_Case_CreateCase(const QString& caseName)
{
	AbaqusData::FITKDataCase* d = new AbaqusData::FITKDataCase;
	d->setDataObjectName(caseName);
	auto abaqusData = AbaqusData::FITKAbaqusData::GetDataFromAppFrame();
	AbaqusData::FITKDataCaseManager* caseMgr = abaqusData->getDataCaseManager();
	caseMgr->appendDataObj(d);
	updateUI(d->getDataObjectID());
}

Case* CaseWapper::static_Case_GetCurrentCase()
{
	auto abaqusData = AbaqusData::FITKAbaqusData::GetDataFromAppFrame();
	auto currentCase = abaqusData->getCurrentCase();
	if (currentCase == nullptr) return new Case(0);
	return new Case(currentCase);
}

Part* CaseWapper::getPart(Case* c, const QString& name)
{
	if (c) return c->getPart(name);
	return nullptr;
}
```

实际写新包装类时，建议遵守当前项目里已经存在的约束：

- decorator 层负责“脚本接口整形”，不要把完整业务流程堆进 decorator。
- 若业务操作会影响树、菜单或视图，参考 `CaseWapper::updateUI()` 在包装层补一次 UI 同步。
- 若脚本得到的对象后续还要继续被 Python 使用，返回值应继续是包装对象指针，而不是底层裸指针。

### 4. 在 `PyRegister` 中注册到 PythonQt

包装类和 decorator 写好之后，还需要在应用启动时注册。当前项目的真实入口在 `StructuralApp/PythonRegister.cpp`：

```cpp
_pyInterface->addDecorator(new FilesWapper());
_pyInterface->registerCPPClass("Files", "", "Global");

_pyInterface->addDecorator(new CaseWapper());
_pyInterface->registerCPPClass("Case", "", "Global");

_pyInterface->addDecorator(new PartWapper());
_pyInterface->registerCPPClass("Part", "", "Part");

_pyInterface->importPython();
```

这里有两个实际约束需要注意：

- `addDecorator()` 负责把 slot 规则交给 PythonQt。
- `registerCPPClass()` 决定 Python 侧类名和包名，例如 `Files`、`Case` 会进入 `PythonQt.Global`，`Part` 会进入 `PythonQt.Part`。

### 真实示例一：文件类包装和 Python 调用

`FilesWapper` 适合说明“纯工具类”的包装方式，因为它本身几乎不保存状态，而是把脚本请求转发给操作器仓库。

头文件中的典型声明如下：

```cpp
class PythonInterfaceAPI FilesWapper : public QObject
{
	Q_OBJECT
public slots:
	Files* new_Files();
	void delete_Files(Files* F);

	void static_Files_ImportInpFile(const QString& fileName, const QString& level);
	void static_Files_ExportInpFile(const QString& fileName);
	void static_Files_OpenDataBase(const QString& fileName);
};
```

实现中并没有自己做导入导出，而是调用现有操作器：

```cpp
void FilesWapper::static_Files_ImportInpFile(const QString & fileName, const QString& level)
{
	auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionImportAsParts");
	if (oper == nullptr) return;

	QString m = "actionImportAsModel";
	if (level.toLower() == "assembly")
		m = "actionImportAsAssembly";
	else if (level.toLower() == "part")
		m = "actionImportAsParts";

	oper->setArgs("FileName", fileName);
	oper->setArgs("Emitter", m);
	oper->execProfession();
	this->initSleep();
}
```

对应的 Python 调用方式可以写成：

```python
from PythonQt.Global import Files

Files.ImportInpFile(r"D:/demo/bracket.inp", "part")
Files.ExportInpFile(r"D:/demo/out.inp")
Files.OpenDataBase(r"D:/demo/model.hdf5")
```

这个例子体现的是：

- 对于无状态工具能力，优先做成 `static_Class_Method`。
- Python 端直接按类方法调用，不需要先创建对象。

### 真实示例二：算例类包装和 Python 调用

`CaseWapper` 适合说明“业务对象 + 对象实例方法”的包装方式。

当前仓库里，`CaseWapper` 同时提供了创建/查询算例和操作算例对象的能力，因此 Python 端通常先拿到对象，再继续调用实例方法：

```python
from PythonQt.Global import Case

Case.CreateCase("Case_A")

case_obj = Case.GetCurrentCase()
print(case_obj.getName())

case_obj.reName("Case_Renamed")

same_case = Case.GetCase("Case_Renamed")
print(same_case.getName())
```

如果要从算例继续拿到部件对象，则可以继续链式调用：

```python
from PythonQt.Global import Case

case_obj = Case.GetCurrentCase()
part_obj = case_obj.getPart("Part-1")
print(part_obj.getName())
```

这一类包装的设计重点是：

- 类静态方法负责查找、创建、切换业务对象。
- 实例方法负责围绕当前对象继续做细化操作。
- Python 侧看到的是稳定的脚本对象，不需要直接理解底层 manager 体系。

### 真实示例三：部件类包装和 Python 调用

`PartWapper` 展示的是“几何/网格操作对象”的包装方式。它暴露了大量几何编辑方法，例如：

```cpp
Part* new_Part();
Part* static_Part_GetPart(const QString& name);
void chamfer(Part* P, int index, double length = 1.0, const QString& name = QString());
void extrudeSolidByLength(Part* P, const QVector<double>& direction, double depth, const QString& name = QString());
void datumPoint(Part* P, double x, double y, double z, const QString& name = QString());
```

对应的 Python 侧调用形态应当是实例方法风格：

```python
from PythonQt.Part import Part

part_obj = Part.GetPart("Part-1")
print(part_obj.getName())

part_obj.chamfer(3, 2.0, "Chamfer-1")
part_obj.datumPoint(0.0, 0.0, 10.0, "DP-1")
part_obj.extrudeSolidByLength([0.0, 0.0, 1.0], 20.0, "Boss-1")
part_obj.update()
```

这里需要注意两点：

- `QVector<double>`、`QList<int>` 这类参数在 Python 侧通常直接传 list 即可。
- 像 `chamfer(...)`、`extrudeSolidByLength(...)` 这类方法，本质上是在操作当前 `Part` 包装对象，因此应该通过实例而不是类去调用。

### 新增业务包装类时的推荐模板

如果后续要新增一个新的业务类，例如 `Material`，建议直接沿用当前仓库已经验证过的模式：

```cpp
class Material
{
public:
	Material(FITKMaterialData* data = nullptr);
	QString getName() const;
	void setName(const QString& name);

private:
	FITKMaterialData* _data{};
};

class MaterialWapper : public QObject
{
	Q_OBJECT
public slots:
	Material* new_Material();
	void delete_Material(Material* m);

	Material* static_Material_GetMaterial(const QString& name);
	Material* static_Material_GetCurrentMaterial();

	QString getName(Material* m);
	void setName(Material* m, const QString& name);
};
```

注册方式保持一致：

```cpp
_pyInterface->addDecorator(new MaterialWapper());
_pyInterface->registerCPPClass("Material", "", "Global");
```

Python 调用方式则自然对应为：

```python
from PythonQt.Global import Material

mat = Material.GetMaterial("Steel")
print(mat.getName())
mat.setName("Steel_Q235")
```

## 使用示例

### 1. 执行脚本文件

```cpp
Python::FITKPythonInterface::getInstance()->submitFile(plugin, vas);
```

### 2. 注册业务包装类

```cpp
_pyInterface->addDecorator(new FilesWapper());
_pyInterface->registerCPPClass("Files", "", "Global");
_pyInterface->importPython();
```

### 3. 清理 Python 环境

```cpp
Python::FITKPythonInterface* py = Python::FITKPythonInterface::getInstance();
if (py) py->claerPythonEnvoirment();
```

## 使用建议

- 新增 Python 可调用能力时，优先通过包装类和 `registerCPPClass()` 暴露，而不是直接让脚本依赖底层复杂 C++ 类型。
- 若要让脚本入口纳入应用生命周期，优先通过 `FITKAppFramework` 的 `regPythonRegister()` 和 `FITKAppScriptHelper` 协同接入。
- 若执行脚本前需要访问同目录其他 `.py` 文件，优先通过 `submitFile()`，因为当前实现会自动把脚本目录加入 `sys.path`。

## 注意事项

- 当前模块依赖 PythonQt 和 Python 3.7 运行时，缺少 `TOOLS_DIR` 配置时无法构建。
- Linux 平台会在模块初始化时修改 Python 相关环境变量，应确保部署目录结构与当前代码约定一致。
- 当前对脚本参数的支持只覆盖 `Bool`、`Int`、`Double`、`String` 四类 `QVariant`。
- 模块说明应聚焦“脚本接入层”和“PythonQt 集成”，不宜把具体业务包装类的能力误写成 FITKPython 自身职责。

最后更新时间：2026-04-17
