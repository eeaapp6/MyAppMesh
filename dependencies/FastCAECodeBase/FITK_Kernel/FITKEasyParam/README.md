# FITKEasyParam

## 简介

FITKEasyParam 是 `FITK_Kernel` 中的参数抽象与参数编辑控件模块，用于在 Qt 界面中统一描述“参数数据对象”和“对应编辑控件”。

该模块的核心价值是把参数语义（如 `int`、`double`、`bool`、`combox`、分组参数）与界面控件解耦，便于上层模块按统一方式创建、复制、管理参数，并生成可编辑的 QWidget。

## 模块定位

- 所在层级：`FITK_Kernel` 底座子模块
- 构建产物：共享库 `FITKEasyParam`
- 主要作用：
  - 提供参数数据抽象基类与多种具体参数类型
  - 提供参数集合管理类
  - 提供“参数对象 -> Qt 控件”的工厂映射

## 架构概览

```text
FITKEasyParam
├── 参数抽象层
│   ├── FITKAbstractEasyParam
│   └── 具体参数类
│       ├── FITKParamInt / FITKParamDouble / FITKParamString
│       ├── FITKParamBool / FITKParamCombox
│       ├── FITKParamDoubleList / FITKParamComboxVector
│       ├── FITKParamBoolGroup / FITKParamRadioGroup
│       └── FITKParamLabel
├── 参数集合层
│   └── FITKParameter (参数创建、复制、查询)
├── 控件抽象与控件实现层
│   ├── FITKAbstractEasyParamWidget
│   └── FITKWidgetInt / FITKWidgetDouble / FITKWidgetComBox ...
└── 工厂层
    ├── FITKEasyParamFactory
    └── FITKEasyParamWidgetFactory
```

## 依赖关系

根据 `FITK_Kernel/FITKEasyParam/CMakeLists.txt` 当前实现，模块依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore（通过 `link_fitk_library` 链接）

同时，模块源码中使用了 `FITKAbstractNDataObject` 与 `FITKAbstractDataManager`（来自 FITKCore），因此其数据对象体系建立在 FITKCore 之上。

## 核心类型说明

### 1. FITKAbstractEasyParam

参数数据基类，继承自 `FITKAbstractNDataObject`，定义统一参数类型枚举 `FITKEasyParamType`，包括：

- `FEPBool`
- `FEPBoolGroup`
- `FEPCombox`
- `FEPComboxVector`
- `FEPDouble`
- `FEPDoubleList`
- `FEPInt`
- `FEPLabel`
- `FEPRadioGroup`
- `FEPString`

并提供通用数据存取接口：

- `getData()` / `setData()`
- `getDataTitle()`
- `sigDataChange` 信号

### 2. FITKParameter

参数集合管理类，继承 `FITKAbstractDataManager<FITKAbstractEasyParam>`，用于统一创建和管理参数对象。

常用能力：

- 创建参数：
  - `createIntParameter`
  - `createDoubleParameter`
  - `createDoubleListParameter`
  - `createStringParameter`
  - `createBoolParameter`
  - `createBoolGroupParameter`
  - `createComboxParameter`
  - `createComBoxVectorParameter`
  - `createRadioGroupParameter`
- 拷贝与追加：`copyPara`、`appendPara`、`copyAppendPara`
- 按名称读取：`getDoubleParaByName`、`getIntParaByName`、`getBoolParaByName` 等

### 3. FITKEasyParamWidgetFactory

`createWidget(FITKAbstractEasyParam* data, QWidget* parent, QString name)` 根据参数类型创建对应 Qt 控件：

- `FEPInt` -> `FITKWidgetInt`
- `FEPDouble` -> `FITKWidgetDouble`
- `FEPString` -> `FITKWidgetString`
- `FEPCombox` -> `FITKWidgetComBox`
- `FEPBool` -> `FITKWidgetBool`
- `FEPRadioGroup` -> `FITKWidgetRadioGroup`
- `FEPComboxVector` / `FEPDoubleList` -> `FITKWidgetHBox`（包含多个子控件）
- `FEPBoolGroup` -> `FITKWidgetBoolGroup`

### 4. FITKEasyParamFactory

当前 `createParam(FITKEasyParamType type)` 的实现返回 `nullptr`，尚未提供参数对象创建映射。当前工程里更常用 `FITKParameter` 直接创建参数对象。

## 构建与集成

### 1. 作为 `FITK_Kernel` 子模块参与构建

`FITK_Kernel/CMakeLists.txt` 已包含：

- `add_subdirectory(FITKEasyParam)`

因此在顶层工程配置后会自动参与编译。

### 2. 典型构建流程（Windows）

在工程构建目录执行：

```bash
cmake ..
cmake --build . --config Debug
```

产物输出目录由模块 CMake 配置为：

- Debug: `output/bin_d`
- Release: `output/bin`

## 使用方式

### 场景 A：创建参数集合

```cpp
#include "FITKParameter.h"

Core::FITKParameter params;
params.createIntParameter("MaxIter", 100, 100000, 1);
params.createDoubleParameter("CFL", 0.5, 10.0, 1e-6);
params.createBoolParameter("EnableTurbulence", true);
params.createComboxParameter("Scheme", {"FirstOrder", "SecondOrder"}, 1);
```

### 场景 B：从参数对象创建编辑控件

```cpp
#include "FITKEasyParamWidgetFactory.h"
#include "FITKParameter.h"

Core::FITKParameter params;
auto* doubleParam = params.createDoubleParameter("Relax", 0.7, 1.0, 0.0);
QWidget* editor = Core::FITKEasyParamWidgetFactory::createWidget(doubleParam, parentWidget, "Relax");
```

### 场景 C：按名称读取参数值

```cpp
bool ok = false;
double relax = params.getDoubleParaByName("Relax", ok);
if (ok) {
    // use relax
}
```

## 注意事项

- 参数对象由调用方创建并加入管理器后，应保持明确的生命周期管理策略。
- `FITKAbstractEasyParam::setData(QVariant)` 可存储指针型 QVariant，相关内存需要调用方自行管理。
- `FITKEasyParamWidgetFactory` 在接收空指针或类型不匹配时会返回空控件，需要调用方判空。
- 当前 `FITKEasyParamFactory` 尚未完成参数类型映射创建，不建议将其作为主要创建入口。


最后更新时间：2026-04-18
