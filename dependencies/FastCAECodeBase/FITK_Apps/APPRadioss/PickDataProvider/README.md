# PickDataProvider - 拾取数据管理模块

## 简介

PickDataProvider 管理用户在图形窗口中进行拾取操作时产生的数据，支持单个拾取、框选拾取、角度拾取等多种拾取方式。

## 模块定位

- 管理拾取数据的存储和生命周期
- 支持多种拾取方式的数据生成
- 提供拾取结果的 VTK 数据集供渲染使用
- 计算和提取拾取元素的相关信息

## 架构与目录

```text
PickDataProvider
├── PickedDataProvider.h / .cpp        拾取数据管理单例
├── PickedData.h / .cpp                拾取数据封装类
├── PickedDataCalculator.h / .cpp      拾取数据计算类
├── GUIPickInfo.h / .cpp               拾取参数静态管理
└── PickDataProviderAPI.h
```

## 依赖关系

- Qt5::Core、VTK
- FITKCore、FITKInterfaceModel、FITKRenderWindowVTK

## 主要类与功能说明

### PickedDataProvider

拾取数据管理单例类：

```cpp
// 生成拾取数据
PickedDataProvider::getInstance()->generatePickedData(actor, index, worldPos, false);

// 获取拾取数据集
vtkDataSet* data = PickedDataProvider::getInstance()->getPickedDataSet();

// 获取拾取数据列表
auto pickedList = PickedDataProvider::getInstance()->getPickedData();
```

### GUIPickInfo

拾取参数静态管理类：

```cpp
// 拾取对象类型
enum PickObjType { POBMeshVert, POBMeshCell, POBMeshCellSurf, ... };

// 拾取方式
enum PickMethod { PMSingle, PMIndividually, PMByAngle };

GUIPickInfo::SetPickInfo(info);  // 设置拾取参数
GUIPickInfo::GetPickObjType();   // 获取拾取对象类型
```

### PickedDataCalculator

拾取数据计算类，提供角度拾取等复杂算法。

## 注意事项

- PickedDataProvider 是单例，通过 `getInstance()` 访问
- 拾取数据在每次新的拾取操作前自动清空
- GUIPickInfo 使用静态方法管理拾取参数

最后更新时间：2026-04-28