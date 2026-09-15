# FITKRenderWindowVTK

## 项目描述

FITKRenderWindowVTK 是 FITK 框架的 VTK 渲染窗口组件，提供了基于 VTK（Visualization Toolkit）的 3D 图形渲染和交互功能。该组件专门用于科学计算和工程数据的可视化展示。

## 主要功能

- **3D 渲染窗口**：基于 VTK 的 3D 图形渲染
- **交互式操作**：支持旋转、缩放、平移等交互操作
- **图形对象管理**：管理各种 3D 图形对象
- **剪切和裁剪**：支持 3D 对象的剪切和裁剪操作
- **图例和标注**：提供完整的图例和标注功能

## 技术栈

- **编程语言**：C++
- **框架**：Qt Widgets + VTK
- **图形库**：OpenGL
- **构建系统**：qmake

## 项目结构

```
FITKRenderWindowVTK/
├── FITKBoundaryActor.cpp/.h                    # 边界图形对象
├── FITKGraph3DClipDialog.cpp/.h/.ui            # 3D 剪切对话框
├── FITKGraph3DWidgetPlaneCallBack.cpp/.h       # 平面回调函数
├── FITKGraph3DWidgetPlane.cpp/.h               # 平面控件
├── FITKGraph3DWindowInterface.cpp/.h           # 3D 窗口接口
├── FITKGraph3DWindowVTK.cpp/.h/.ui             # VTK 3D 窗口
├── FITKGraphAreaPicker.cpp/.h                  # 区域拾取器
├── FITKGraphInteractionStyle.cpp/.h            # 交互样式
├── FITKGraphObjectVTK.cpp/.h                   # VTK 图形对象
├── FITKGraphOperator.cpp/.h                    # 图形操作器
├── FITKGraphRender.cpp/.h                      # 图形渲染器
├── FITKLegendScaleActor.cpp/.h                 # 图例比例尺
├── FITKIcons/                                  # 图标资源
├── FITKIcons.qrc                               # 图标资源文件
├── OpenGL/                                     # OpenGL 资源
├── OpenGL.qrc                                  # OpenGL 资源文件
├── vtk.pri                                     # VTK 配置文件
├── FITKRenderWindowVTKAPI.h                    # API 导出宏定义
├── FITKRenderWindowVTK.pri                     # qmake 项目文件
└── FITKRenderWindowVTK.pro                     # 主项目文件
```

## 核心组件

### 渲染窗口类
- **FITKGraph3DWindowVTK**：主要的 VTK 3D 渲染窗口
- **FITKGraph3DWindowInterface**：3D 窗口的抽象接口

### 图形对象类
- **FITKGraphObjectVTK**：VTK 图形对象基类
- **FITKBoundaryActor**：边界显示的图形对象

### 交互和操作类
- **FITKGraphInteractionStyle**：自定义交互样式
- **FITKGraphAreaPicker**：区域拾取功能
- **FITKGraphOperator**：图形操作工具

### 对话框和控件
- **FITKGraph3DClipDialog**：3D 剪切操作对话框
- **FITKGraph3DWidgetPlane**：平面控件
- **FITKLegendScaleActor**：图例和比例尺

### 渲染和回调
- **FITKGraphRender**：图形渲染管理
- **FITKGraph3DWidgetPlaneCallBack**：平面交互回调

## 主要特性

- **高性能渲染**：基于 VTK 的高性能 3D 渲染
- **丰富交互**：支持多种 3D 交互操作
- **剪切功能**：支持任意平面和体积剪切
- **拾取功能**：支持对象和区域的拾取操作
- **图例系统**：完整的图例和比例尺显示
- **可定制样式**：支持自定义渲染样式和交互方式

## 支持的图形类型

### 基础几何体
- **点云**：散点数据的 3D 显示
- **线框**：线框模型的显示
- **曲面**：三角网格曲面的显示
- **体数据**：体数据的渲染显示

### 科学可视化
- **等值面**：标量场的等值面显示
- **矢量场**：矢量场的箭头或流线显示
- **张量场**：张量场的椭球或超二次曲面显示
- **流线**：流场的流线显示

### 工程应用
- **网格显示**：有限元和计算流体力学网格
- **边界条件**：边界条件的可视化
- **结果云图**：计算结果的云图显示
- **动画播放**：时间序列数据的动画播放

## 交互功能

### 基础交互
- **旋转**：鼠标左键拖拽旋转视角
- **缩放**：鼠标滚轮或右键拖拽缩放
- **平移**：鼠标中键或 Shift+左键平移
- **拾取**：点击选择对象

### 高级交互
- **剪切平面**：动态创建和调整剪切平面
- **区域选择**：矩形或多边形区域选择
- **测量工具**：距离、角度、面积测量
- **标注功能**：添加文字和几何标注

## 依赖关系

- **VTK**：Visualization Toolkit 可视化库
- **Qt Widgets**：GUI 界面框架
- **OpenGL**：图形渲染 API
- **FITK_Kernel**：依赖 FITK 核心模块

## 编译要求

- Qt 5.x 或更高版本
- VTK 8.x 或更高版本
- OpenGL 3.3 或更高版本
- C++11 或更高标准
- 支持 qmake 构建系统

## 使用示例

```cpp
#include "FITKGraph3DWindowVTK.h"

// 创建 3D 渲染窗口
FITKGraph3DWindowVTK* renderWindow = new FITKGraph3DWindowVTK();

// 添加图形对象
FITKGraphObjectVTK* meshObject = new FITKGraphObjectVTK();
meshObject->setMeshData(meshData);
renderWindow->addObject(meshObject);

// 显示窗口
renderWindow->show();

// 设置剪切平面
renderWindow->enableClipping(true);
renderWindow->setClipPlane(0, 0, 1, 0); // YZ 平面剪切
```

## 性能优化

- **LOD 技术**：层次细节优化，提高大数据量渲染性能
- **视锥体裁剪**：只渲染可见区域的图形对象
- **GPU 加速**：利用 GPU 并行计算加速渲染
- **内存管理**：智能内存管理，减少内存占用

## 配置选项

### 渲染设置
- **背景颜色**：可自定义背景颜色
- **光照设置**：可调整光照强度和方向
- **材质属性**：可设置对象的材质属性
- **透明度**：支持半透明渲染

### 交互设置
- **旋转速度**：可调整旋转灵敏度
- **缩放速度**：可调整缩放灵敏度
- **拾取精度**：可调整拾取的精度范围


