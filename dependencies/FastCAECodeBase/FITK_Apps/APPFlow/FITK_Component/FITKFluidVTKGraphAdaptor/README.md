# FITKFluidVTKGraphAdaptor

## 项目描述

FITKFluidVTKGraphAdaptor 是 FITK 框架的流体 VTK 图形适配器组件，专门用于将流体数据适配到 VTK 可视化管道。该组件提供了流体数据到 VTK 图形对象的高效转换，支持各种流场可视化技术。

## 主要功能

- **流体数据适配**：将流体数据适配为 VTK 可视化格式
- **流场可视化**：支持速度场、压力场等流场可视化
- **矢量场显示**：箭头、流线等矢量场可视化
- **标量场显示**：云图、等值面等标量场可视化
- **动态更新**：支持实时数据更新和动画播放

## 技术栈

- **编程语言**：C++
- **框架**：Qt + VTK
- **可视化库**：VTK (Visualization Toolkit)
- **构建系统**：qmake

## 项目结构

```
FITKFluidVTKGraphAdaptor/
├── FITKFluidVTKGraphAdaptorAPI.h    # API 导出宏定义
├── FITKFluidVTKGraphAdaptor.pri     # qmake 项目文件
├── FITKFluidVTKGraphAdaptor.pro     # 主项目文件
└── [其他源文件]
```

## 核心功能

### 数据适配
- **网格适配**：流体网格到 VTK 网格的转换
- **标量场适配**：压力、温度等标量场适配
- **矢量场适配**：速度、力等矢量场适配
- **张量场适配**：应力、应变等张量场适配

### 可视化技术
- **流线显示**：3D 流线的生成和显示
- **箭头场**：矢量箭头场的显示
- **等值面**：标量场的等值面显示
- **云图显示**：标量场的云图显示
- **粒子追踪**：粒子追踪和路径显示

### 动态功能
- **实时更新**：数据的实时更新显示
- **动画播放**：时间序列数据的动画播放
- **交互控制**：可视化参数的交互控制
- **缓存管理**：高效的缓存和内存管理

## 可视化类型

### 矢量场可视化
- **箭头图**：离散位置的矢量箭头
- **流线图**：基于速度场的流线
- **流管图**：流线管状显示
- **粒子动画**：粒子运动轨迹

### 标量场可视化
- **云图**：彩色云图显示
- **等值线**：2D 等值线显示
- **等值面**：3D 等值面显示
- **体积渲染**：3D 体积渲染

### 复合可视化
- **组合显示**：多种可视化技术的组合
- **叠加显示**：不同数据类型的叠加
- **透明混合**：半透明效果显示

## 使用示例

```cpp
#include "FITKFluidVTKGraphAdaptor.h"

// 创建适配器
FITKFluidVTKGraphAdaptor* adaptor = new FITKFluidVTKGraphAdaptor();

// 设置流体数据
adaptor->setFluidData(fluidMesh, velocityField, pressureField);

// 创建流线可视化
adaptor->createStreamLines(seedPoints, lineCount);

// 创建云图可视化
adaptor->createContour(pressureField, contourLevels);

// 更新显示
adaptor->updateDisplay();
```

## 性能优化

- **GPU 加速**：利用 GPU 并行计算
- **LOD 技术**：层次细节优化
- **流式处理**：大数据量的流式处理
- **内存优化**：高效的内存管理

## 应用场景

- **CFD 后处理**：计算流体力学结果可视化
- **流场分析**：流场特性的直观分析
- **教学演示**：流体力学教学演示
- **工程报告**：工程分析和报告展示

