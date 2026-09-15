# GUIWidget

## 简介

GUIWidget 是 CADOCC 应用中面向通用界面部件的基础模块。

它位于应用层公共 GUI 库位置，负责为主窗口菜单、参数面板子部件、空属性页、通用对话框基类以及少量输入控件提供统一封装。

相对于 FITK_Kernel 中的 GUI 抽象，GUIWidget 的职责更靠近本应用自身。

它不负责应用生命周期，也不承担具体建模逻辑。

它解决的是“本应用自己的窗口、菜单和参数面板如何在统一约束下复用”的问题。

从当前代码可以看到，模块主要向上提供以下能力：

- 面向主窗口的菜单描述类与菜单构建入口。
- 面向业务窗口的 QWidget 派生基类。
- 面向参数对话框的 GUIDialogBase。
- 面向属性区的 PropertyChildWidgetBase 与空页面实现。
- 面向简化输入的 CompLineEdit。

## 模块定位

- 所在目录：GUIWidget
- 构建产物：共享库 GUIWidget
- 主要使用者：GUIFrame、GUIDialog、OperatorsModel、OperatorsGUI 以及应用主程序
- 上游依赖：FITKAppFramework、FITKCore、FITKInterfaceGeometry、FITKInterfaceModel、FITKGeoCompOCC、FITKRenderWindowOCC、FITKOCCGraphAdaptor、Qt、OpenCASCADE

GUIWidget 属于“应用 GUI 基础层”。

它的边界很明确：

- 不创建主程序。
- 不注册组件。
- 不直接执行业务操作器。
- 通过基类和通用部件把上层 GUI 代码的重复逻辑收敛起来。

## 架构与目录

从 CMakeLists.txt 与头文件可见，模块结构可概括为：

```text
GUIWidget
├── 菜单基础
│   ├── MainMenuBase
│   └── MenuActionItem
├── 界面与对话框基础
│   ├── GUIWidgetBase
│   └── GUIDialogBase
├── 属性面板基础
│   ├── PropertyChildWidgetBase
│   └── PropertyEmptyWidget
├── 输入控件
│   └── CompLineEdit
└── 构建与导出
    ├── GUIWidgetAPI.h
    ├── CMakeLists.txt
    ├── GUIWidget.pro / .pri
    └── 翻译文件
```

这个目录结构说明它是一个“面向界面复用”的库，而不是功能组件。

## 依赖关系

根据 GUIWidget/CMakeLists.txt，当前模块显式链接：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- OCC::OCC
- FITKAppFramework
- FITKCore
- FITKInterfaceGeometry
- FITKInterfaceModel
- FITKGeoCompOCC
- FITKRenderWindowOCC
- FITKOCCGraphAdaptor

其中值得注意的事实有两点：

第一，GUIWidget 并不只是纯 Qt 小控件库。

第二，GUIDialogBase 已经直接包含几何命令列表、基准元素列表以及图形相关操作器接口，因此该模块和应用的几何建模链路有直接耦合。

## 主要类与职责

### MenuActionItem

MenuActionItem 用于描述菜单项元数据。

当前可确认的构造形式包括：

- MenuActionItem(QString title, QString action, QIcon icon, bool checkable = false, QString checkGroup = "")
- MenuActionItem(QString title, QString action, bool checkable = false, QString checkGroup = "")
- MenuActionItem(QString title, QString action, QList<MenuActionItem*> childs)

它提供的读取接口包括：

- getSeparator()
- getCheckable()
- getCheckGroup()
- getTitle()
- getIcon()
- getOperAction()
- getChilds()
- getKey()

README 中涉及菜单示例时，应基于这些真实接口，而不是假定存在额外的 setter。

### MainMenuBase

MainMenuBase 是主窗口菜单构造基类。

当前公开接口包括：

- MainMenuBase(MainWindow* mainWindow)
- addMenu(QString title, QList<MenuActionItem*> actions, QMenu* parent = nullptr)
- addToolMenu(QString title, QList<MenuActionItem*> actions, Qt::ToolButtonStyle style = Qt::ToolButtonIconOnly)

