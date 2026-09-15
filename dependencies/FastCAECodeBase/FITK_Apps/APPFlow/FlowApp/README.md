# FlowApp - 应用程序入口模块

## 简介

FlowApp 是 FastCAE 集成工具包（FITK）的应用程序入口模块，负责初始化应用框架、注册核心组件、管理主窗口和处理应用生命周期。

## 模块定位

FlowApp 位于项目根目录，是应用程序的入口点，主要承担以下职责：

- 初始化 Qt 应用程序和 FITK 应用框架
- 注册全局数据工厂、组件工厂、主窗口生成器等核心组件
- 处理命令行参数和系统检查
- 管理应用程序的启动和退出流程

## 架构与目录

```text
FlowApp/
├── main.cpp                   # 应用入口函数
├── MainWindowGenerator.h/cpp  # 主窗口生成器
├── GlobalDataFactory.h/cpp    # 全局数据工厂
├── ComponentFactory.h/cpp     # 组件工厂
├── CommandLineHandler.h/cpp   # 命令行处理器
├── SignalProcessor.h/cpp      # 信号处理器
├── FlowAPPSettings.h/cpp      # 应用设置
├── WorkBenchHandler.h/cpp     # 工作台处理器
├── SystemChecker.h/cpp        # 系统检查器
└── PreWindowInitializer.h/cpp # 窗口初始化器
```

## 依赖关系

### 编译依赖

- Qt5::Core
- Qt5::Widgets
- FITKAppFramework（应用框架）
- FITKCore（核心对象）

## 主要类与功能说明

### `MainWindowGenerator`

主窗口生成器，继承自 `FITKAbstractMainwindowGenerator`。负责生成应用程序的主窗口。

**主要方法：**
- `genMainWindow()`：生成主窗口实例

### `GlobalDataFactory`

全局数据工厂，负责创建和管理全局数据对象。

### `ComponentFactory`

组件工厂，负责注册和管理应用组件。

### `CommandLineHandler`

命令行处理器，负责解析和处理命令行参数。

### `SignalProcessor`

信号处理器，负责处理全局信号和事件。

### `FlowAPPSettings`

应用设置类，管理应用程序的配置信息。

### `WorkBenchHandler`

工作台处理器，负责管理工作bench的状态和行为。

### `SystemChecker`

系统检查器，负责检查系统环境和依赖。

## 启动流程

应用程序启动流程如下：

1. **初始化 Qt 应用**：设置高DPI缩放属性
2. **初始化 FITK 应用框架**：创建 `FITKApplication` 实例
3. **系统检查**：调用 `SystemChecker` 检查系统环境
4. **注册组件**：
   - 注册主窗口生成器
   - 注册全局数据工厂
   - 注册组件工厂
   - 注册应用设置
   - 注册命令行处理器
   - 添加信号处理器
   - 注册工作台处理器
5. **运行消息循环**：启动应用程序的事件循环

## 使用示例

应用程序通过 `main.cpp` 入口启动：

```cpp
#include "MainWindowGenerator.h"
#include "GlobalDataFactory.h"
#include "ComponentFactory.h"

int main(int argc, char *argv[])
{
    // 初始化应用框架
    AppFrame::FITKApplication app(argc, argv);
    
    // 系统检查
    app.checkSystem(new SystemChecker);
    
    // 注册组件
    app.regMainWindowGenerator(new MainWindowGenerator);
    app.regGlobalDataFactory(new GlobalDataFactory);
    app.regComponentsFactory(new ComponentFactory);
    app.regAppSettings(new FlowAPPSettings);
    app.regCommandLineHandler(new CommandLineHandler);
    app.addGolbalSignalProcesser(new SignalProcessor);
    app.regWorkBenchHandler(new FlowAppWorkBenchHandler);
    
    // 运行应用
    return app.exec();
}
```

## 注意事项

- FlowApp 是应用程序的入口模块，不包含业务逻辑
- 所有核心组件通过工厂模式注册到应用框架
- 系统检查确保运行环境满足要求
- 主窗口生成器负责创建 GUI 主界面

最后更新时间：2026-04-30