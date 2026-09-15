# FITKCompMessageWidget - 消息控制台组件

## 简介

FITKCompMessageWidget 是 FITK_Component 下的消息控制台组件模块，用于在 FastCAE 应用中提供统一的消息显示窗口。

该模块将框架消息、Qt 日志消息和用户可见的控制台交互封装为组件 `Comp::ConsoleComponent` 与控件 `Comp::FITKConsole`。上层应用或组件工厂注册后，可以按端口索引获取控制台窗口并嵌入主界面、停靠窗口或其他容器中。

## 架构与职责

模块由两个核心类组成：

- `Comp::ConsoleComponent`
  - 组件入口类，继承 `QObject` 和 `AppFrame::FITKComponentInterface`。
  - 负责按端口索引创建并缓存 `FITKConsole` 实例。
  - 对外暴露组件名 `MessageConsole`，供上层通过组件仓库按名查询。

- `Comp::FITKConsole`
  - 控制台显示控件，继承 `QTextEdit` 和 `Core::FITKAbstractGUIObject`。
  - 监听 `FITKSignalTransfer::outputMessageSig(int, QString)` 并将消息格式化显示。
  - 负责右键清空、保存文本、颜色配置，以及可选的 Qt 消息接管。

典型调用关系如下：

1. 上层在组件工厂中创建 `ConsoleComponent`。
2. 业务界面通过组件名 `MessageConsole` 获取组件实例。
3. 调用 `getWidget(indexPort)` 获取某个端口对应的控制台控件。
4. `FITKConsole` 接收框架信号并显示 `Normal`、`Warning`、`Error`、`Info` 等消息。

## 依赖

### 内部依赖

- `FITKCore`
- `FITKAppFramework`

### 外部依赖

- Qt5 `Core`
- Qt5 `Gui`
- Qt5 `Widgets`

### 运行时依赖

- 依赖 `FITKAPP` 可用，以便获取 `FITKSignalTransfer` 并接收框架消息。
- 若启用 Qt 日志接管，运行期间会安装全局 Qt 消息处理函数。

## 主要类与功能说明

### `Comp::ConsoleComponent`

主要职责：

- 管理多个控制台窗口实例。
- 为上层返回指定端口的控制台控件。
- 控制是否接管 Qt 的 `qInfo`、`qWarning`、`qCritical` 等输出。

关键接口：

- `QString getComponentName()`
  - 返回固定组件名 `MessageConsole`。
- `QWidget* getWidget(const int indexPort = 0)`
  - 根据端口索引返回对应控制台控件。
  - 同一端口会复用已创建的控件实例。
- `void takeoverQtMessage(bool takeover = true)`
  - 控制新创建控制台是否安装 Qt 消息回调。

### `Comp::FITKConsole`

主要职责：

- 显示框架消息和 Qt 消息。
- 根据消息类型设置不同文本样式。
- 提供右键清空和保存功能。

关键接口：

- `void displayMessage(MessageType type, const QString message)`
- `void setNormalTextColor(QColor color)`
- `void setWarningTextColor(QColor color)`
- `void setErrorTextColor(QColor color)`

支持的消息类型：

- `Normal`
- `Warning`
- `Error`
- `Info`

## 目录结构

当前模块中对理解最关键的文件如下：

- `FITKConsoleComponent.h/.cpp`
  - 组件入口与控制台实例管理。
- `FITKConsole.h/.cpp`
  - 控制台显示逻辑、消息格式化、右键菜单和保存行为。
- `FITKCompMessageWidgetAPI.h`
  - 模块导出宏定义。
- `CMakeLists.txt`
  - 动态库构建配置，输出到上层工程 `output/bin_d` 与 `output/bin`。

## 构建与集成方式

该模块是一个动态库组件，不独立作为应用程序运行，而是通过上层工程的 `FITK_Component/CMakeLists.txt` 纳入整体构建。

当前构建事实：

- 目标名：`FITKCompMessageWidget`
- 目标类型：`SHARED`
- Debug 输出目录：`output/bin_d`
- Release 输出目录：`output/bin`

若要在上层工程中使用该模块，需要确保：

1. `FITK_Component/CMakeLists.txt` 已添加该子目录。
2. 上层应用在组件创建阶段实例化 `Comp::ConsoleComponent`。
3. 运行时相关依赖库已正确部署到可执行程序目录。

## 使用方法

### 1. 注册组件

通常在组件工厂或应用初始化阶段创建 `Comp::ConsoleComponent`，并传入主窗口或宿主父对象。

### 2. 获取控制台控件

通过组件系统按名称获取 `MessageConsole`，然后调用 `getWidget(indexPort)` 获取控件。

### 3. 输出消息

框架层通过 `FITKSignalTransfer::outputMessageSig(int, QString)` 发送消息，控制台会自动接收并显示。

### 4. 可选接管 Qt 日志

在创建控件前可调用 `takeoverQtMessage(true)`，此后由控制台接收 Qt 日志消息。

## 使用示例

### 示例1：在组件工厂中注册消息控制台

```cpp
#include "FITK_Component/FITKCompMessageWidget/FITKConsoleComponent.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

QList<AppFrame::FITKComponentInterface*> createComponents()
{
  QList<AppFrame::FITKComponentInterface*> componentList;
  componentList << new Comp::ConsoleComponent(FITKAPP->getGlobalData()->getMainWindow());
  return componentList;
}
```

### 示例2：按组件名获取并嵌入控制台窗口

```cpp
#include "FITK_Component/FITKCompMessageWidget/FITKConsoleComponent.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"

void attachConsoleToLayout(QLayout* layout)
{
  auto comp = FITKAPP->getComponents()->getComponentByName("MessageConsole");
  auto consoleComp = dynamic_cast<Comp::ConsoleComponent*>(comp);
  if (consoleComp == nullptr || layout == nullptr) return;

  layout->addWidget(consoleComp->getWidget(0));
}
```

## 注意事项与限制

- `takeoverQtMessage(true)` 会安装全局 Qt 消息处理函数，适合在应用级统一控制台场景中使用。
- 控制台实例按端口索引缓存，同一端口会返回同一个窗口实例。
- `Info` 类型消息当前直接显示原始文本，不附加和其他类型相同的标签颜色格式。
- 模块当前仅提供消息展示能力，不负责消息产生与业务过滤。

最后更新时间：2026-04-21
