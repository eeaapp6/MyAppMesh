# FITKCore - FITK核心框架库

## 简介

FITKCore 是 FastCAE 集成工具包（FITK）的核心底座模块，负责提供整个工程通用的对象模型、数据仓库、操作器仓库、图形抽象、线程任务和基础工具能力。它本身不是一个独立应用程序，而是 `FITKAppFramework`、`FITKAdaptor`、`FITKEasyParam` 以及各类 `FITK_Interface`、`FITK_Component` 模块的公共基础层。

FITKCore 解决的是“上层模块如何复用同一套基础对象与运行机制”的问题。应用程序并不直接围绕某个具体业务类构建，而是先依赖 FITKCore 提供的抽象基类、仓库机制和任务调度能力，再在上层完成业务装配。

## 模块定位

FITKCore 位于 `FITK_Kernel` 下，属于底座子模块，主要承担以下职责：

- 定义统一的基础对象体系，例如 `FITKAbstractObject`、`FITKAbstractDataObject`、`FITKAbstractOperator`。
- 提供全局数据仓库与操作器仓库，支持对象集中管理和按 key 获取操作器实例。
- 提供图形对象、图形窗口和 GUI 抽象层，供上层图形与界面模块继承实现。
- 提供线程池、线程任务和任务组，用于统一调度后台任务。
- 提供几何算法、向量、文件目录、系统信息等基础工具类。

## 架构与目录

从当前目录中的头文件和源文件看，FITKCore 可以概括为以下几组能力：

```text
FITKCore
├── 基础对象
│   ├── FITKAbstractObject
│   ├── FITKAbstractDataObject / FITKAbstractNDataObject
│   ├── FITKAbstractOperator / FITKActionOperator
│   └── FITKAbstractFactory
├── 仓库与管理
│   ├── FITKDataRepo
│   ├── FITKOperatorRepo
│   ├── FITKGUIRepo
│   ├── FITKScriptTextRepo
│   └── FITKAbstractDataManager
├── 图形与界面抽象
│   ├── FITKAbstractGUIObject / FITKAbstractGUIDialog / FITKAbstractGUI
│   ├── FITKAbstractGraphObject
│   └── FITKAbstractGraphWidget
├── 线程任务
│   ├── FITKThreadPool
│   ├── FITKThreadTask
│   └── FITKThreadTaskGroup
└── 工具与算法
    ├── FITKVec3D / FITKVec3DAlg
    ├── FITKCircleAlg / FITKTriangleAlg
    ├── FITKDirFileTools
    ├── FITKSystemInfo
    ├── FITKContainerAlg
    └── FITKNumberComplex
```

## 依赖关系

### 编译依赖

根据 `FITK_Kernel/FITKCore/CMakeLists.txt`，该模块当前显式依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets


### 模块层级关系

- FITKCore 本身不依赖更下层的内部模块，是 FITK 体系的基础层。
- `FITKAppFramework` 在其构建说明中明确依赖 FITKCore，并负责把这些基础能力装配成应用程序框架。
- `FITKAdaptor`、`FITKEasyParam` 以及接口层、组件层模块也都建立在 FITKCore 的对象模型与仓库机制之上。

## 主要类与功能说明

### 基础对象与数据对象

#### `FITKAbstractObject`

FITKCore 中大多数基础类型的共同父类，用于提供统一的对象抽象基础。

#### `FITKAbstractDataObject`

所有业务数据对象的抽象基类。它负责数据对象 ID、父子层级关系、用户数据挂载、序列化/反序列化接口以及数据关联检查。上层业务数据通常都会直接或间接继承它。

#### `FITKAbstractNDataObject`

带名称语义的数据对象基类，适合在需要“名称 + 数据对象”语义的场景下复用。

#### `FITKAbstractDataManager`

数据对象管理模板基类，用于封装对象列表管理逻辑；`FITKDataRepo` 的内部管理器就建立在它之上。

### 仓库与工厂机制

#### `FITKDataRepo`

全局数据仓库，负责集中持有和查询 `FITKAbstractDataObject` 派生对象。当前接口中支持按 ID 获取对象、按类型批量获取对象、按名称查找类型化数据以及重置仓库。

#### `FITKOperatorRepo`

操作器仓库，负责按字符串 key 注册和创建 `FITKAbstractOperator` 派生类。上层模块通常通过注册宏把操作器接入仓库，再在运行时通过 `getOperatorT<T>()` 获取实例。

#### `FITKAbstractFactory`

通用工厂抽象，提供按 `QString` 或 `int` key 创建对象的统一接口。它是 FITK 中多种“按 key 创建对象”机制的基础抽象。

### 操作器体系

#### `FITKAbstractOperator`

操作器抽象基类，负责保存触发对象和参数列表，并为上层操作流程提供统一的参数传递接口。

#### `FITKActionOperator`

面向界面动作的操作器扩展类。应用层常用它来承接按钮、菜单或命令触发后的业务执行流程。

### 图形与界面抽象

#### `FITKAbstractGraphObject`

显示对象抽象基类，负责关联数据对象与图形窗口，并提供更新、移除等基础行为。它是图形适配和图元显示相关模块的重要扩展点。

#### `FITKAbstractGraphWidget`

图形窗口抽象基类，供具体图形窗口或渲染组件实现。

#### `FITKAbstractGUIObject` / `FITKAbstractGUIDialog` / `FITKAbstractGUI`

