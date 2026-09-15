# FITKAppFramework 应用程序框架

## 简介

FITKAppFramework 是 APPStructural 中的应用程序框架层，负责把 `FITKCore` 提供的数据对象、仓库、线程和操作器等基础能力装配成可运行的桌面应用。它围绕 `FITKApplication` 统一管理应用生命周期，并向上提供主窗口生成、全局数据、组件管理、插件管理、命令行、自动保存、Python 注册、工作台、程序驱动和许可证检查等扩展入口。

相对于 `FITKCore`，FITKAppFramework 解决的是“如何把底座能力组织成应用程序”的问题。它的实际接入样例：应用启动时注册主窗口生成器、全局数据工厂、组件工厂、设置、命令行处理器、工作台处理器、事件过滤器、Python 注册器和自动保存器，然后进入 `app.exec()`。

## 模块定位

FITKAppFramework 位于 `FITK_Kernel` 下，属于应用框架层模块，主要职责包括：

- 管理应用程序初始化、事件循环和退出流程。
- 统一注册主窗口、全局数据、组件工厂、命令行处理器和运行时扩展点。
- 提供全局数据总线、信号转接器和工作台处理入口。
- 管理插件、组件、程序驱动、自动保存、脚本接口和许可证检查等运行期能力。
- 作为上层应用的装配中心，而不是替代具体业务模块本身。

## 架构与目录

从当前目录中的头文件和源文件看，FITKAppFramework 可以概括为以下几组能力：

```text
FITKAppFramework
├── 应用核心
│   ├── FITKAppFramework / FITKApplication
│   ├── FITKAbstractMainWinGenerator
│   ├── FITKAbstractGlobalDataFactory
│   └── FITKAbstractAppInitializer
├── 全局状态与信号
│   ├── FITKGlobalData
│   ├── FITKSignalTransfer
│   ├── FITKAppSettings
│   ├── FITKRunTimeSetting
│   ├── FITKKeyMouseStates
│   └── FITKAppHistoryFiles
├── 组件与插件
│   ├── FITKComponents
│   ├── FITKComponentFactory
│   ├── FITKComponentInterface
│   ├── FITKPluginManager
│   └── FITKAbstractPlugin
├── 运行期扩展
│   ├── FITKCommandLineHandler
│   ├── FITKWorkBenchHandler
│   ├── FITKAbstractEventFilter
│   ├── FITKAbstractPythonRegister
│   ├── FITKAbstractAutoSaver
│   ├── FITKAbstractSysChecker
│   └── FITKAbstractLicenseChecker
└── 程序驱动与脚本
    ├── FITKProgramDriverFactory
    ├── FITKProgramTaskManager
    ├── FITKAbstractProgramDriver
    ├── FITKExecProgramDriver
    ├── FITKLinuxShellDriver
    ├── FITKAbstractCommandRunner
    └── FITKAppScriptHelper
```

## 依赖关系

### 编译依赖

根据 `FITK_Kernel/FITKAppFramework/CMakeLists.txt`，该模块当前显式依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- Qt5::Network
- FITKCore
- Windows 平台额外链接 `DbgHelp`

### 模块层级关系

- FITKAppFramework 直接依赖 `FITKCore`，并通过 `link_fitk_library(${PROJECT_NAME} FITKCore)` 进行链接。
- 上层应用例如 `StructuralApp` 通过该模块统一装配主窗口、全局数据、组件和运行时扩展。
- 组件、接口和业务模块通常不直接承担应用生命周期管理，而是通过本框架接入应用程序。

## 主要类与功能说明

### 应用核心

#### `FITKApplication`

应用程序核心类，继承自 `QApplication`。它负责应用初始化、主窗口生成、全局数据建立、组件注册、事件过滤器接入、命令行处理、自动保存和许可证检查等统一入口，是整个模块最核心的类。

#### `FITKAbstractMainwindowGenerator`

主窗口生成器接口。应用层通过派生类决定主窗口如何创建，并交给 `FITKApplication` 在初始化过程中统一装配。

#### `FITKAbstractGlobalDataFactory`

全局数据工厂接口。应用侧通过它决定几何、物理、后处理和其他全局数据对象分别由哪些具体类型实现。

### 全局状态与运行时信息

#### `FITKGlobalData`

全局数据总线，负责保存主窗口指针、几何/网格/物理/后处理等主数据对象，以及运行时设置、键鼠状态和历史文件管理器。它是上层应用共享运行时状态的主要入口。

#### `FITKAppSettings` / `FITKRunTimeSetting`

