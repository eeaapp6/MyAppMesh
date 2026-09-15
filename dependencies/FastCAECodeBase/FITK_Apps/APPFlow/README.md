# APPFlow - 流体仿真应用模块

## 简介

APPFlow 是 FastCAE 集成工具包（FITK）的流体仿真应用模块，专注于计算流体动力学（CFD）仿真。该应用集成了几何建模、网格生成、求解器配置、仿真运行和后处理分析等完整工作流程，支持稳态和瞬态流动分析、多种湍流模型以及热-流耦合等复杂物理场景。

## 模块定位

APPFlow 作为 FITK 框架的流体仿真应用入口，主要承担以下职责：

- 提供完整的 CFD 仿真工作流程
- 集成几何建模、网格划分、求解配置功能
- 支持 OpenFOAM 求解器的驱动和管理
- 提供结果可视化和后处理分析能力
- 作为上层应用与底层组件的集成桥梁

## 架构与目录

```text
APPFlow/
├── FlowApp/                    # 应用入口模块
│   ├── main.cpp                # 主入口函数
│   ├── MainWindowGenerator.h   # 主窗口生成器
│   ├── ComponentFactory.h      # 组件工厂
│   └── GlobalDataFactory.h     # 全局数据工厂
├── GUIFrame/                   # GUI框架模块
│   ├── MainWindow.h            # 主窗口类
│   ├── MainMenu.h              # 主菜单
│   ├── MainTreeWidget.h        # 树结构视图
│   └── RenderWidget.h          # 渲染窗口
├── GUIWidget/                  # GUI基础部件
│   ├── TreeWidget.h            # 树控件
│   ├── PickedData.h            # 拾取数据
│   └── GUIDialogBase.h         # 对话框基类
├── GUIDialog/                  # GUI对话框模块
│   ├── GUICalculateDialog/     # 计算相关对话框
│   ├── GUIGeometryDialog/      # 几何操作对话框
│   └── GUIMeshDialog/          # 网格管理对话框
├── OperatorsModel/             # 操作器模型
│   ├── OperatorsNew.h          # 新建操作
│   ├── OperatorsRun.h          # 运行操作
│   ├── OperatorsPost.h         # 后处理操作
│   └── OperatorsMeshManager.h  # 网格管理器
├── OperatorsGUI/               # 操作器GUI
│   ├── OperGraphEvent3D.h      # 3D交互事件
│   └── OperToolbarViews.h      # 工具栏视图
├── OperatorsInterface/         # 操作器接口
├── GraphDataProvider/          # 图形数据提供
│   ├── GraphProviderManager.h  # 数据管理器
│   └── GraphModelProvider.h    # 模型数据提供器
├── Resources/                  # 资源文件（图标等）
└── Tools/                      # 第三方工具库
```

## 依赖关系

### 编译依赖

- Qt 5.15+（GUI框架）
- VTK 8.90+（3D可视化）
- OpenFOAM（求解器后端）

### FITK模块依赖

- FITKCore（核心对象和数据模型）
- FITKAppFramework（应用程序框架）
- FITK_Component（组件层：网格、求解器驱动等）
- FITK_Interface（接口层：流体仿真接口）

## 主要模块说明

### FlowApp

应用入口模块，负责初始化应用框架、注册核心组件和管理应用生命周期。

**核心类：**
- `MainWindowGenerator`：主窗口生成器
- `ComponentFactory`：组件工厂
- `GlobalDataFactory`：全局数据工厂

### GUIFrame

GUI框架模块，提供主窗口、菜单、工具栏和属性面板等界面组件。

**核心类：**
- `MainWindow`：主窗口类
- `MainTreeWidget`：树结构视图
- `RenderWidget`：3D渲染窗口

### OperatorsModel

操作器模型模块，提供各种操作器的业务逻辑实现。

**核心类：**
- `OperatorsNew`：新建项目操作
- `OperatorsOpen`：打开项目操作
- `OperatorsRun`：运行求解操作
- `OperatorsPost`：后处理操作
- `OperatorsBoundaryConditions`：边界条件管理

### GraphDataProvider

图形数据提供模块，管理三维可视化数据。

**核心类：**
- `GraphProviderManager`：数据管理器（单例）
- `GraphModelProvider`：模型数据提供器
- `GraphMarkProvider`：标记数据提供器

## 构建与集成

### CMake

项目通过 CMake 构建，根目录包含主 CMakeLists.txt，各子模块有独立的构建配置。

### 集成方式

```cpp
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "MainWindowGenerator.h"
#include "ComponentFactory.h"

int main(int argc, char *argv[])
{
    // 初始化应用框架
    AppFrame::FITKApplication app(argc, argv);
    
    // 注册核心组件
    app.regMainWindowGenerator(new MainWindowGenerator);
    app.regComponentsFactory(new ComponentFactory);
    app.regGlobalDataFactory(new GlobalDataFactory);
    
    // 运行应用
    return app.exec();
}
```

## 典型工作流程

1. **新建项目** → 创建流体仿真项目
2. **几何建模** → 导入或创建几何模型
3. **网格划分** → 生成计算网格
4. **物理设置** → 配置边界条件和物理模型
5. **求解配置** → 设置求解器参数
6. **运行求解** → 启动 OpenFOAM 求解
7. **后处理** → 可视化和分析结果

## 支持的物理模型

- **湍流模型**：k-ε、k-ω、SST k-ω、Spalart-Allmaras、LES
- **热物理**：能量方程、热传导、辐射模型
- **多相流**：VOF、Mixture 模型
- **热-流耦合**：共轭传热（CHT）

## 注意事项

- 需要正确配置 OpenFOAM 环境变量
- 网格质量直接影响求解收敛性
- 复杂模型建议使用并行求解
- 后处理需要足够的内存资源

## 许可证

本项目遵循 LGPL-2.1 许可证，详见 License.txt。

最后更新时间：2026-04-30