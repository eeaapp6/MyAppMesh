# FITKAdaptor

## 简介

FITKAdaptor 是 APPStructural 中的数据适配层，负责在“业务数据对象”和“具体 IO / 视图实现”之间建立可注册、可查找、可替换的适配关系。它本身不直接提供 Abaqus、HDF5、VTK 或其他具体格式能力，而是提供一套统一的适配器抽象与工厂注册机制，供上层接口模块、组件模块和图形模块接入自己的实现。

相对于 `FITKCore` 提供的对象模型，FITKAdaptor 解决的是“如何根据文件类型、图元类型和数据对象类型，找到对应的读写适配器或显示适配器”的问题。当前仓库中的 `APPTestModelDataComparisonTool` 和 `GraphDataProvider` 都在真实使用它。

## 模块定位

FITKAdaptor 位于 `FITK_Kernel` 下，属于机制层模块，主要职责包括：

- 定义统一的适配器基类 `FITKAbstractAdaptor`。
- 区分 IO 适配器和 View 适配器两类适配能力。
- 提供 `FITKIOAdaptorFactory` 和 `FITKViewAdaptorFactory` 两个单例工厂。
- 通过注册宏把具体适配器接入工厂，而不是把具体实现写死在框架里。
- 让上层业务通过“文件类型 / 图元类型 + 数据对象类型”动态获取适配器实例。

## 架构与目录

从当前目录中的头文件和源文件看，FITKAdaptor 的结构比较集中，主要包含以下内容：

```text
FITKAdaptor
├── FITKAdaptorAPI.h
├── FITKAbstractAdaptor.h / .cpp
├── FITKAbstractIOAdaptor.h / .cpp
├── FITKAbstractViewAdaptor.h / .cpp
├── FITKIOAdaptorFactory.h / .cpp
├── FITKViewAdaptorFactory.h / .cpp
├── CMakeLists.txt
├── FITKAdaptor.pro
└── FITKAdaptor.pri
```

## 依赖关系

### 编译依赖

根据 `FITK_Kernel/FITKAdaptor/CMakeLists.txt`，该模块当前显式依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore

当前模块不直接链接 HDF5、VTK、OpenCASCADE 或 Abaqus 相关库，这些具体能力都来自上层模块注册进来的适配器实现。

### 模块层级关系

- FITKAdaptor 直接依赖 `FITKCore`。
- `FITK_Interface` 中的 IO 模块会用它注册 HDF5、INP 等具体读写适配器。
- `FITK_Component/FITKVTKGraphAdaptor` 会用它注册 VTK 视图适配器。
- `APPTestModelDataComparisonTool` 和 `GraphDataProvider` 等上层代码在运行时通过工厂创建具体适配器。

## 主要类与功能说明

### `FITKAbstractAdaptor`

所有适配器的共同抽象基类。它定义了三件核心事情：

- 适配器对应哪个数据类 `getAdaptorClass()`。
- 它属于哪一类适配器 `getAdaptorType()`。
- 如何接收输入数据对象并执行 `adapt()`。

它也是 IO 适配器和 View 适配器的共同父类。

### `FITKAbstractIOAdaptor`

面向数据读写流程的适配器基类。它在 `FITKAbstractAdaptor` 基础上进一步定义：

- `adaptR()`：读入适配。
- `adaptW()`：写出适配。

上层具体的 HDF5、INP 等读写适配器都会继承它。

### `FITKAbstractViewAdaptor`

面向图形或显示对象转换的适配器基类。它用于把数据对象转换为可视化对象或图形表示，是图形适配组件扩展的基础入口。

### `FITKIOAdaptorFactory`

IO 适配器工厂，负责按“文件类型 + 数据类名”或“文件类型 + 数据对象实例”创建具体适配器。它是当前仓库里 IO 适配的主要入口。

该工厂通过 `Register2FITKIOAdaptorFactory(...)` 注册宏接收具体适配器实现。

### `FITKViewAdaptorFactory`

视图适配器工厂，负责按“图元类型 + 数据类名”或“图元类型 + 数据对象实例”创建具体视图适配器。图形模块中的可视化对象通常通过它按需创建。

该工厂通过 `Register2FITKViewAdaptorFactory(...)` 注册宏接收具体适配器实现。

