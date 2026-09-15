# FITKPlotWindow - 数据图表组件

## 简介

FITKPlotWindow 是 FITK_Component 组件层中提供的数据可视化图表组件库，基于Qwt（Qt Widgets for Technical Applications）库构建。该模块提供XY曲线图、频谱图、极坐标图等多种图表类型，支持科学数据和仿真结果的可视化展示。

FITKPlotWindow 的核心价值在于"提供专业的数据可视化能力"。通过封装Qwt库的功能，该模块为CAE应用程序提供统一的图表显示接口，支持数据的曲线绘制、对比分析和结果展示。

## 模块定位

FITKPlotWindow 位于 `FITK_Component` 下，属于数据可视化组件模块，主要承担以下职责：

- 提供XY直角坐标系图表（`FITKXYPlotWindow`）。
- 提供频谱图/热力图（`FITKSpectrogramWindow`）。
- 提供极坐标图（`FITKPolarPlotWindow`）。
- 定义图表样式和属性管理。

从架构层级来看，FITKPlotWindow 位于 FITKCore 图形抽象层之上，是二维数据可视化能力的具体实现层。

## 架构与目录

```text
FITKPlotWindow
├── XY曲线图
│   ├── FITKXYPlotWindow
│   ├── FITKXYPlotCurve
│   ├── FITKXYPlotProperty
│   └── FITKXYCurveDrawManager
├── 频谱图
│   ├── FITKSpectrogramWindow
│   ├── FITKSpectrogramProperty
│   └── FITKSpectrogramManager
├── 极坐标图
│   ├── FITKPolarPlotWindow
│   └── FITKPolarPlotProperty
└── API导出
    └── FITKPlotWindowAPI
```

## 依赖关系

### 编译依赖

- Qt5::Core
- Qt5::Widgets
- Qwt（通过 qwt.pri 配置）
- FITKCore（FITKAbstractGraphWidget）

### 模块层级关系

- FITKPlotWindow 直接依赖 `FITKCore` 和 `Qwt`，是二维数据可视化能力的具体实现。
- 后处理组件依赖本模块展示仿真结果。
- 本模块定义的图表类型可被多个应用程序共享使用。

## 主要类与功能说明

### XY曲线图

#### `FITKXYPlotWindow`

XY直角坐标系图表窗口，继承自 `FITKAbstractGraph2DWidget`。该类是曲线绘制的主容器，支持多条曲线的叠加显示。

主要功能包括：

- 曲线管理：添加、删除、更新曲线。
- 坐标轴控制：设置坐标轴范围、标签、刻度。
- 视图操作：缩放、平移、拖拽。
- 工具栏：提供常用的视图操作工具。
- 图像导出：支持将图表导出为图像文件。

#### `FITKXYPlotCurve`

曲线数据类，封装了单条曲线的数据和样式信息。

#### `FITKXYPlotProperty`

图表属性类，定义了图表的显示属性，如标题、图例、网格等。

#### `FITKXYCurveDrawManager`

曲线绘制管理器，负责管理图表中的曲线绘制和更新。

### 频谱图

#### `FITKSpectrogramWindow`

频谱图/热力图窗口，用于显示二维数据的颜色映射分布。

主要功能包括：

- 数据映射：将数值映射为颜色。
- 色标管理：显示颜色与数值的对应关系。
- 等值线：支持显示等值线。
- 交互操作：支持数据的查看和分析。

#### `FITKSpectrogramProperty`

频谱图属性类，定义了频谱图的显示属性，如颜色映射范围、颜色方案等。

#### `FITKSpectrogramManager`

频谱图管理器，负责管理频谱图的数据和显示更新。

### 极坐标图

#### `FITKPolarPlotWindow`

极坐标图表窗口，用于显示极坐标系下的数据分布。

主要功能包括：

- 极坐标绘制：在极坐标系中绘制数据。
- 角度控制：设置角度范围和方向。
- 半径控制：设置半径范围。
- 多曲线支持：支持多条极坐标曲线叠加显示。

#### `FITKPolarPlotProperty`

极坐标图属性类，定义了极坐标图的显示属性。

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `project(FITKPlotWindow LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`
- 通过 `link_fitk_library(${PROJECT_NAME} FITKCore)` 链接底层模块
- 通过 QwtConfig.cmake 配置链接 Qwt 库

### 在上层工程中的参与方式

FITKPlotWindow 作为数据可视化组件，一般由后处理组件或结果展示模块进行链接和调用。典型的接入方式是在主窗口中嵌入图表窗口组件，然后在运行时添加曲线或数据。

## 使用示例

### 1. 创建XY曲线图

```cpp
#include "FITKXYPlotWindow.h"
#include "FITKXYCurveDrawManager.h"

// 创建图表窗口
Plot::FITKXYPlotWindow* plotWindow = new Plot::FITKXYPlotWindow(this);

// 创建曲线
Plot::FITKXYCurveDrawProp* curve = new Plot::FITKXYCurveDrawProp();

// 添加数据点
QVector<double> xData = {1, 2, 3, 4, 5};
QVector<double> yData = {1, 4, 9, 16, 25};
curve->setData("Data1", xData, yData);

// 添加曲线到图表
plotWindow->getCurveManager()->appendGraphObj(curve);

// 更新显示
plotWindow->updataCanvas();
```

### 2. 配置图表属性

```cpp
#include "FITKXYPlotProperty.h"

// 获取属性
Plot::FITKXYPlotProperty* property = plotWindow->getPlotProperty();

// 设置标题
property->setTitle("Simulation Results");

// 设置坐标轴标签
property->setAxisName("Time (s)", "Displacement (mm)");

// 设置图例
property->setLegendVisible(true);
```

### 3. 使用频谱图

```cpp
#include "FITKSpectrogramWindow.h"

// 创建频谱图窗口
Plot::FITKSpectrogramWindow* spectrogramWindow = new Plot::FITKSpectrogramWindow(this);

// 获取频谱图管理器
Plot::FITKSpectrogramManager* spectrogramManager = spectrogramWindow->getSpectrogramManager();

// 设置数据
QVector<double> xData = {1.0, 2.0, 3.0};
QVector<double> yData = {1.0, 2.0, 3.0};
QVector<QVector<double>> valueData = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
spectrogramManager->setData(xData, yData, valueData);
```

### 4. 保存图表

```cpp
// 保存为图像
plotWindow->saveImage("plot.png");
```

## 使用建议

- 根据数据特点选择合适的图表类型（XY曲线、频谱图、极坐标图）。
- 曲线数量过多时考虑分组显示或使用图例控制可见性。
- 图表属性设置在添加数据之前进行，避免频繁的重绘。
- 大数据量场景下使用数据抽稀或分时加载。

## 注意事项

- Qwt库的初始化需要在Qt事件循环启动前完成。
- 图表的更新使用批量更新模式，避免频繁的重绘。
- 颜色映射需要考虑数据的分布特点。
- 图表的导出在后台线程执行，避免阻塞主界面。

最后更新时间：2026-04-22
