# APPRadioss - Radioss 前处理与可视化应用程序

## 项目简介

APPRadioss 是基于 FITK 框架构建的 Radioss 有限元分析软件前处理与可视化桌面应用程序，支持算例管理、几何模型操作、网格生成、材料定义、边界条件设置、求解配置及后处理结果显示等功能。

# 编译说明

务必查看 Doc 文件夹下的编译视频 

## 架构与目录

```text
APPRadioss
├── AppRadioss（应用程序入口：初始化、主窗口、组件工厂）
├── GraphDataProvider（数据提供者：模型、后处理、标记）
├── GraphDataVTKAdaptor（VTK适配器：渲染对象、视图适配器）
├── GUIFrame（主窗口框架：主窗口、中心窗口、消息显示）
├── GUIWidget（界面组件：树控件、图形区域、控制面板）
├── OperatorsInterface（操作器接口：图形事件、树事件基类）
├── OperatorsModel（模型操作器：新建、保存、导入导出）
├── OperatorsGUI（GUI操作器：树动作、图形交互、结果显示）
├── PickDataProvider（拾取数据：拾取信息、数据计算）
└── GUIDialog（业务对话框：分析、几何、网格、后处理）
```

## 核心模块说明

### AppRadioss - 应用程序主模块

负责应用程序的初始化、主窗口生成和组件注册。

- `AppInitializer`：应用程序初始化器
- `MainWindowGenerator`：主窗口生成器
- `ComponentFactory`：组件工厂
- `RadiossAppSettings`：运行时设置管理

### GraphDataProvider - 三维数据提供者

管理 Radioss 数据的可视化对象，采用单例模式。

- `GraphProviderManager`：数据提供者管理器
- `GraphProviderBase`：可视化数据基类
- `GraphModelProvider`：模型数据提供者
- `GraphPostProvider`：后处理数据提供者

### GraphDataVTKAdaptor - VTK渲染适配器

将数据对象转换为 VTK 渲染对象。

**对象层**：
- `GraphVTKObject3D`（基类）
- `GraphVTKObjectModel`（几何模型）
- `GraphVTKObjectPartMesh`（部件网格）
- `GraphVTKObjectSurf`（表面）
- `GraphVTKObjectBCS`（边界条件）
- `GraphVTKObjectPostModel`（后处理模型）

**视图适配器层**：
- `GraphVTKViewAdaptorBase`（基类）
- `GraphVTKViewAdaptorModel`（模型显示）
- `GraphVTKViewAdaptorSurf`（表面显示）
- `GraphVTKViewAdaptorField`（场数据显示）

### GUIFrame - 主窗口框架

负责应用程序主窗口界面构建。

- `MainWindow`：主窗口类
- `CentralWidget`：中心窗口
- `ActionEventHandler`：动作事件处理器

### GUIWidget - 界面组件

继承自 FITKWidget，提供 Radioss 特定的界面组件。

- `GUITreeWidgetBase`：树形菜单基类
- `TreeWidgetAssembly`：装配树
- `TreeWidgetSolution`：求解设置树
- `TreeWidgetProperty`：属性树
- `TreeWidgetGroup`：组管理树
- `ResultTreeWidget`：结果树
- `GraphArea`：图形区域
- `GraphMdiArea`：多文档界面区域

### OperatorsInterface - 操作器接口

定义操作器的抽象接口。

- `GraphEventOperator`：图形事件操作器基类
- `TreeEventOperator`：树形控件事件操作器基类
- `GraphInteractionOperator`：图形交互操作器

### OperatorsModel - 模型操作器

实现各种业务操作器。

- `OperatorsNewProject`：新建工程
- `OperOpenHDF5` / `OperSaveHDF5`：HDF5文件读写
- `OperatorImportRad` / `OperatorExportRad`：Radioss格式导入导出
- `OperatorMaterialProp` / `OperatorProperty`：材料和属性定义
- `OperatorBCS`：边界条件设置
- `OperGmshGenerator`：Gmsh网格生成
- `OperatorOpenRadiossSolve`：Radioss求解器启动

### OperatorsGUI - GUI操作器

连接用户界面和业务逻辑。

- `OperTreeAction`：树形菜单动作操作器基类
- `OperTreeAssemblyAction` / `OperTreeAssemblyEvent`：装配树动作和事件
- `OperTreeSolutionAction` / `OperTreeSolutionEvent`：求解设置树动作和事件
- `OperTreePropertyAction` / `OperTreePropertyEvent`：属性树动作和事件
- `OperGraphPick` / `OperGraphPickPreview`：图形拾取和预览

### PickDataProvider - 拾取数据提供

处理三维对象的拾取操作。

- `PickedDataProvider`：拾取数据提供者
- `PickedData`：拾取数据结构
- `PickedDataCalculator`：拾取数据计算器

### GUIDialog - 业务对话框

各种业务对话框用于用户交互式输入。

```text
GUIDialog
├── GUIAnalysisDialog（分析对话框：材料、属性、边界条件、接触）
├── GUIGeometryDialog（几何对话框：平移、旋转、缩放、镜像）
├── GUIMeshDialog（网格对话框：部件选择）
├── GUIPostDialog（后处理对话框：求解运行、结果请求、颜色条）
└── GUIToolDialog（工具对话框：图形设置、文件目录、批处理）
```

## 依赖关系

### 编译依赖

- **Qt5**：Core、Gui、Widgets、Designer、Xml
- **VTK**：渲染和可视化
- **HDF5**：数据文件读写
- **FITK_Kernel**：FITKCore、FITKAppFramework、FITKAdaptor
- **FITK_Component**：FITKWidget、FITKRenderWindowVTK、FITKRadiossData、FITKRadiossRadIO

### 模块层级关系

```text
FITKCore（基础层）
    ↓
FITKAppFramework / FITKAdaptor（框架层）
    ↓
FITK_Interface / FITK_Component（接口与组件层）
    ↓
APPRadioss（应用层）
```

## 使用示例

### 获取主窗口实例

```cpp
#include "GUIFrame/MainWindow.h"

GUI::MainWindow* mainWindow = GUI::MainWindow::GetMainWindowFromFramework();
if (mainWindow) {
    mainWindow->showMaximized();
}
```

### 获取数据提供者

```cpp
#include "GraphDataProvider/GraphProviderManager.h"

GraphData::GraphProviderManager* manager = GraphData::GraphProviderManager::getInstance();
GraphData::GraphModelProvider* modelProvider = manager->getModelProvider(graphWidget);
```

### 触发操作器

```cpp
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

Core::FITKOperatorRepo* operRepo = Core::FITKOperatorRepo::getInstance();
ModelOper::OperatorsNewProject* newOper = operRepo->getOperatorT<ModelOper::OperatorsNewProject>("actionNew");
if (newOper) {
    newOper->exec();
}
```

## 注意事项

1. **初始化顺序**：AppRadioss 必须在其他模块之前完成初始化
2. **单例管理**：GraphProviderManager 采用单例模式，程序结束时需调用销毁方法
3. **操作器注册**：所有自定义操作器需通过 `Register2FITKOperatorRepo` 宏注册
4. **VTK上下文**：GraphDataVTKAdaptor 依赖 VTK 渲染上下文，应在主线程中初始化和销毁

---

编写日期：2026-04-28  
最后更新日期：2026-04-28