前者负责持久化设置，后者负责运行时状态。

#### `FITKSignalTransfer`

应用级信号转接器，用于模块间解耦通信。它承担消息输出、进度传递、程序驱动消息和文件变更通知等统一信号出口。

### 组件与插件机制

#### `FITKCmponents`

组件管理器，负责保存和查询应用中的功能组件。上层业务通常通过名称或类型从这里获取组件实例。

#### `FITKComponentFactory` / `FITKComponentInterface`

分别负责组件创建与组件接口约束，是应用功能模块集中装配的基础。

#### `FITKPluginsManager` / `FITKAbstractPlugin`

分别负责插件动态库管理与插件接口定义。插件能力位于扩展层，框架负责其加载、卸载和生命周期接入。

### 运行期扩展点

#### `FITKCommandLineHandler`

命令行处理扩展入口。

#### `FITKWorkBenchHandler`

工作台运行模式处理入口，用于框架与具体工作台流程协同。

#### `FITKAbstractEventFilter`

自定义事件过滤器基类，用于将全局键盘、鼠标或其他应用级事件处理接入框架。

#### `FITKAbstractPythonRegister`

Python 注册接口，用于向脚本层暴露应用能力。

#### `FITKAbstractAutoSaver`

自动保存接口，用于应用侧定义自动保存周期和具体保存行为。

#### `FITKAbstractSysChecker` / `FITKAbstractLicenseChecker`

分别负责系统检查和许可证校验，属于应用启动期的扩展点。

### 程序驱动与脚本

#### `FITKProgramTaskManeger` / `FITKProgramDriverFactory`

负责管理外部程序执行任务和程序驱动创建流程。

#### `FITKAbstractProgramDriver` / `FITKExecProgramDriver` / `FITKLinuxShellDriver`

定义程序驱动抽象与具体执行方式，用于集成外部可执行程序或 shell 命令。

#### `FITKAppScriptHelper`

脚本辅助类，用于脚本文件执行和框架级脚本交互。

## 在应用程序中的实际接入

### 1. 主程序统一注册扩展点

```cpp
int main(int argc, char *argv[])
{
    // 初始化应用框架
    AppFrame::FITKApplication app(argc, argv);
    //系统检查
    app.checkSystem(new SystemChecker);

    // 注册程序的主要组件和设置
    // 注册主窗口生成器
    app.regMainWindowGenerator(new MyWindowGenerator);
    // 注册全局数据工厂
    app.regGlobalDataFactory(new MyGlobalDataFactory);
    // 注册组件工厂
    app.regComponentsFactory(new MyComponentFactory);
    // 注册命令行处理
    app.regCommandLineHandler(new MyCommandLineHandler);
    //注册自定义事件处理
    app.addEventFilter(new MyMouseKeyEventFilter);

    // 运行程序，包含初始化到释放内存的全部操作
    return app.exec();
}
```

这说明 FITKAppFramework 在本项目中的真实价值是“提供统一装配点”，而不是替代业务模块自身的实现。

### 2. 用全局数据工厂决定应用数据骨架

`GlobalDataFactory.cpp` 中，全局数据工厂把几何、物理和后处理数据统一绑定到当前应用实际使用的数据对象上：

```cpp
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossPostData.h"

class GlobalDataFactory : public AppFrame::FITKAbstractGlobalDataFactory
{
private:
    virtual Core::FITKAbstractDataObject *createMeshData() override
    {
        return nullptr;
    }
    virtual Core::FITKAbstractDataObject *createGeoData() override
    {
        // 创建几何数据
        return new Interface::FITKGeoCommandList;
    }
    virtual Core::FITKAbstractDataObject *createPhysicsData() override
    {
        return  new Radioss::FITKRadiossCase;
    }
    virtual Core::FITKAbstractDataObject *createPostData() override
    {
        // 创建后处理数据
        return  new Radioss::FITKRadiossPostData;
    }
    virtual QHash<int, Core::FITKAbstractDataObject *> createOtherData() override
    {
        // 不创建其他数据
        return QHash<int, Core::FITKAbstractDataObject *>();
    }
};
```

在软件中访问这些全局数据：

```cpp
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

Radioss::FITKRadiossCase* caseData = FITKAPP->getGlobalData()
                 ->getPhysicsData<Radioss::FITKRadiossCase>();
```

这说明框架不直接决定业务数据类型，而是把选择权交给应用工厂实现。

### 3. 用命令行处理器控制运行时行为

`CommandLineHandler.cpp` 中继承 `FITKCommandLineHandler` 并重写命令行解析逻辑，在框架约定的入口中修改设置和运行时状态：

