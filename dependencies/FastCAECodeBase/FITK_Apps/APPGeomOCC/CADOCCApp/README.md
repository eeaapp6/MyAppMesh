# CADOCCApp

## 简介

CADOCCApp 是当前仓库的应用主程序模块。

它负责把 FITK_Kernel、FITK_Interface、FITK_Component 以及本仓库的 GUI、操作器、Python 包装等业务模块装配成可运行的 CADOCC 桌面程序。

与其他共享库模块不同，CADOCCApp 构建产物是可执行程序。

它解决的是“如何初始化应用框架、创建主窗口、注册组件、创建全局数据、接入 Python、处理命令行和工作台输入输出”的问题。

## 模块定位

- 所在目录：CADOCCApp
- 构建产物：可执行程序 CADOCCApp
- 主要上游依赖：FITKAppFramework、FITKCore、FITKCompMessageWidget、FITKInterfaceModel、FITKInterfaceGeometry、FITKGeoCompOCC、FITKRenderWindowOCC、OperatorsGUI、OperatorsModel、FITKWidget、GUIWidget、GUIFrame、FITKInterfaceGeoIOHDF5、FITKGeoOCCIOHDF5、FITKPython、PythonInterface、Qt、OCC、HDF5

它是所有非 FITK 业务模块的装配中心。

## 架构与目录

从当前目录文件看，应用入口主要由以下对象构成：

```text
CADOCCApp
├── main.cpp
├── ApplicationCAD
├── MainWindowGenerator
├── GlobalDataFactory
├── ComponentFactory
├── CommandLineHandler
├── WorkBenchHandler
├── PythonRegister
├── SignalProcessor
├── CADOCCSettings
└── 资源与构建文件
```

这组类可以对应到应用装配链路：

- ApplicationCAD：应用对象本体
- MainWindowGenerator：主窗口创建与翻译文件提供
- GlobalDataFactory：全局数据骨架创建
- ComponentFactory：组件注册
- CommandLineHandler：命令行处理
- WorkBenchHandler：批处理/工作台输入输出
- PythonRegister：脚本包装注册
- SignalProcessor：全局信号处理
- CADOCCSettings：运行配置读取

## 依赖关系

根据 CADOCCApp/CMakeLists.txt，当前显式链接：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- OCC::OCC
- HDF5::HDF5
- FITKAppFramework
- FITKCore
- FITKCompMessageWidget
- FITKInterfaceModel
- FITKInterfaceGeometry
- FITKGeoCompOCC
- FITKRenderWindowOCC
- OperatorsGUI
- OperatorsModel
- FITKWidget
- GUIWidget
- GUIFrame
- FITKInterfaceGeoIOHDF5
- FITKGeoOCCIOHDF5
- FITKPython
- PythonInterface

这意味着 CADOCCApp 是整个应用的最终聚合层。

## 主要类与职责

### main.cpp

main.cpp 展示了应用的真实启动链路。

当前代码中可以确认以下关键动作：

- 设置高 DPI 属性
- 创建 ApplicationCAD app(argc, argv)
- Linux 下设置 PYTHONHOME 与 PYTHONPATH
- 注册 MainWindowGenerator
- 注册 GlobalDataFactory
- 注册 ComponentFactory
- 注册 CommandLineHandler
- 注册 SignalProcessor
- 注册 OCCAppWorkBenchHandler
- 注册 CADOCCSettings
- 注册 PyRegister
- 设置运行时 DEV 标志
- 调用 app.exec()

这份启动逻辑是理解整个应用装配关系的第一事实来源。

### ApplicationCAD

ApplicationCAD 继承自 AppFrame::FITKApplication。

当前接口包括：

- ApplicationCAD(int& argc, char** argv)
- bool init() override

它是对 FITKApplication 的应用级扩展。

### MainWindowGenerator

MainWindowGenerator 继承自 AppFrame::FITKAbstractMainwindowGenerator。

当前接口包括：

- QWidget* genMainWindow() override
- QStringList getTranslatorQmFiles() override
- getEnTranslatorFiles() const
- getCnTranslatorFiles() const

