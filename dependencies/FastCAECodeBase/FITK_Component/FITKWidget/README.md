# FITKWidget

## 简介

FITKWidget 是 `FITK_Component` 下的通用 Qt 控件库，用于为 FastCAE 应用提供可复用的界面控件、对话框和轻量级界面容器。它覆盖了颜色选择、表格编辑、抽屉式布局、工作路径设置、插件管理、内存占用显示等常见界面需求。

与 `FITKRenderWindowVTK`、`FITKCGNSIO` 这类通过统一组件接口接入的模块不同，FITKWidget 当前更接近一个共享库形式的控件集合：上层代码通常直接包含头文件并创建控件或对话框实例，而不是通过组件注册表按名称获取。

## 架构

### 模块定位

- 所在层级：`FITK_Component`
- 构建形态：共享库 `FITKWidget`
- 主要职责：为上层 GUI、操作器和应用框架补充常用控件与对话框

### 内部结构

按职责可大致分为以下几类：

- 基础输入控件：`FITKPushButtonColor`、`FITKRadioColor`、`FITKSciNotationLineEdit`、`FITKComboBox`
- 数据展示与编辑控件：`FITKTableWidget`、`FITKTableView`、`FITKTreeWidget`、`FITKTreeTableWidget`
- 容器与布局控件：`FITKDrawerWidget`、`FITKLockerWidget`、`FITKMdiArea`、`FITKMdiSubWindow`、`FITKTabWidget`
- 应用辅助对话框：`FITKWorkingDirDialog`、`FITKPluginManageDialog`、`FITKLicenseFileCheckDialog`、`FITKFileDirectoryDialog`、`TableWidgetFileReadFileDialog`
- 运行信息控件：`FITKRAMProportionBar`

### 与上层工程的关系

- `FITK_Component/CMakeLists.txt` 通过 `add_subdirectory(FITKWidget)` 将该模块纳入组件总工程。
- 上层界面与操作器会直接引用本模块中的头文件并创建实例。
- 部分对话框依赖 `FITKAppFramework` 的全局对象，例如 `FITKAPP->getAppSettings()`、`FITKAPP->getPluginsManager()`。

## 依赖

### 编译依赖

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- FITKAppFramework

### 关键内部依赖

- `FITK_Kernel/FITKCore/FITKAbstractGUI.h`
- `FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h`
- `FITK_Kernel/FITKAppFramework/FITKAppFramework.h`
- `FITK_Kernel/FITKAppFramework/FITKAppSettings.h`
- `FITK_Kernel/FITKAppFramework/FITKPluginManager.h`

### 运行相关说明

- `FITKPluginManageDialog` 需要可访问的插件目录，并依赖运行中的插件管理器。
- `FITKWorkingDirDialog` 依赖 `FITKAPP->getAppSettings()` 非空。
- `FITKRAMProportionBar` 在 Windows 和 Linux 下分别使用不同的系统接口读取内存信息。
- 模块资源由 `resource/FITKWidgetResource.qrc` 提供，例如插件管理对话框使用的图标资源。

## 主要类与功能说明

### `FITKPushButtonColor`

继承自 `QPushButton` 的颜色按钮控件。点击按钮后会弹出 `QColorDialog`，在用户选择新颜色后刷新按钮背景色，并发出 `sigColorChanged(const QColor&)` 信号。

### `FITKComboBox`

对 `QComboBox` 的轻量封装，补充点击信号和可选的滚轮禁用能力，适合在参数面板中避免误滚导致选项变化。

### `FITKTableWidget`

继承自 `QTableWidget` 的增强表格控件，提供按回车追加行、右键剪切/复制/粘贴、插入/删除行、清空内容以及读取文件等操作入口，适合参数录入表格场景。

### `FITKDrawerWidget` 与 `FITKLockerWidget`

用于构造抽屉式折叠界面。`FITKDrawerWidget` 负责管理多个抽屉项，`FITKLockerWidget` 负责单个可折叠面板，两者协作实现前处理参数面板这类纵向折叠布局。

### `FITKWorkingDirDialog`

工作路径设置对话框。构造时会从 `FITKAPP->getAppSettings()` 读取当前工作目录，确认后通过 `setWorkingDir()` 回写配置。

### `FITKPluginManageDialog`

插件管理对话框。通过 `FITKAPP->getPluginsManager()` 获取当前已装载插件列表，并扫描给定目录中的 `.dll` 或 `.so` 文件，在确认时执行卸载与安装。