## 在仓库中的实际用法

### 1. 在模型比较工具中创建 IO 适配器

`APPTestModelDataComparisonTool/FITKComparisonInfoLog.cpp` 中，工具会按文件类型和数据类名动态创建 Abaqus INP 输出适配器：

```cpp
auto adaptor = FITKIOADAPTORFACTORY->createT<IO::FITKAbaqusAdaptorOutput>("INP", "Interface::FITKAbaAbstractOutput");
if (adaptor == nullptr) return false;

adaptor->setFileWriter(io);
adaptor->setCaseData(caseData);
adaptor->setStepData(stepData);
adaptor->setDataObject(outputData);
bool wOk = adaptor->adaptW();
```

这说明 FITKAdaptor 在当前项目中的一个重要用途，是让业务侧按“格式 + 数据类型”动态找到对应的写出适配器，而不是在业务代码里手写大量类型分支。

### 2. 在图形数据提供器中创建 View 适配器

`GraphDataProvider/GraphProviderBase.cpp` 中，会按图元类型和数据对象动态创建 VTK 视图适配器：

```cpp
Exchange::FITKVTKViewAdaptorBase* adaptor = FITKVIEWADAPTORFACTORY->createT<Exchange::FITKVTKViewAdaptorBase>(adaptorKeyName, dataObj);
if (!adaptor)
{
    return obj;
}

adaptor->setStep(stepId != -1 ? stepId : 0);
adaptor->setCaseData(caseData);
```

这说明 FITKAdaptor 不只服务文件读写，也承担“数据对象到显示对象”的桥接职责。

### 3. 具体适配器通过注册宏接入工厂

在 `FITK_Interface/FITKInterfaceIOHDF5` 中，可以看到很多 IO 适配器通过 `Register2FITKIOAdaptorFactory(...)` 注册到工厂。

在 `FITK_Component/FITKVTKGraphAdaptor` 中，也有大量视图适配器通过 `Register2FITKViewAdaptorFactory(...)` 注册，例如几何、网格、后处理和标记相关适配器。

这说明 FITKAdaptor 的核心价值不在于“实现某个格式”，而在于“提供统一注册与创建机制”。

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `project(FITKAdaptor VERSION 0.5.0 LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`、`AUTOUIC`、`AUTORCC`
- 通过 `link_fitk_library(${PROJECT_NAME} FITKCore)` 链接底层模块 `FITKCore`

### qmake

当前目录中同时保留了 `FITKAdaptor.pro` 和 `FITKAdaptor.pri`，说明该模块仍同时兼容 qmake 工程组织方式。

### 在仓库中的参与方式

- `FITKAdaptor` 作为 Kernel 层机制模块被上层接口模块和图形模块依赖。
- 它通常不会被独立使用，而是随着具体 IO / 视图实现一起接入上层业务。

## 使用方法

当前仓库中，FITKAdaptor 的典型使用方式主要有三类：

1. 继承 `FITKAbstractIOAdaptor` 或 `FITKAbstractViewAdaptor` 定义具体适配器实现。
2. 通过 `Register2FITKIOAdaptorFactory(...)` 或 `Register2FITKViewAdaptorFactory(...)` 注册实现。
3. 在业务侧通过 `FITKIOADAPTORFACTORY` 或 `FITKVIEWADAPTORFACTORY` 动态创建适配器，并设置输入数据后执行适配。

## 使用建议

- 新增文件格式支持时，优先新增具体 IO 适配器并注册到 `FITKIOAdaptorFactory`，不要在业务代码中堆叠格式分支。
- 新增显示桥接能力时，优先新增具体 View 适配器并注册到 `FITKViewAdaptorFactory`。
- 若工厂创建失败，优先检查注册宏、数据类名字符串和查询 key 是否一致，而不是先怀疑工厂实现本身。

## 注意事项

- FITKAdaptor 本身不包含具体格式逻辑，具体能力来自其他模块的注册实现。
- 工厂创建依赖字符串 key，文件类型名、图元类型名和类名必须与注册时完全一致。
- 当前模块说明应聚焦机制层职责，不宜把某个具体格式模块的实现细节误写成 FITKAdaptor 自身能力。

最后更新时间：2026-04-17