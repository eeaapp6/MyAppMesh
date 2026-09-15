# FITKPlotWindow

## 项目描述

FITKPlotWindow 是 FITK 框架的专业 2D 数据可视化组件，提供了丰富的科学和工程数据绘图功能。该组件基于 QWT（Qt Widgets for Technical Applications）库，支持多种类型的 2D 图表，包括曲线图、柱状图、极坐标图和频谱图等，专为科学计算和工程分析设计。

## 主要功能

- **2D 数据绘图**：支持多种 2D 图表类型
- **交互式操作**：支持缩放、平移、数据拾取等交互
- **多图表支持**：支持在同一窗口显示多个图表
- **数据导入导出**：支持多种数据格式的导入导出
- **样式定制**：丰富的图表样式和外观定制
- **实时更新**：支持数据的实时更新和动态显示

## 技术栈

- **编程语言**：C++
- **框架**：Qt Widgets
- **绘图库**：QWT (Qt Widgets for Technical Applications)
- **构建系统**：qmake

## 项目结构

```
FITKPlotWindow/
├── FITKBarChartDrawManager.cpp/.h        # 柱状图绘制管理器
├── FITKBarChartProperty.cpp/.h           # 柱状图属性
├── FITKBarChartWindow.cpp/.h/.ui         # 柱状图窗口
├── FITKPolarCurveDrawManager.cpp/.h      # 极坐标曲线绘制管理器
├── FITKPolarPlotProperty.cpp/.h          # 极坐标图属性
├── FITKPolarPlotWindow.cpp/.h/.ui        # 极坐标图窗口
├── FITKSpectrogramManager.cpp/.h         # 频谱图管理器
├── FITKSpectrogramProperty.cpp/.h        # 频谱图属性
├── FITKSpectrogramWindow.cpp/.h/.ui      # 频谱图窗口
├── FITKXYCurveDrawManager.cpp/.h         # XY 曲线绘制管理器
├── FITKXYPlotCurveStyle.cpp/.h/.ui       # XY 曲线样式
├── FITKXYPlotProperty.cpp/.h             # XY 图属性
├── FITKXYPlotWindow.cpp/.h/.ui           # XY 图窗口
├── FITKPlotWindowsInterface.cpp/.h       # 绘图窗口接口
├── FITKPlotWindowAPI.h                   # API 导出宏定义
├── FITKPlotWindow.pri                    # qmake 项目文件
├── FITKPlotWindow.pro                    # 主项目文件
├── qwt.pri                               # QWT 库配置文件
└── [UI文件]                              # Qt Designer 界面文件
```

## 核心组件

### XY 图表组件
- **FITKXYPlotWindow**：XY 坐标系图表窗口
- **FITKXYCurveDrawManager**：XY 曲线绘制管理器
- **FITKXYPlotCurveStyle**：曲线样式管理
- **FITKXYPlotProperty**：XY 图属性管理

### 柱状图组件
- **FITKBarChartWindow**：柱状图显示窗口
- **FITKBarChartDrawManager**：柱状图绘制管理器
- **FITKBarChartProperty**：柱状图属性管理

### 极坐标图组件
- **FITKPolarPlotWindow**：极坐标图表窗口
- **FITKPolarCurveDrawManager**：极坐标曲线绘制管理器
- **FITKPolarPlotProperty**：极坐标图属性管理

### 频谱图组件
- **FITKSpectrogramWindow**：频谱图显示窗口
- **FITKSpectrogramManager**：频谱图管理器
- **FITKSpectrogramProperty**：频谱图属性管理

## 图表类型

### XY 图表
- **线图**：连续数据点的线条连接
- **散点图**：离散数据点的显示
- **阶梯图**：阶梯状的数据显示
- **棒图**：垂直或水平棒状图
- **面积图**：填充区域图
- **误差棒图**：带误差棒的数据显示

### 柱状图
- **垂直柱状图**：标准的垂直柱状图
- **水平柱状图**：水平方向的柱状图
- **堆叠柱状图**：多个数据系列的堆叠显示
- **分组柱状图**：多组数据的并排显示

### 极坐标图
- **极坐标线图**：极坐标系中的线图
- **极坐标散点图**：极坐标散点数据
- **雷达图**：多维度数据的雷达图
- **极坐标面积图**：极坐标填充图

### 频谱图
- **2D 频谱图**：颜色编码的 2D 频谱显示
- **瀑布图**：时间序列频谱图
- **等值线图**：频谱等值线显示
- **3D 频谱图**：3D 频谱显示

## 交互功能

### 基础交互
- **缩放**：鼠标滚轮缩放、框选缩放
- **平移**：鼠标拖拽平移图表
- **数据拾取**：鼠标悬停显示数据值
- **图例交互**：点击图例显示/隐藏曲线

### 高级交互
- **标记功能**：添加数据标记和注释
- **测量工具**：距离、角度、面积测量
- **数据编辑**：直接编辑数据点
- **曲线拟合**：内置曲线拟合功能

## 样式定制

### 曲线样式
```cpp
// 设置曲线样式
FITKXYPlotCurveStyle* curveStyle = new FITKXYPlotCurveStyle();
curveStyle->setLineColor(QColor(255, 0, 0));
curveStyle->setLineWidth(2.0);
curveStyle->setLineStyle(Qt::SolidLine);
curveStyle->setSymbol(QwtSymbol::Ellipse);
curveStyle->setSymbolSize(8);
```

### 坐标轴样式
```cpp
// 设置坐标轴
plot->setAxisTitle(QwtPlot::xBottom, "Time (s)");
plot->setAxisTitle(QwtPlot::yLeft, "Amplitude");
plot->setAxisScale(QwtPlot::xBottom, 0, 100);
plot->setAxisScale(QwtPlot::yLeft, -1, 1);
```