```cpp
class MyCommandLineHandler : public AppFrame::FITKCommandLineHandler
{
public:
    MyCommandLineHandler() = default;
    ~MyCommandLineHandler() = default;

    // 重写该函数处理命令行参数
    void handle(const QStringList &args) override
    {
        // 解析args，根据参数修改运行时设置
        // 例如: FITKAPP->getGlobalData()->getRunTimeSetting()->setValue("key", value);
    }
};
```

同一处理器还会通过 `FITKOPERREPO->getOperatorT<Core::FITKActionOperator>(...)` 获取操作器，说明框架层与 `FITKCore` 的操作器体系是协同工作的。

## 其他现有示例

### 1. `FITKApplication` 使用示例

```cpp
// include header files

int main(int argc, char *argv[])
{
    // 初始化应用框架
    AppFrame::FITKApplication app(argc, argv);
     // 设置应用程序名称
    app.setApplicationName("FastCAE-OpenRadioss");
    // 注册程序的主要组件和设置
    app.regMainWindowGenerator(new MainWindowGenerator);
    // 注册主窗口生成器
    app.regGlobalDataFactory(new GlobalDataFactory);
    // 注册全局数据工厂
    app.regComponentsFactory(new ComponentFactory);
    // 注册组件工厂
    app.regAppSettings(new RadiossAPPSettings);
    // 注册应用设置
    app.regCommandLineHandler(new CommandLineHandler);
    // 注册命令行处理
    app.addGolbalSignalProcesser(new SignalProcessor);
    // 注册信号处理器
    app.regAppInitalizer(new AppInitializer);
    // 设置插件识别密钥
    app.setPluginKey("RadiossAppPlugin");
    // 运行应用程序的消息循环
    return app.exec();
}
```

### 2. 主窗口生成器示例

```cpp
class MyWindowGenerator : public AppFrame::FITKAbstractMainwindowGenerator
{
public:
    MyWindowGenerator () = default;
    ~MyWindowGenerator () = default;
     //创建主界面窗体
    QWidget *genMainWindow() override
    {
         //创建一个Qt的窗体，例如QMainWindow
         // 返回窗体指针 return new QMainWindow;
    }
    //指定qss样式文件
    QString getQssStyle() override
    {
        //返回qss样式文件路径，通常是在qrc资源文件
        //不需要加载样式文件可以返回空对象
    }
    //指定需要加载的语言包 qm文件
    QStringList getTranslatorQmFiles() override
    {
        //返回语言包文件，通常是在qrc资源文件
        //若不需要可以返回空对象
    }
};
```

### 3. 全局数据工厂示例

```cpp
class MyGlobalDataFactory :  public FITKAbstractGlobalDataFactory
{
public:
    MyGlobalDataFactory () = default;
    ~MyGlobalDataFactory () = default;

private:
    //创建全局网格，可以为空
    virtual Core::FITKAbstractDataObject* createMeshData() override
    {
        // return new MyMeshData;
    }
     //创建全局几何管理，可以为空
    virtual Core::FITKAbstractDataObject* createGeoData() override
    {
        // return  new MyGeoData;
    }
    //创建全局物理模型管理，可以为空
    virtual Core::FITKAbstractDataObject* createPhysicsData() override
    {
          // return  new MyPhysicsData;
    }
    //创建全局后处理数据，可以为空
    virtual Core::FITKAbstractDataObject* createPostData() override
    {
          // return  new MyPostData;
    }
    //自定义数据拓展
    //创建其他数据，返回值哈希中的int至少为FITKGolbalData.h/FITKGlobalDT::GDTOther
    virtual QHash< int,Core::FITKAbstractDataObject*> createOtherData() override
    {
          //QHash< int,Core::FITKAbstractDataObject*> dataHash;
          //dataHash[1001] = new MyData;
          // return dataHash;
    }

};
```

### 4. 组件工厂示例

```cpp
class MyComponentFactory : public AppFrame::FITKComponentFactory
{
public:
    MyComponentFactory () = default;
    ~MyComponentFactory () = default;
    QList<AppFrame::FITKComponentInterface *> createComponents() override
    {
         QList<AppFrame::FITKComponentInterface *> comps;
         // comps << new MyComp1;
         // comps << new MyComp2;
        return comps;
    }
};
```

### 5. 事件过滤器示例