它负责创建主窗口并提供翻译文件列表。

### GlobalDataFactory

GlobalDataFactory 继承自 AppFrame::FITKAbstractGlobalDataFactory。

当前实现里可以确认：

- createGeoData() 返回 new Interface::FITKGeoCommandList
- createMeshData() 返回 nullptr
- createPhysicsData() 返回 nullptr
- createPostData() 返回 nullptr
- createOtherData() 返回空 QHash

这说明当前应用的全局主数据重点在几何命令列表，而不是网格、物理或后处理数据。

### ComponentFactory

ComponentFactory 继承自 AppFrame::FITKComponentFactory。

当前 createComponents() 返回的组件包括：

- Comp::ConsoleComponent
- OperModel::OpersModelInterface
- GUIOper::OperatorsGUIInterface
- IO::FITKOCCIOHDF5Interface
- Render::FITKGraph3DWindowOCCInterface
- OCC::FITKGeoCompOCCInterface

这份列表是当前应用功能组成的关键事实来源。

### OCCAppWorkBenchHandler

OCCAppWorkBenchHandler 继承自 AppFrame::FITKWorkBenchHandler。

当前头文件和实现可确认的接口与行为包括：

- execHandler()：遍历输入文件
- execOutput()：输出工程与 brep
- importGeoFile(const QString& fileName)
- openProjectFile(const QString& fileName)
- saveProjectFile()
- exportBrep()

其内部真实调用了以下操作器键：

- OperImportManager
- actionOpenProject
- actionSaveProject
- actionExport

这是工作台模式和操作器体系之间的真实桥接点。

### PyRegister

PyRegister 继承自 AppFrame::FITKAbstractPythonRegister。

当前 registWapper() 中注册了：

- Point / PointWapper
- Solid / SolidWapper
- Box / BoxWrapper
- Chamfer / ChamferWrapper
- AppendCompound / AppendCompoundWrapper
- RemoveCompound / RemoveCompoundWrapper

并在最后调用 _pyInterface->importPython()。

## 构建与集成

CADOCCApp 通过 CMake 构建为可执行程序：

- project(CADOCCApp VERSION 1.0.0 LANGUAGES CXX)
- add_executable(${PROJECT_NAME} WIN32)
- 资源文件包含 resources.qrc 与 translations.qrc
- 输出目录为 output/bin_d 与 output/bin

由于它是最终可执行程序，因此 README 中的构建说明应聚焦“上层装配”，而不是把它写成普通共享库。

## 使用示例

### 场景 1：主程序启动链路

下面的片段直接对应当前 main.cpp 的真实装配模式：

```cpp
ApplicationCAD app(argc, argv);

app.regMainWindowGenerator(new MainWindowGenerator);
app.regGlobalDataFactory(new GlobalDataFactory);
app.regComponentsFactory(new ComponentFactory);
app.regCommandLineHandler(new CommandLineHandler);
app.addGolbalSignalProcesser(new SignalProcessor);
app.regWorkBenchHandler(new OCCAppWorkBenchHandler);
app.regAppSettings(new CADOCCSettings);
app.regPythonRegister(new PyRegister);

return app.exec();
```

### 场景 3：工作台模式导入几何并保存工程

以下行为在当前 WorkBenchHandler.cpp 中真实存在：

```cpp
Core::FITKActionOperator* importOper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("OperImportManager");
Core::FITKActionOperator* openOper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionOpenProject");
Core::FITKActionOperator* saveOper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionSaveProject");
Core::FITKActionOperator* exportOper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionExport");
```

## 注意事项

- 当前应用全局数据工厂只创建几何主数据，README 不应夸大为完整 CAE 五段数据都已接入。
- PyRegister 中方法名沿用现有代码的 registWapper 拼写，而不是擅自改成 registerWrapper。
- 工作台模式依赖操作器键字符串，若将来变更注册名，README 也需要同步修正。
- Linux 下 Python 环境变量由 main.cpp 直接设置，部署时需要保证相对路径结构成立。

最后更新时间：2026-04-23