### 网格和背景
```cpp
// 设置网格
QwtPlotGrid* grid = new QwtPlotGrid();
grid->setPen(QPen(Qt::gray, 0, Qt::DotLine));
grid->attach(plot);

// 设置背景
plot->setCanvasBackground(QColor(240, 240, 240));
```

## 使用示例

### 基本 XY 图表
```cpp
#include "FITKXYPlotWindow.h"

// 创建 XY 图表窗口
FITKXYPlotWindow* xyPlot = new FITKXYPlotWindow();

// 设置窗口标题
xyPlot->setWindowTitle("Data Analysis");

// 添加数据曲线
QVector<double> x = {0, 1, 2, 3, 4, 5};
QVector<double> y1 = {0, 2, 4, 2, 0, -2};
QVector<double> y2 = {1, 3, 2, 4, 3, 1};

// 添加第一条曲线
xyPlot->addCurve(x, y1, "Signal 1", Qt::blue);

// 添加第二条曲线
xyPlot->addCurve(x, y2, "Signal 2", Qt::red);

// 设置坐标轴标签
xyPlot->setAxisTitle(QwtPlot::xBottom, "Time (s)");
xyPlot->setAxisTitle(QwtPlot::yLeft, "Amplitude");

// 显示窗口
xyPlot->show();
```

### 柱状图示例
```cpp
#include "FITKBarChartWindow.h"

// 创建柱状图窗口
FITKBarChartWindow* barChart = new FITKBarChartWindow();

// 设置数据
QStringList categories = {"A", "B", "C", "D", "E"};
QVector<double> values = {10, 25, 15, 30, 20};

// 添加数据
barChart->setData(categories, values);

// 设置标题
barChart->setWindowTitle("Category Analysis");

// 显示窗口
barChart->show();
```

### 极坐标图示例
```cpp
#include "FITKPolarPlotWindow.h"

// 创建极坐标图窗口
FITKPolarPlotWindow* polarPlot = new FITKPolarPlotWindow();

// 设置极坐标数据
QVector<double> angles = {0, 45, 90, 135, 180, 225, 270, 315};
QVector<double> radii = {1, 2, 1.5, 3, 2.5, 1.8, 2.2, 1.2};

// 添加数据
polarPlot->addPolarCurve(angles, radii, "Polar Data");

// 显示窗口
polarPlot->show();
```

### 频谱图示例
```cpp
#include "FITKSpectrogramWindow.h"

// 创建频谱图窗口
FITKSpectrogramWindow* spectrogram = new FITKSpectrogramWindow();

// 设置频谱数据
QVector<double> xData(100), yData(100);
QVector<double> zData(100 * 100);

// 填充数据...
for (int i = 0; i < 100; ++i) {
    xData[i] = i * 0.1;
    yData[i] = i * 0.1;
    for (int j = 0; j < 100; ++j) {
        zData[i * 100 + j] = sin(i * 0.1) * cos(j * 0.1);
    }
}

// 设置数据
spectrogram->setData(xData, yData, zData);

// 显示窗口
spectrogram->show();
```

## 数据格式支持

### 输入格式
- **CSV 文件**：逗号分隔值文件
- **TXT 文件**：文本格式数据文件
- **JSON 格式**：JSON 格式数据
- **XML 格式**：XML 格式数据
- **二进制格式**：自定义二进制格式

### 输出格式
- **PNG 图片**：高质量 PNG 图片导出
- **JPG 图片**：JPG 格式图片导出
- **PDF 文档**：PDF 矢量图导出
- **SVG 矢量图**：SVG 矢量格式导出
- **CSV 数据**：数据导出为 CSV 格式

## 性能优化

### 大数据量处理
- **数据抽样**：大数据量的智能抽样显示
- **虚拟化**：大数据集的虚拟化显示
- **延迟加载**：按需加载数据
- **缓存机制**：绘图结果缓存

### 渲染优化
- **硬件加速**：利用 GPU 加速渲染
- **双缓冲**：减少闪烁的双缓冲技术
- **增量更新**：只更新变化的部分
- **LOD 技术**：层次细节优化

## 配置选项

### 全局配置
```cpp
// 设置默认字体
FITKPlotWindow::setDefaultFont(QFont("Arial", 10));

// 设置默认颜色主题
FITKPlotWindow::setColorTheme(FITKPlotWindow::DarkTheme);

// 设置渲染质量
FITKPlotWindow::setRenderQuality(FITKPlotWindow::HighQuality);
```

### 图表配置
```cpp
// 设置图表尺寸
plot->setMinimumSize(800, 600);
plot->setMaximumSize(1920, 1080);

// 设置边距
plot->setContentsMargins(20, 20, 20, 20);

// 自动缩放
plot->setAutoReplot(true);
```

## 依赖关系

- **Qt Widgets**：GUI 界面框架
- **QWT Library**：科学绘图库
- **FITK_Kernel**：依赖 FITK 核心模块

## 编译要求

- Qt 5.x 或更高版本
- QWT 6.x 或更高版本
- C++11 或更高标准
- 支持 qmake 构建系统

## 应用场景

- **科学数据可视化**：实验数据和仿真结果的可视化
- **工程分析**：工程测试数据的分析和显示
- **实时监控**：实时数据的监控和趋势分析
- **报告生成**：技术报告和论文中的图表生成
- **教学演示**：科学和工程教学的数据演示



## 贡献指南

1. 遵循项目的编码规范
2. 新增图表类型时请更新相应的测试用例
3. 样式定制功能请保持向后兼容性
4. 提交前请确保所有示例代码正常运行