```cpp
class  MyMouseKeyEventFilter : public AppFrame::FITKAbstractEventFilter
{
public:
    explicit MyMouseKeyEventFilter() = default;
    virtual ~MyMouseKeyEventFilter() = default;

    // 事件过滤器，Object是事件的触发对象，event 是发出的事件
    void eventFilter(QObject *Object, QEvent *event) override
    {
          //对Object 和 event进行判断，只处理需要处理的对象和事件
          // do something
    }
};

// 注册事件过滤器
app.addEventFilter(new MyMouseKeyEventFilter);
```

### 6. 自动保存示例

```cpp
class AutoSaver : public AppFrame::FITKAbstractAutoSaver
{
public:
   //默认计时器的触发时间间隔，12000毫秒
    explicit AutoSaver(const int &msc = 120000 );
    virtual ~AutoSaver() = default;
    //执行自动保存操作
    void autoSave() override
    {
         //指定文件操作，并通过多线程操作写出文件
    }

    void finalize() override
    {
        //程序运行结束，删除临时文件等操作
    }
};

// 注册自动保存器
AppFrame::FITKApplication app(argc, argv);
app.regAutoSaver(new AutoSaver);
```

### 7. 许可证检查示例

```cpp
class MyFITKLicenseChecker :
        public AppFrame::FITKAbstractLicenseChecker
{
public:
    explicit MyFITKLicenseChecker () = default;
    virtual ~MyFITKLicenseChecker ()= default;

    /**
     * @brief 检查许可证有效性
     * @param error 错误信息输出参数
     * @return bool 许可证是否有效
     */
    virtual bool checkLicense(QString &error) override
    {
         //常用的License检查方法包含
         //1. 检查特定路径下的License文件是否有效
         //2. 联网登录账号密码获取授权信息并进行校验
         //3. 获取硬件输入，如U盾、指纹等

         //如果程序分模块授权，需要将记录各个模块的授权状态
         //接口是addFeatureLicense(const QString& feature, bool enabled = true)

         //若检查授权失败或者未授权，返回false，并在error变量中加入错误信息
         //返回false，程序将直接退出

         //授权正确则返回true，程序继续执行
        return false;
    }
};

// 注册许可证检查器
AppFrame::FITKApplication app(argc, argv);
app.regLicenseChecker(new MyFITKLicenseChecker );
```

### 8. 可执行程序驱动示例

```cpp
class MyExecProgramDriver : public AppFrame::FITKExecProgramDriver
{
    Q_OBJECT
public:
    MyExecProgramDriver () = default;
     ~MyExecProgramDriver () = default;
    int getProgramType() override
    {
        return 2;
    }

    QString getProgramName() override
    {
        return "MyExecProgramDriver";
    }
};

// 注册到程序驱动工厂
Register2FITKProgramDriverFactory(2, MyExecProgramDriver, MyExecProgramDriver);

// 使用程序驱动
AppFrame::FITKProgramInputInfo* inputInfo = new MyProgramInputInfo;
AppFrame::FITKProgramTaskManeger* programTaskMgr = FITKAPP->getProgramTaskManager();
auto* myExe = dynamic_cast<MyExecProgramDriver*>(
    programTaskMgr->createProgram(2, "MyExecProgramDriver", inputInfo));
myExe->setExecProgram("D:/myprogram.exe");
myExe->setWorkingDirectory("D:/");
//开始驱动
myExe->start();
```

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `project(FITKAppFramework VERSION 1.0.7 LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`、`AUTOUIC`、`AUTORCC`
- 资源文件包括 `License.qrc`
- UI 文件包括 `FITKCopyrightDialog.ui`
- 通过 `link_fitk_library(${PROJECT_NAME} FITKCore)` 链接底层模块 `FITKCore`

### 在仓库中的集成方式

- `FITKAppFramework` 是 `FITK_Kernel` 的核心子模块之一。
- 应用程序通过包含 `FITK_Kernel/FITKAppFramework/FITKAppFramework.h` 并在 `main.cpp` 中创建 `FITKApplication` 来接入框架。

## 使用建议

- 新应用优先通过注册器、工厂和处理器扩展 FITKAppFramework，不要直接改框架内部初始化流程。
- 与应用生命周期相关的逻辑，例如自动保存、命令行、系统检查、许可证校验和 Python 注册，建议都接入框架已有扩展点。
- 若业务逻辑需要跨模块通知，优先通过 `FITKSignalTransfer` 和框架级处理器解耦，而不是在模块间直接耦合。

## 注意事项

- 当前模块是应用框架层，不负责定义具体业务数据或业务组件内容。
- 组件、插件和事件过滤器的所有权与生命周期应交给框架管理，不要在业务侧重复释放。

最后更新时间：2026-04-17