界面对象、对话框和 GUI 能力的基础抽象，方便上层界面模块以统一方式扩展。

### 线程与任务

#### `FITKThreadPool`

线程池单例，负责执行和等待后台任务，并可与应用框架的信号转接器协同工作。

#### `FITKThreadTask`

线程任务抽象基类，用于封装单个后台任务的执行逻辑。

#### `FITKThreadTaskGroup`

任务组管理类，用于组织一组任务的协同执行。

### 基础工具与算法

- `FITKVec3D` / `FITKVec3DAlg`：三维向量与相关算法。
- `FITKCircleAlg` / `FITKTriangleAlg`：基础几何算法。
- `FITKDirFileTools`：目录与文件辅助工具。
- `FITKSystemInfo`：系统信息获取工具。
- `FITKContainerAlg`：容器操作辅助算法。
- `FITKNumberComplex`：复数数值工具。

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `project(FITKCore VERSION 1.0.4 LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`、`AUTOUIC`、`AUTORCC`
- 输出目录为 `output/bin_d` 和 `output/bin`
- 编译定义包含 `FITKCore_API` 与 `FITKUseAppFramework`

### qmake

当前目录中同时保留了 `FITKCore.pro`、`FITKCore.pri` 和 Visual Studio 工程文件，说明该模块需要同时兼容现有的多套工程组织方式。在更新说明时，应以当前仓库仍在使用的构建文件为准，而不是假定其只服务某一种构建系统。

### 在上层工程中的参与方式

FITKCore 作为基础层，一般不会被独立启动，而是由上层应用或模块进行链接和调用。最典型的接入方式，是由 `FITKAppFramework` 把这些基础能力装配到应用程序生命周期中。


## 使用示例

以下示例保留当前 README 中已有内容，用于展示 FITKCore 的典型使用方式。

### 1. 数据对象创建与使用

```cpp
// 第一步：定义数据类，继承自 Core::FITKAbstractDataObject，必须有无参默认构造函数
class TstDataClass : public Core::FITKAbstractDataObject
{
public :
    TstDataClass() = default;
    ~TstDataClass() = default;
    //添加其他默认成员函数与成员变量
};

// 第二步：注册到工厂
Register2FITKDataFactory(TST, TstDataClass);

// 第三步：通过工厂创建对象
auto td = Core::FITKDataFactory::getInstance()->create("TST");
auto tstd = dynamic_cast<TstDataClass*>(td);
if(tstd)
{
    // do something with tstd
}
```

### 2. 操作器仓库使用

```cpp
// 第一步：定义操作器，继承 FITKAbstractOperator，必须有无参构造函数
class MyOperator : public FITKAbstractOperator
{
    public:
        explicit MyOperator() = default;
        ~MyOperator() = default;
        void execOperator() override
        {
            // 添加操作逻辑
        }
};

// 第二步：将操作器注册到操作器仓库
Register2FITKOPeratorRepo(MyString, MyOperator );

// 第三步：通过操作器仓库获取并执行操作器
//获取抽象类型
FITKAbstractOperator* oper1 = FITKOPERREPO->getOperator("MyString");
 //获取具体类型
MyOperator* oper2 = FITKOPERREPO->getOperatorT<MyOperator>("MyString");
```

### 3. 线程池使用

```cpp
// 第一步：定义线程任务，继承 Core::FITKThreadTask
class MyThreadTask : public Core::FITKThreadTask
{
public:
    MyThreadTask()= default;
    virtual ~MyThreadTask() = default;
    void run() override
    {
        //需要在线程中执行的任务，通常是耗时的操作
    }
};

// 第二步：提交任务到线程池执行
//创建线程任务
MyThreadTask* task1 = new MyThreadTask();
//获取线程池
auto tp = Core::FITKThreadPool::getInstance();
//任务提交到线程池执行，将会调用run函数
tp->execTask(task1);
```

### 4. 任务组使用

```cpp
//创建三个任务
MyThreadTask* task1 = new MyThreadTask();
MyThreadTask* task2 = new MyThreadTask();
MyThreadTask* task3 = new MyThreadTask();
//创建线程任务组
Core::FITKThreadTaskGroup g;
g.addTask(task1);
g.addTask(task2);
g.addTask(task3);
//获取线程池
auto tp = Core::FITKThreadPool::getInstance();
//提交任务组，任务将会并行执行
tp->execTaskGroup(&g);
//等待所有任务执行完成（阻塞当前线程）
g.wait();
```

### 5. 枚举转换工具使用

```cpp
#include <QObject>
class EnumTransferTest : public QObject
{
    Q_OBJECT
public:
    enum MyEnum {M1, M2, M3};
    Q_ENUM(MyEnum);
};

Core::FITKEnumTransfer<MyEnum> enumObj;
//字符转枚举
bool ok = false;
MyEnum va = enumObj.toEnum("M1", ok);  // va的值是M1，ok为true表示获取成功
//枚举转字符
QString sM1 = enumObj.toStrting(MyEnum::M1, ok);  // sM1的值是M1，ok为true表示获取成功
```


## 注意事项

- FITKCore 是基础层模块，README 应重点说明其“支撑关系”和“扩展点”，而不是把它写成独立软件说明。
- 当前仓库同时维护 CMake、qmake 和部分 Visual Studio 工程文件，更新文档时应优先以实际构建文件为准。
- 若继续扩展 README，建议优先补充具体类之间的协作关系，而不是继续机械罗列全部头文件。

最后更新时间：2026-04-17
