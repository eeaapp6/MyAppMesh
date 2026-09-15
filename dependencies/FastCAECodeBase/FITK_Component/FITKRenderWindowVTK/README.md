# FITKRenderWindowVTK - VTK三维渲染窗口组件

## 简介

FITKRenderWindowVTK 是 FITK_Component 组件层中提供基于VTK的三维渲染窗口组件，封装了科学可视化中常用的三维图形显示能力。该模块基于VTK库构建，提供高质量的网格、几何、后处理结果的渲染功能，支持交互式的模型观察和操作。

FITKRenderWindowVTK 的核心价值在于"提供高性能的三维可视化渲染能力"。通过封装VTK的渲染管线，该模块为CAE应用程序提供统一的图形显示接口，支持几何模型、网格和仿真结果的实时可视化。

## 模块定位

FITKRenderWindowVTK 位于 `FITK_Component` 下，属于图形渲染组件模块，主要承担以下职责：

- 定义三维渲染窗口的组件入口类（`FITKGraph3DWindowInterface`）以及实际窗口类（`FITKGraph3DWindowVTK`）。
- 提供图形对象的管理和渲染（`FITKGraphObjectVTK`、`FITKGraphRender`）。
- 实现交互式的图形操作（平移、旋转、缩放、选择等）。
- 支持图形输出和导出功能。

从架构层级来看，FITKRenderWindowVTK 位于 FITKCore 图形抽象层之上，是图形能力的具体实现层。

## 架构与目录

```text
FITKRenderWindowVTK
├── 渲染窗口
│   ├── FITKGraph3DWindowVTK
│   └── FITKGraph3DWindowInterface
├── 渲染管理
│   ├── FITKGraphRender
│   └── FITKGraphObjectVTK
├── 交互操作
│   ├── FITKGraphOperator
│   ├── FITKGraphInteractionStyle
│   └── FITKGraphAreaPicker
├── 图形控件
│   └── FITKGraph3DWidgetPlane
├── 图例与标尺
│   └── FITKLegendScaleActor
└── API导出
    └── FITKRenderWindowVTKAPI
```

## 依赖关系

### 编译依赖

- Qt5::Core
- Qt5::Widgets
- VTK（通过 vtk.pri 配置）
- FITKCore（FITKAbstractGraphWidget、FITKAbstractGraphObject）

### 模块层级关系

- FITKRenderWindowVTK 直接依赖 `FITKCore` 和 `VTK`，是图形渲染能力的具体实现。
- 上层应用程序界面模块依赖本模块提供三维可视化能力。
- FITKVTKGraphAdaptor 图形适配器模块依赖本模块进行图形渲染。

## 主要类与功能说明

### 渲染窗口

#### `FITKGraph3DWindowVTK`

三维渲染窗口的主类，继承自 `FITKAbstractGraph3DWidget`。该类是VTK渲染管线与Qt界面的集成点，提供完整的三维图形显示功能。

主要功能包括：

- 渲染初始化：初始化VTK渲染管线、相机、灯光等。
- 视图控制：支持多种视图模式（线框、着色、隐藏线等）。
- 相机操作：支持平移、旋转、缩放、正交/透视切换等。
- 窗口管理：支持多视图、视图分割、视图保存等。
- 图像导出：支持将渲染结果导出为PNG、JPEG、TIFF等格式图像。

#### `FITKGraph3DWindowInterface`

三维渲染窗口的接口类，提供窗口操作的抽象接口。

### 渲染管理

#### `FITKGraphRender`

图形渲染管理器，负责管理渲染场景中的所有图形对象。该类是图形对象与渲染管线之间的桥梁。

主要功能包括：

- 对象管理：添加、删除、更新渲染场景中的图形对象。
- 渲染控制：控制渲染的启动、暂停、更新。
- 场景管理：管理场景的灯光、背景、环境设置。

#### `FITKGraphObjectVTK`

图形对象的VTK实现，继承自 `FITKAbstractGraphObject`。该类是具体图形元素的渲染表示。

### 交互操作

#### `FITKGraphOperator`

图形操作器，提供交互式操作的基础设施。

#### `FITKGraphInteractionStyle`

交互样式类，定义用户与渲染窗口的交互行为。支持的交互模式包括：

- 选择模式：点选、框选、多选等。
- 旋转模式：旋转视图。
- 平移模式：平移视图。
- 缩放模式：缩放视图。
- 测量模式：距离、角度测量。

#### `FITKGraphAreaPicker`

区域拾取器，用于在渲染场景中选择区域内的图形元素。

### 图形控件

#### `FITKGraph3DWidgetPlane`

三维控件平面，用于在渲染场景中显示参考平面或工作平面。

### 图例与标尺

#### `FITKLegendScaleActor`

