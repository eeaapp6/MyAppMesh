# FITKCompMessageWidget

## 项目描述

FITKCompMessageWidget 是 FITK 框架的消息和显示组件，提供了控制台和消息展示功能。该组件主要用于应用程序内部的消息传递、日志显示和用户交互界面。

## 主要功能

- **控制台组件**：提供类似终端的控制台界面
- **消息显示**：支持多种消息类型的显示
- **日志记录**：集成日志记录功能
- **用户交互**：支持用户输入和命令执行

## 技术栈

- **编程语言**：C++
- **框架**：Qt Widgets
- **构建系统**：qmake

## 项目结构

```
FITKCompMessageWidget/
├── FITKConsoleComponent.cpp/.h    # 控制台组件类
├── FITKConsole.cpp/.h            # 控制台实现类
├── FITKCompMessageWidgetAPI.h    # API 导出宏定义
├── FITKCompMessageWidget.pri     # qmake 项目文件
├── FITKCompMessageWidget.pro     # 主项目文件
├── Resource/                     # 资源文件目录
├── ZH_CN/                        # 中文翻译文件
├── Translations_Update.bat       # 翻译更新脚本
└── Translations_Release.bat      # 翻译发布脚本
```

## 核心类

### FITKConsoleComponent
控制台组件的主要接口类，提供组件管理和初始化功能。

### FITKConsole
控制台实现类，负责具体的控制台功能实现，包括：
- 消息显示
- 用户输入处理
- 命令执行
- 历史记录管理

## 主要特性

- **多语言支持**：支持中文和英文界面
- **消息过滤**：支持按级别过滤消息
- **历史记录**：保存命令和消息历史
- **可定制界面**：支持自定义颜色和字体
- **实时更新**：支持实时消息显示

## 依赖关系

- **Qt Widgets**：GUI 界面框架
- **FITK_Kernel**：依赖 FITK 核心模块

## 编译要求

- Qt 5.x 或更高版本
- C++11 或更高标准
- 支持 qmake 构建系统

## 使用示例

```cpp
#include "FITKConsoleComponent.h"

// 创建控制台组件
FITKConsoleComponent* console = new FITKConsoleComponent();

// 初始化控制台
console->initialize();

// 显示消息
console->printMessage("Hello, FITK!");

// 执行命令
console->executeCommand("help");
```

## 国际化

组件支持国际化，翻译文件位于 `ZH_CN/` 目录下：
- `*.ts` 文件：翻译源文件
- `*.qm` 文件：编译后的翻译文件

使用以下脚本管理翻译：
- `Translations_Update.bat`：更新翻译文件
- `Translations_Release.bat`：发布翻译文件

## 配置选项

控制台组件支持多种配置选项：
- 消息颜色设置
- 字体大小设置
- 历史记录长度
- 自动滚动设置

## 贡献指南

1. 遵循项目的编码规范
2. 添加新功能时请更新相应的翻译文件
3. 提交前请确保所有测试通过