# FITKWidget

## 项目描述

FITKWidget 是 FITK 框架的核心 UI 组件库，提供了丰富的自定义控件和界面元素。该组件库包含了各种用于构建科学计算和工程软件界面的专用控件。

## 主要功能

- **自定义控件**：提供多种自定义 UI 控件
- **表格组件**：强大的表格和树形表格控件
- **对话框组件**：常用的对话框和窗口组件
- **界面布局**：MDI 风格的多文档界面支持

## 技术栈

- **编程语言**：C++
- **框架**：Qt Widgets
- **UI 文件**：Qt Designer (.ui) 文件
- **构建系统**：qmake

## 项目结构

```
FITKWidget/
├── FITKComboBox.cpp/.h                    # 自定义组合框
├── FITKDrawerWidget.cpp/.h/.ui            # 抽屉式控件
├── FITKLicenseFileCheckDialog.cpp/.h/.ui  # 许可证文件检查对话框
├── FITKLockerButton.cpp/.h                # 锁定按钮
├── FITKLockerWidget.cpp/.h                # 锁定控件
├── FITKMdiArea.cpp/.h                     # MDI 区域控件
├── FITKMdiSubWindow.cpp/.h                # MDI 子窗口
├── FITKPluginManageDialog.cpp/.h/.ui      # 插件管理对话框
├── FITKRadioColor.cpp/.h/.ui              # 颜色单选按钮
├── FITKSciNotationLineEdit.cpp/.h         # 科学计数法输入框
├── FITKTableView.cpp/.h                   # 表格视图
├── FITKTableWidget.cpp/.h                 # 表格控件
├── FITKTabWidget.cpp/.h                   # 标签页控件
├── FITKTreeTableWidget.cpp/.h             # 树形表格控件
├── FITKTreeWidget.cpp/.h                  # 树形控件
├── FITKWorkingDirDialog.cpp/.h/.ui        # 工作目录选择对话框
├── TableWidgetFileReadFileDialog.cpp/.h/.ui  # 表格文件读取对话框
├── resource/                              # 资源文件
├── ZH_CN/                                 # 中文翻译文件
├── FITKWidgetAPI.h                        # API 导出宏定义
├── FITKWidget.pri                         # qmake 项目文件
└── FITKWidget.pro                         # 主项目文件
```

## 核心组件

### 表格组件系列
- **FITKTableWidget**：功能强大的表格控件，支持大数据量显示
- **FITKTableView**：基于模型/视图架构的表格视图
- **FITKTreeTableWidget**：树形表格控件，支持层次数据显示
- **FITKTreeWidget**：树形控件，支持分层结构显示

### MDI 界面组件
- **FITKMdiArea**：多文档界面区域，支持多个子窗口管理
- **FITKMdiSubWindow**：MDI 子窗口，提供窗口管理功能

### 对话框组件
- **FITKLicenseFileCheckDialog**：许可证文件检查对话框
- **FITKPluginManageDialog**：插件管理对话框
- **FITKWorkingDirDialog**：工作目录选择对话框

### 输入控件
- **FITKSciNotationLineEdit**：科学计数法输入框，支持数值科学表示
- **FITKComboBox**：自定义组合框控件
- **FITKRadioColor**：颜色选择单选按钮

### 界面布局控件
- **FITKDrawerWidget**：抽屉式控件，支持可收缩的侧边栏
- **FITKTabWidget**：标签页控件
- **FITKLockerWidget**：锁定控件，用于界面状态管理

## 主要特性

- **高性能表格**：支持大数据量的高效显示和操作
- **科学计算支持**：提供科学计数法和数值输入控件
- **国际化支持**：完整的中文翻译支持
- **主题定制**：支持自定义样式和主题
- **插件集成**：与 FITK 插件系统无缝集成
- **许可证管理**：内置许可证检查功能

## 依赖关系

- **Qt Widgets**：GUI 界面框架
- **FITK_Kernel**：依赖 FITK 核心模块

## 编译要求

- Qt 5.x 或更高版本
- C++11 或更高标准
- 支持 qmake 构建系统

## 使用示例

```cpp
#include "FITKTableWidget.h"
#include "FITKMdiArea.h"

// 创建表格控件
FITKTableWidget* table = new FITKTableWidget();
table->setColumnCount(5);
table->setRowCount(100);

// 创建 MDI 区域
FITKMdiArea* mdiArea = new FITKMdiArea();
mdiArea->addSubWindow(table);
```

## 定制化

### 样式定制
支持通过 Qt 样式表定制控件外观：
```css
FITKTableWidget {
    background-color: #ffffff;
    alternate-background-color: #f5f5f5;
}
```

### 翻译定制
翻译文件位于 `ZH_CN/` 目录，可以添加其他语言支持。

## 性能优化

- 虚拟滚动技术支持大数据量显示
- 延迟加载机制减少内存占用
- 高效的渲染算法提升界面响应速度


