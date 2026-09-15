# GUIFrame

## 简介

GUIFrame 是 CADOCC 应用的主界面框架模块。

它负责把主窗口、三维渲染区、模型树、消息面板以及 QAction 触发链路组织成一个可运行的桌面界面骨架。

如果说 GUIWidget 更偏“界面基础件”，那么 GUIFrame 更偏“应用主框架”。

它不直接实现建模命令，也不负责数据对象定义。

它解决的是“应用主界面如何承载模型树、消息区和三维视窗，并把菜单动作转发给操作器仓库”的问题。

## 模块定位

- 所在目录：GUIFrame
- 构建产物：共享库 GUIFrame
- 直接服务对象：CADOCCApp 主程序、GUIWidget、OperatorsGUI、OperatorsModel、PickDataProvider
- 主要上游依赖：FITKAppFramework、FITKCore、FITKCompMessageWidget、FITKWidget、GUIWidget、FITKInterfaceGeometry、FITKPython、FITKInterfaceModel、Qt、OCC

从当前代码看，GUIFrame 是应用主界面层，不是组件层。

## 架构与目录

从 CMakeLists.txt 和头文件可见，模块主要包含：

```text
GUIFrame
├── MainWindow
├── RenderWidget
├── TreeWidget
├── MessageTabWidget
├── ActionEventHandler
├── GUIFramEnum
└── 构建与资源
    ├── MainWindow.ui
    ├── resources.qrc
    ├── 翻译文件
    └── CMakeLists.txt
```

其中最核心的类是 MainWindow。

其他类基本都围绕主窗口布局、消息处理和动作分发服务。

## 依赖关系

根据 GUIFrame/CMakeLists.txt，模块显式链接：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- OCC::OCC
- FITKAppFramework
- FITKCore
- FITKCompMessageWidget
- FITKWidget
- GUIWidget
- FITKInterfaceGeometry
- FITKPython
- FITKInterfaceModel

这说明 GUIFrame 同时依赖：

- 底座 GUI 抽象
- 消息组件
- 脚本能力
- 几何与模型接口

因此它既是界面骨架，也是多个功能模块的承载入口。

## 主要类与职责

### MainWindow

MainWindow 继承自 QMainWindow 和 Core::FITKAbstractGUIObject。

它是整个 GUIFrame 模块的核心类。

当前头文件中确认存在的关键接口包括：

- MainWindow(QWidget* parent = nullptr)
- ~MainWindow()
- updateGeometryTree()
- ActionEventHandler* getActionEventHandle() const
- RenderWidget* getRenderWidget() const
- TreeWidget* getTreeWidget()
- QAction* createAction(const QString& toolTip, const QString& objectName, const QString& iconPath = "", const QString& iconText = "")
- void setBarVisible(bool visible)
- void setSketchMode(bool isOn)
- QAction* getSketchAction()
- void devTest()

这些接口足以看出 MainWindow 负责：

- 创建和管理主界面动作。
- 暴露模型树与渲染区访问点。
- 在普通模式与草图模式之间切换。
- 为开发测试保留额外入口。

### ActionEventHandler

ActionEventHandler 是主界面按钮事件处理类。

当前对外最关键的槽函数是：

- void execOperator()

内部还存在：

- Core::FITKActionOperator* getOperator(QObject* object)

这说明 QAction 触发后，GUIFrame 会通过 ActionEventHandler 把界面对象转换成 FITKActionOperator，再交由操作器体系执行。

### RenderWidget

RenderWidget 是主界面中承载三维渲染区的 QWidget。

当前公开接口为：

- RenderWidget(QWidget* parent = nullptr)
- Core::FITKAbstractGraph3DWidget* getGraph3DWidget() const

这说明 GUIFrame 并不在这里硬编码具体 OCC 窗口类型，而是先以抽象图形窗口接口暴露能力。

### TreeWidget

TreeWidget 继承 QTreeWidget。

它是模型树显示与交互的承载窗口。

虽然当前未在本次 README 里展开其所有成员函数，但 MainWindow::getTreeWidget 已明确说明它是主窗口的重要组成部分。

### MessageTabWidget

MessageTabWidget 继承 QTabWidget。

从命名和依赖关系可知，它承担日志、消息或控制台相关内容的显示入口。

## 构建与集成

GUIFrame 通过 CMake 构建为共享库：

- project(GUIFrame VERSION 1.0.0 LANGUAGES CXX)
- add_library(${PROJECT_NAME} SHARED)
- 启用 Qt 自动生成工具
- 包含 MainWindow.ui、resources.qrc 与翻译文件
- 输出目录为 output/bin_d 与 output/bin

模块同时保留 qmake 与 Visual Studio 工程文件，因此 README 里应按“多构建系统共存”来描述，而不是只写单一路径。

## 模块层级关系

当前仓库中，GUIFrame 所处位置可以概括为：

1. CADOCCApp 通过 MainWindowGenerator 创建主窗口实例。
2. 主窗口内部承载 RenderWidget、TreeWidget、MessageTabWidget。
3. QAction 触发后交给 ActionEventHandler::execOperator。
4. OperatorsGUI 与 OperatorsModel 提供具体操作器实现。

换句话说，GUIFrame 负责“承载和分发”，不负责“定义业务动作”。

## 使用示例

### 场景 1：创建主窗口动作并绑定统一执行入口

```cpp
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/ActionEventHandler.h"

GUI::MainWindow mainWindow;

QAction* importAction = mainWindow.createAction(
    "导入几何",
    "actionImportCreate",
    ":/icons/import.png",
    "导入"
);

QObject::connect(
    importAction,
    &QAction::triggered,
    mainWindow.getActionEventHandle(),
    &GUI::ActionEventHandler::execOperator
);
```

这里使用的 createAction、getActionEventHandle 和 execOperator 都已在头文件中确认存在。

### 场景 2：访问渲染区和模型树

```cpp
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "GUIFrame/TreeWidget.h"

GUI::MainWindow mainWindow;

GUI::RenderWidget* renderWidget = mainWindow.getRenderWidget();
GUI::TreeWidget* treeWidget = mainWindow.getTreeWidget();

Core::FITKAbstractGraph3DWidget* graphWidget = nullptr;
if (renderWidget)
{
    graphWidget = renderWidget->getGraph3DWidget();
}
```

### 场景 3：切换界面条带与草图模式

```cpp
GUI::MainWindow mainWindow;

mainWindow.setBarVisible(true);
mainWindow.setSketchMode(true);

QAction* sketchAction = mainWindow.getSketchAction();
Q_UNUSED(sketchAction)
```

这些接口表明 GUIFrame 已将“界面模式切换”纳入主窗口职责范围。

## 注意事项

- ActionEventHandler 的工作前提是 QAction 的 objectName 或相关对象信息能够映射到正确的 FITKActionOperator。
- GUIFrame 既依赖消息窗口组件，也依赖图形窗口与脚本模块，初始化顺序要和上层应用装配保持一致。
- MainWindow 已经承担模式切换和动作创建职责，不宜再把大量业务判断塞回 GUIFrame，避免界面层膨胀。
- 当前仅根据源码接口和构建文件核对 README，实际运行行为仍应以应用启动结果为准。

最后更新时间：2026-04-23