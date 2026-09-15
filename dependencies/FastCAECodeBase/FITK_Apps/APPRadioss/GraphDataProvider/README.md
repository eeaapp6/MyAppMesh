# GraphDataProvider - 三维可视化数据提供者模块

## 简介

GraphDataProvider 负责在数据对象和图形渲染窗口之间建立数据管理和分发机制。通过与 GraphDataVTKAdaptor 协作，按数据类型（模型、标记、后处理）分类管理可视化对象的创建、更新和销毁。

## 模块定位

- 按数据类型分类管理可视化对象
- 以窗口为粒度的可视化对象管理器生命周期管理
- 支持按数据 ID 动态创建、更新和销毁可视化对象
- 管理可视化对象的显示、隐藏和高亮状态

## 架构与目录

```text
GraphDataProvider
├── GraphProviderManager.h / .cpp    单例管理器
├── GraphProviderBase.h / .cpp        抽象基类
├── GraphModelProvider.h / .cpp       模型数据提供者
├── GraphMarkProvider.h / .cpp        标记数据提供者
├── GraphPostProvider.h / .cpp        后处理数据提供者
└── GraphDataProviderAPI.h
```

## 依赖关系

- Qt5::Core
- FITKCore、FITKVTKGraphAdaptor
- FITKGraph3DWindowVTK、FITKRadiossData

## 主要类与功能说明

### GraphProviderManager

单例管理器，统一管理所有窗口的可视化对象提供者：

```cpp
// 获取模型可视化提供者
GraphModelProvider* modelProvider = GraphProviderManager::getInstance()->getModelProvider(graphWidget);

// 按数据ID更新可视化对象
GraphProviderManager::getInstance()->updateObjectById(dataObjId);
```

### GraphProviderBase

可视化数据提供者抽象基类：

- `getCurrentGraphObjs()` - 获取当前算例已实例化的可视化对象（纯虚）
- `getCurrentVisibleGraphObjs()` - 获取当前可见的可视化对象
- `updateObjById()` - 按数据 ID 更新可视化对象
- `updateVisibility()` - 更新全部显示状态
- `clearHighlight()` - 清除高亮效果

### GraphModelProvider

继承自 GraphProviderBase，管理模型相关数据的可视化对象：

- `getModelGraphObject()` - 几何模型
- `getPartGraphObject()` - Part 网格
- `getRWallGraphObject()` - 刚体墙
- `getGravityGraphObject()` - 重力载荷
- `getBCSGraphObject()` - 边界条件
- `getInteractionGraphObject()` - 相互作用

### GraphMarkProvider / GraphPostProvider

分别管理标记符号类和后处理结果数据的可视化对象。

## 构建与集成

```cmake
project(GraphDataProvider VERSION 1.0.0)
add_library(${PROJECT_NAME} SHARED)
link_fitk_library(${PROJECT_NAME} FITKCore VTK_COMPONENTS)
```

## 注意事项

- GraphProviderManager 是单例，全局唯一
- GraphProviderBase 的构造函数和析构函数受 friend 保护，只能由 GraphProviderManager 创建和销毁
- 可视化对象创建依赖 FITKViewAdaptorFactory 中的适配器注册

最后更新时间：2026-04-28