它持有主窗口指针 m_MainWindow，说明菜单构造是围绕具体主窗口实例进行的。

### GUIWidgetBase

GUIWidgetBase 继承自 Core::FITKWidget。

它是本应用通用 QWidget 派生基类，主要用于让业务界面部件共享主窗口上下文。

当前接口非常轻量：

- GUIWidgetBase(QWidget* parent = nullptr)
- virtual ~GUIWidgetBase()

### GUIDialogBase

GUIDialogBase 是当前模块最关键的基础类。

它继承 Core::FITKDialog，并直接暴露与操作器、图形预览、日志输出有关的通用能力。

当前可确认的核心接口包括：

- setOperator(EventOper::ParaWidgetInterfaceOper* oper)
- execProfession()
- printLog(int type, QString msg)
- updateGraph(int id)
- previewGraph(int id, EventOper::PreviewType type = EventOper::PT_Default, QColor color = QColor())
- clearPreview()

它还重写了：

- showEvent
- closeEvent
- hideEvent
- accept
- reject

这说明 GUIDialogBase 已经承担了业务参数对话框的公共生命周期管理。

### PropertyChildWidgetBase 与 PropertyEmptyWidget

PropertyChildWidgetBase 是属性页子部件基类。

PropertyEmptyWidget 是空属性页实现，用于当前没有业务对象可显示时占位。

这两个类更多承担布局和占位职责，README 不应把它们写成复杂业务类。

### CompLineEdit

CompLineEdit 继承自 QLineEdit。

从当前头文件看，它是为本应用定制的输入控件扩展点。

## 构建与集成

GUIWidget 通过 CMake 构建为共享库：

- project(GUIWidget VERSION 1.0.0 LANGUAGES CXX)
- add_library(${PROJECT_NAME} SHARED)
- 启用 AUTOMOC、AUTOUIC、AUTORCC
- 产物输出到 output/bin_d 和 output/bin

当前模块同时保留：

- GUIWidget.pro
- GUIWidget.pri
- Visual Studio 工程文件

这说明仓库仍兼容多套工程组织方式，README 里不能把它描述成“只支持 CMake”。

## 使用示例

### 场景 1：构建菜单描述并添加到主窗口

下面的示例只使用当前头文件中确认存在的接口。

```cpp
#include "GUIWidget/MainMenuBase.h"
#include "GUIFrame/MainWindow.h"

GUI::MainWindow* mainWindow = /* 已创建的主窗口 */ nullptr;

GUI::MainMenuBase menu(mainWindow);

auto* importAction = new GUI::MenuActionItem("导入", "actionImportCreate");
auto* exportAction = new GUI::MenuActionItem("导出", "actionExport");

QList<GUI::MenuActionItem*> fileActions;
fileActions << importAction << exportAction;

menu.addMenu("文件", fileActions);
menu.addToolMenu("文件", fileActions, Qt::ToolButtonTextUnderIcon);
```

这里的 action 字符串应和操作器仓库中注册的 action 名保持一致。

### 场景 2：业务对话框复用 GUIDialogBase

```cpp
#include "GUIWidget/GUIDialogBase.h"

class MyDialog : public GUI::GUIDialogBase
{
public:
    explicit MyDialog(QWidget* parent = nullptr)
        : GUI::GUIDialogBase(parent)
    {
    }

    void runPreview(int dataId)
    {
        previewGraph(dataId, EventOper::PT_Default);
    }
};
```

如果对话框绑定了 ParaWidgetInterfaceOper 派生操作器，还可以在合适时机调用 execProfession()。

## 注意事项

- GUIDialogBase 已经和图形操作器、几何命令列表耦合，新增基础能力时要考虑对话框生命周期与图形状态清理。
- MenuActionItem 目前以 action 字符串作为和操作器仓库协作的关键键值，拼写不一致会导致按钮点击后找不到操作器。
- 该模块虽然偏 GUI，但依赖 OCC 与多个内部模块，单独抽离时需要同时处理链接依赖。
- 当前环境仅验证了 Windows 下的仓库结构与接口存在性，Linux 下行为仍以实际构建结果为准。

最后更新时间：2026-04-23