图例和比例尺显示组件，用于在渲染窗口中显示颜色映射图例和比例尺。

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `project(FITKRenderWindowVTK LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`
- 通过 `link_fitk_library(${PROJECT_NAME} FITKCore)` 链接底层模块
- 通过 VTKConfig.cmake 配置链接 VTK 库

### 在上层工程中的参与方式

FITKRenderWindowVTK 作为三维渲染组件，在当前工程里不是直接由界面代码 `new FITKGraph3DWindowVTK(...)` 创建，而是先注册 `FITKGraph3DWindowInterface` 组件，再由上层界面代码通过组件统一接口获取具体窗口。

当前工程中的典型注册方式在 `ComponentFactory::createComponents()` 中：

```cpp
auto compVTKrender = new Comp::FITKGraph3DWindowInterface;
Comp::FITKGraph3DWindowInterface::SetResolveCoincidentType(
    Comp::ResolveCoincidentType::RCT_PolygonOffset);
componentList << compVTKrender;

compVTKrender->addInitializer(1, new PreWindowInitializer);
```

注册完成后，组件会以名称 `Graph3DWindowVTK` 存在于 `FITKAPP->getComponents()` 中。上层界面模块再通过 `getWidget(indexPort)` 取得具体的 `QWidget` 窗口实例。

## 使用示例

### 1. 注册组件

如果当前应用没有在组件工厂中统一注册该组件，可以手动按与主工程一致的方式注册：

```cpp
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowInterface.h"

auto compVTKrender = new Comp::FITKGraph3DWindowInterface;
Comp::FITKGraph3DWindowInterface::SetResolveCoincidentType(
    Comp::ResolveCoincidentType::RCT_PolygonOffset);

compVTKrender->addInitializer(1, new PreWindowInitializer);
FITKAPP->getComponents()->addComponent(compVTKrender);
```

这里的关键点是：

- `FITKGraph3DWindowInterface` 是统一组件入口
- `addInitializer(1, ...)` 用于为 `getWidget(1)` 创建出的窗口提供初始化器
- 上层代码通常不直接负责保存 `FITKGraph3DWindowVTK` 的构造参数，而是通过组件统一索引获取窗口

### 2. 通过组件统一接口创建渲染窗口

```cpp
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"

AppFrame::FITKComponentInterface* graph3DCompBase =
    FITKAPP->getComponents()->getComponentByName("Graph3DWindowVTK");
if (!graph3DCompBase) return;

QWidget* renderWidget = graph3DCompBase->getWidget(1);
layout->addWidget(renderWidget);

Comp::FITKGraph3DWindowVTK* renderWindow =
    dynamic_cast<Comp::FITKGraph3DWindowVTK*>(renderWidget);
if (renderWindow)
{
    renderWindow->setIsShowActions(true);
    renderWindow->setIsShowClipAction(true);
    renderWindow->SetGlobalWarningDisplay(false);
}
```

这里的 `1` 对应前面注册时的初始化器索引。若调用 `getWidget(1)`，组件会先取出索引 `1` 对应的 `Graph3DWindowInitializer`，再在内部创建 `FITKGraph3DWindowVTK` 窗口。

### 3. 添加图形对象

```cpp
#include "FITKGraphRender.h"
#include "FITKGraphObjectVTK.h"

// 获取渲染管理器
Comp::FITKGraphRender* render = renderWindow->getGraphRender();

// 创建几何图形对象
Comp::FITKGraphObjectVTK* geoObject = new Comp::FITKGraphObjectVTK();
geoObject->setGeometryData(geometry);

// 添加到渲染场景
render->addObject(geoObject);

// 触发渲染更新
render->update();
```

### 4. 控制视图

```cpp
// 重置视角
renderWindow->resetCamera();

// 设置视图模式
renderWindow->setDisplayMode(FITKGraph3DWindowVTK::DisplayMode::Surface);

// 保存图像
renderWindow->saveImage("screenshot.png");
```

### 5. 交互操作

```cpp
// 设置交互模式
renderWindow->setInteractionMode(FITKGraph3DWindowVTK::InteractionMode::Rotate);

// 启用选择
renderWindow->enablePicking(true);

// 连接选择信号
connect(renderWindow, &FITKGraph3DWindowVTK::objectPicked, 
        this, &MyClass::onObjectPicked);
```

## 使用建议

- 优先使用图形适配器添加和管理图形对象，而不是直接操作渲染对象。
- 渲染窗口的更新使用批量更新模式，避免频繁的单独更新。
- 图像导出在后台线程执行，避免阻塞主界面。
- 交互模式根据用户需求选择合适的模式。

## 注意事项

- VTK库的初始化需要在Qt事件循环启动前完成。
- 渲染窗口的销毁需要注意VTK对象的生命周期管理。
- 多窗口场景下需要协调各窗口的渲染更新。
- 图像分辨率设置需要考虑内存使用。

最后更新时间：2026-04-30
