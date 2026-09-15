# FITKGeoCompOCC

## 项目描述

FITKGeoCompOCC 是 FITK 框架的几何计算组件，基于 OpenCASCADE Technology (OCC) 提供了强大的 3D 几何建模和计算功能。该组件专门用于处理复杂的几何运算，包括布尔运算、几何分析、曲面处理等。

## 主要功能

- **3D 几何建模**：基于 OCC 的 3D 几何实体建模
- **布尔运算**：并集、交集、差集等布尔操作
- **几何分析**：几何属性分析和计算
- **曲面处理**：曲面的创建、修改和分析
- **几何修复**：几何模型的修复和优化

## 技术栈

- **编程语言**：C++
- **框架**：Qt
- **几何内核**：OpenCASCADE Technology (OCC)
- **构建系统**：qmake

## 项目结构

```
FITKGeoCompOCC/
├── FITKGeoCompOCCAPI.h              # API 导出宏定义
├── FITKGeoCompOCC.pri               # qmake 项目文件
├── FITKGeoCompOCC.pro               # 主项目文件
└── [其他源文件]
```

## 核心功能

### 几何实体操作
- **基本几何体**：创建点、线、面、体等基本几何元素
- **复杂实体**：构建复杂的 3D 实体模型
- **几何变换**：平移、旋转、缩放等几何变换
- **几何测量**：计算体积、表面积、质心等几何属性

### 布尔运算
- **并集运算**：多个几何体的合并
- **交集运算**：几何体的相交部分
- **差集运算**：从一个几何体中减去另一个
- **切片运算**：几何体的切片操作

### 曲面处理
- **曲面创建**：创建各种类型的曲面
- **曲面编辑**：修改和优化曲面
- **曲面分析**：分析曲面的几何特性
- **曲面缝合**：将多个曲面缝合成实体

## 使用示例

```cpp
#include "FITKGeoCompOCC.h"

// 创建几何计算器
FITKGeoCompOCC* geoComp = new FITKGeoCompOCC();

// 创建基本几何体
geoComp->createBox(10, 10, 10);  // 创建 10x10x10 的立方体
geoComp->createSphere(5);        // 创建半径为 5 的球体

// 执行布尔运算
geoComp->booleanOperation(UNION, boxId, sphereId);

// 计算几何属性
double volume = geoComp->calculateVolume(resultId);
double area = geoComp->calculateSurfaceArea(resultId);
```

## 应用领域

- **CAD 建模**：计算机辅助设计建模
- **CAE 前处理**：计算机辅助工程前处理
- **逆向工程**：点云到几何模型的转换
- **几何修复**：修复破损或低质量的几何模型

