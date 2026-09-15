# GraphDataProvider - 图形数据提供模块

## 简介

GraphDataProvider 是 FastCAE 集成工具包（FITK）的图形数据提供模块，负责管理三维可视化数据，包括模型数据、标记数据等，为渲染模块提供统一的数据接口。

## 模块定位

GraphDataProvider 位于项目根目录，主要承担以下职责：

- 管理三维模型可视化对象
- 管理符号标记可视化对象
- 提供数据更新和刷新机制
- 处理拾取信息和演员状态

## 架构与目录

```text
GraphDataProvider/
├── GraphDataProviderAPI.h      # API声明
├── GraphProviderBase.h         # 提供器基类
├── GraphProviderManager.h/cpp  # 提供器管理器（单例）
├── GraphModelProvider.h/cpp    # 模型数据提供器
└── GraphMarkProvider.h/cpp     # 标记数据提供器
```

## 依赖关系

### 编译依赖

- Qt5::Core
- FITKCore（核心对象）
- FITKRenderWindowVTK（渲染模块）

## 主要类与功能说明

### `GraphProviderManager`

图形数据管理器，单例模式。管理所有三维数据分装类。

**主要方法：**
- `getInstance()`：获取单例实例
- `Delete()`：手动销毁实例
- `getModelProvider()`：获取三维模型数据提供器
- `getMarkProvider()`：获取符号标记数据提供器
- `updateObjectById()`：根据数据ID更新可视化对象
- `setActorStateByPickInfo()`：根据拾取信息设置演员状态

### `GraphModelProvider`

模型数据提供器，管理三维模型可视化对象。

### `GraphMarkProvider`

标记数据提供器，管理符号标记可视化对象。

## 使用示例

```cpp
#include "GraphProviderManager.h"

// 获取数据管理器
GraphData::GraphProviderManager* manager = 
    GraphData::GraphProviderManager::getInstance();

// 获取模型提供器
GraphData::GraphModelProvider* modelProvider = 
    manager->getModelProvider(graphWidget);

// 获取标记提供器
GraphData::GraphMarkProvider* markProvider = 
    manager->getMarkProvider(graphWidget);

// 更新指定ID的对象
manager->updateObjectById(dataId);

// 根据拾取信息设置演员状态
manager->setActorStateByPickInfo(pickType, pickMethod, dataId);
```

## 注意事项

- GraphProviderManager 采用单例模式
- 数据提供器与可视化窗口关联
- 线程安全通过 QMutex 保证
- 需要配合 FITKGraph3DWindowVTK 使用

最后更新时间：2026-04-30