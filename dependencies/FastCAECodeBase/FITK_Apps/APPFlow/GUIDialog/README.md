# GUIDialog - GUI对话框模块

## 简介

GUIDialog 是 FastCAE 集成工具包（FITK）的 GUI 对话框模块，提供各种功能对话框，包括计算设置、几何操作、网格管理等界面组件。

## 模块定位

GUIDialog 位于项目根目录，提供应用程序的各种对话框界面，主要承担以下职责：

- 提供计算相关对话框（边界条件、初始条件、求解设置等）
- 提供几何操作对话框（布尔操作、基本几何体创建等）
- 提供网格管理对话框（网格类型设置、局部加密等）
- 作为用户与业务逻辑之间的交互界面

## 架构与目录

```text
GUIDialog/
├── CMakeLists.txt              # 构建配置
├── GUIDialog.pro               # Qt项目文件
├── GUICalculateDialog/         # 计算对话框
│   ├── BoundaryWidget.h/cpp    # 边界条件对话框
│   ├── InitialWidget.h/cpp     # 初始条件对话框
│   ├── SolutionWidget.h/cpp    # 求解设置对话框
│   ├── TurbulenceWidget.h/cpp  # 湍流模型对话框
│   ├── TransportWidget.h/cpp   # 传输属性对话框
│   ├── ThermoWidget.h/cpp      # 热物理属性对话框
│   ├── RadiationWidget.h/cpp   # 辐射模型对话框
│   ├── OperatingConditionWidget.h/cpp # 操作条件对话框
│   ├── DiscretizationWidget.h/cpp     # 离散设置对话框
│   ├── RegionMeshWidget.h/cpp  # 区域网格对话框
│   ├── RunWidget.h/cpp         # 运行对话框
│   └── PostWidget.h/cpp        # 后处理对话框
├── GUIGeometryDialog/          # 几何对话框
│   ├── GeometryBoolWidget.h/cpp # 布尔操作对话框
│   ├── CudeInfoWidget.h/cpp    # 立方体对话框
│   ├── CylinderInfoWidget.h/cpp # 圆柱体对话框
│   └── SphereInfoWidget.h/cpp  # 球体对话框
└── GUIMeshDialog/              # 网格对话框
    ├── MeshBaseWidget.h/cpp    # 网格基础设置对话框
    ├── MeshGeoWidget.h/cpp     # 几何网格对话框
    ├── LocalGroupInfoWidget.h/cpp # 局部加密对话框
    └── MaterialPointWidget.h/cpp # 材料点对话框
```

## 依赖关系

### 编译依赖

- Qt5::Core
- Qt5::Widgets
- FITKCore（核心对象）
- FITKInterfaceFlowOF（流体接口）

## 主要对话框说明

### GUICalculateDialog

计算相关对话框集合：

- **BoundaryWidget**：边界条件设置
- **InitialWidget**：初始条件设置
- **SolutionWidget**：求解器配置
- **TurbulenceWidget**：湍流模型选择
- **TransportWidget**：传输属性设置
- **ThermoWidget**：热物理属性配置
- **RadiationWidget**：辐射模型设置
- **RunWidget**：求解运行控制

### GUIGeometryDialog

几何操作对话框集合：

- **GeometryBoolWidget**：布尔运算（并、交、差）
- **CudeInfoWidget**：立方体创建
- **CylinderInfoWidget**：圆柱体创建
- **SphereInfoWidget**：球体创建

### GUIMeshDialog

网格管理对话框集合：

- **MeshBaseWidget**：网格基础参数设置
- **MeshGeoWidget**：几何网格设置
- **LocalGroupInfoWidget**：局部加密设置
- **MaterialPointWidget**：材料点管理

## 使用示例

创建边界条件对话框：

```cpp
#include "GUICalculateDialog/BoundaryWidget.h"

// 创建边界条件对话框
GUI::BoundaryWidget* boundaryWidget = new GUI::BoundaryWidget(parent);

// 设置边界条件类型
boundaryWidget->setBoundaryType(Velocity);

// 显示对话框
boundaryWidget->show();
```

## 注意事项

- GUIDialog 包含多个子目录，每个子目录对应一类对话框
- 对话框通过信号槽与业务逻辑交互
- UI 文件通过 Qt Designer 设计
- 对话框通常作为模态窗口使用

最后更新时间：2026-04-30