### `FITKRAMProportionBar`

运行内存占比信息条。内部使用定时器定期读取系统总内存、系统已用内存和当前进程内存，并更新进度条颜色和文本显示。

## 构建与集成方式

### CMake

当前模块通过 `FITK_Component/FITKWidget/CMakeLists.txt` 构建为共享库：

- `project(FITKWidget VERSION 1.0.1 LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`、`AUTORCC`、`AUTOUIC`
- 链接 `Qt5::Core`、`Qt5::Gui`、`Qt5::Widgets`
- 通过 `link_fitk_library(${PROJECT_NAME} FITKAppFramework)` 和 `link_fitk_library(${PROJECT_NAME} FITKCore)` 链接内部底座库

输出目录与当前工程保持一致：

- Debug：`output/bin_d`
- Release：`output/bin`

### qmake

模块同时保留了 `FITKWidget.pro` / `FITKWidget.pri`，用于 qmake 构建路径下的同等接入。其链接依赖与 CMake 配置保持一致，同样依赖 `FITKAppFramework` 与 `FITKCore`。

### 在上层工程中的参与方式

典型接入方式是直接链接 `FITKWidget`，并在界面或操作器代码中包含所需头文件后直接实例化控件，例如：

- 在操作器中创建 `Plugins::FITKPluginManageDialog`
- 在参数面板中嵌入 `FITKPushButtonColor` 或 `FITKTableWidget`
- 在状态区或工具区嵌入 `GUI::FITKRAMProportionBar`

## 使用方法

### 基础控件使用

对于不依赖应用框架的基础控件，直接包含头文件并创建实例即可，例如颜色按钮、下拉框、表格控件、抽屉控件。

### 依赖应用框架的对话框使用

以下对话框在使用前需要确保 FastCAE 应用框架已完成初始化：

- `FITKWorkingDirDialog`：需要 `FITKAPP->getAppSettings()` 可用
- `FITKPluginManageDialog`：需要 `FITKAPP->getPluginsManager()` 可用，并提供正确的插件目录

### 使用边界说明

- 本模块当前没有统一的组件名注册入口，不适用“按组件名获取实例”的接入方式。
- `FITKPluginManageDialog` 只管理指定目录下的动态库文件，不负责生成插件目录。
- `FITKRAMProportionBar` 会启动周期性定时器，适合放在长生命周期界面中使用。

## 使用示例

### 1. 使用颜色按钮控件

适用于参数面板中让用户选择显示颜色的场景。

```cpp
#include "FITK_Component/FITKWidget/FITKPushButtonColor.h"

auto* colorButton = new FITKPushButtonColor(this);
colorButton->setCurrentColor(QColor(255, 0, 0));

connect(colorButton, &FITKPushButtonColor::sigColorChanged,
        this, [=](const QColor& color)
{
    // 根据新颜色刷新业务对象显示
});
```

### 2. 显示插件管理对话框

这与当前工程 `OperatorShowPluginDialog` 中的实际调用方式一致。

```cpp
#include "FITK_Component/FITKWidget/FITKPluginManageDialog.h"

QString pluginPath = QApplication::applicationDirPath() + "/Plugins";
Plugins::FITKPluginManageDialog dialog(pluginPath, mainWindow);
dialog.exec();
```

### 3. 弹出工作目录设置对话框

该示例要求应用框架已初始化，并且 `FITKAPP->getAppSettings()` 可正常返回对象。

```cpp
#include "FITK_Component/FITKWidget/FITKWorkingDirDialog.h"

Comp::FITKWorkingDirDialog dialog(this);
if (dialog.exec() == QDialog::Accepted)
{
    // 工作目录已经在对话框内部写回 AppSettings
}
```

## 注意事项

- FITKWidget 中的类分布较散，新增 README 时更适合按“控件类别”理解，而不是把它视为单一功能组件。
- 依赖 `FITKAppFramework` 的对话框不适合在脱离主应用上下文的纯 Qt 小程序中直接复用，除非同时补齐 `FITKAPP` 相关初始化。
- `FITKRAMProportionBar` 的内存采集实现区分 Windows 与 Linux；当前仓库环境下仅能基于源码确认跨平台逻辑，未在双平台做运行验证。
- 若只需要基础 Qt 控件增强能力，优先选用 `FITKPushButtonColor`、`FITKComboBox`、`FITKTableWidget` 这类与应用框架解耦的类。

最后更新时间：2026-04-30