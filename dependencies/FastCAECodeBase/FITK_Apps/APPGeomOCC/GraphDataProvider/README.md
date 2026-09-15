# GraphDataProvider

## 简介

GraphDataProvider 是 CADOCC 应用中的三维可视化数据管理模块。

它负责根据业务数据对象、适配器关键字和当前三维窗口，创建、缓存、更新和查询 OCC 图形对象。

该模块不直接定义几何数据结构，也不直接实现具体视图适配器。

它解决的是“业务数据对象如何在运行期被转换成可视化对象，并在窗口内持续更新”的问题。

从职责划分上看，GraphDataProvider 位于：

- FITKAdaptor 机制层之上
- 具体业务操作器之下
- 渲染窗口与数据对象之间

## 模块定位

- 所在目录：GraphDataProvider
- 构建产物：共享库 GraphDataProvider
- 主要使用者：OperatorsGUI、主界面渲染逻辑、拾取与预览相关流程
- 上游依赖：FITKAppFramework、FITKCore、FITKAdaptor、FITKInterfaceGeometry、FITKOCCGraphAdaptor、FITKRenderWindowOCC、Qt、OCC

## 架构与目录

当前模块结构较集中，主要由四个类构成：

```text
GraphDataProvider
├── GraphProviderManager
├── GraphProviderBase
├── GraphModelProvider
├── GraphMarkerProvider
└── 构建文件
    ├── GraphDataProviderAPI.h
    ├── CMakeLists.txt
    ├── .pro / .pri
    └── VS 工程文件
```

这种结构说明本模块强调“管理器 + 基类 + 两类具体提供器”的模式。

## 依赖关系

根据 GraphDataProvider/CMakeLists.txt，当前显式链接：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- OCC::OCC
- FITKAppFramework
- FITKCore
- FITKAdaptor
- FITKInterfaceGeometry
- FITKOCCGraphAdaptor
- FITKRenderWindowOCC

其中最关键的事实是：

- 它直接依赖 FITKAdaptor，说明图形对象创建会经过适配器工厂。
- 它直接依赖 FITKOCCGraphAdaptor 与 FITKRenderWindowOCC，说明当前实现是 OCC 渲染后端专用。

## 主要类与职责

### GraphProviderManager

GraphProviderManager 是模块总入口，也是单例管理器。

已确认存在的核心接口包括：

- static GraphProviderManager* getInstance()
- static void Delete()
- GraphModelProvider* getModelProvider(Render::FITKGraph3DWindowOCC* graphWidget)
- GraphMarkerProvider* getMarkerProvider(Render::FITKGraph3DWindowOCC* graphWidget)
- void updateObjectById(int dataId)

它的职责是：

- 管理具体 provider 的生命周期。
- 按窗口获取模型 provider 与标识 provider。
- 为外部暴露统一更新入口。

### GraphProviderBase

GraphProviderBase 是所有三维数据提供器的公共基类。

它定义了当前模块最关键的抽象接口：

- virtual QList<Exchange::FITKOCCGraphObject3D*> getCurrentGraphObjs() = 0
- QList<Exchange::FITKOCCGraphObject3D*> getCurrentVisibleGraphObjs()
- virtual QString getClassName()
- virtual bool updateObjById(int dataId, QVariant info = QVariant())
- virtual void clearHighlight()

同时还提供受保护的工具方法：

- deleteObjsHash(...)
- getGraphObject(QString adaptorKeyName, QHash<int, Exchange::FITKOCCGraphObject3D*>& objDict, Core::FITKAbstractDataObject* dataObj)
- getGraphObject(QString adaptorKeyName, QString key, QVariant info = QVariant())

这说明 GraphProviderBase 自身已经封装了“按适配器 key 获取图形对象”的共性逻辑。

### GraphModelProvider

GraphModelProvider 继承自 GraphProviderBase。

它用于管理模型实体对应的 OCC 图形对象。

虽然本次没有展开其全部成员函数，但通过 GraphProviderManager::getModelProvider 可以明确看出它是应用中最常用的 provider。

### GraphMarkerProvider

GraphMarkerProvider 同样继承自 GraphProviderBase。

它面向的是工具标识、辅助箭头、方向显示等非主体模型图元。

从名称和管理器接口可以看出，它和模型对象是并列但职责不同的提供器。

## 构建与集成

GraphDataProvider 通过 CMake 构建为共享库：

- project(GraphDataProvider VERSION 1.0.0 LANGUAGES CXX)
- add_library(${PROJECT_NAME} SHARED)
- 启用 Qt 自动生成工具
- 产物输出到 output/bin_d 与 output/bin

当前目录同时保留 qmake 与 VS 工程文件，因此文档描述需要兼容现有多工程实践。

## 模块层级关系

模块在系统中的典型链路如下：

1. OperatorsGUI 触发图形刷新或预览。
2. GraphProviderManager 获取具体 provider。
3. GraphProviderBase 结合数据对象与 adaptor key 创建或获取 FITKOCCGraphObject3D。
4. RenderWindowOCC 把图形对象显示到窗口中。

这条链路说明 GraphDataProvider 并不直接替代视图适配器，而是承接它们的运行时调度。

## 使用示例

### 场景 1：获取模型 provider 并按数据 ID 更新

```cpp
#include "GraphDataProvider/GraphProviderManager.h"

Render::FITKGraph3DWindowOCC* graphWidget = /* 当前 OCC 窗口 */ nullptr;

GraphData::GraphProviderManager* mgr = GraphData::GraphProviderManager::getInstance();
GraphData::GraphModelProvider* modelProvider = mgr->getModelProvider(graphWidget);

if (modelProvider)
{
    modelProvider->updateObjById(dataId);
}
```

### 场景 2：通过统一入口刷新对象

```cpp
#include "GraphDataProvider/GraphProviderManager.h"

GraphData::GraphProviderManager::getInstance()->updateObjectById(dataId);
```

### 场景 3：读取当前可见图形对象列表

```cpp
#include "GraphDataProvider/GraphProviderManager.h"
#include "GraphDataProvider/GraphProviderBase.h"

Render::FITKGraph3DWindowOCC* graphWidget = /* 当前 OCC 窗口 */ nullptr;

auto* provider = GraphData::GraphProviderManager::getInstance()->getMarkerProvider(graphWidget);
QList<Exchange::FITKOCCGraphObject3D*> visibleObjs;

if (provider)
{
    visibleObjs = provider->getCurrentVisibleGraphObjs();
}
```

### 场景 4：在派生 provider 中复用适配器创建图元

下面的片段展示 GraphProviderBase 的真实扩展方式，只使用已确认存在的受保护接口名。

```cpp
class MyProvider : public GraphData::GraphProviderBase
{
public:
    explicit MyProvider(Render::FITKGraph3DWindowOCC* widget)
        : GraphData::GraphProviderBase(widget)
    {
    }

    QList<Exchange::FITKOCCGraphObject3D*> getCurrentGraphObjs() override
    {
        return {};
    }

    bool buildGraph(Core::FITKAbstractDataObject* dataObj)
    {
        QHash<int, Exchange::FITKOCCGraphObject3D*> dict;
        auto* obj = getGraphObject("Model", dict, dataObj);
        return obj != nullptr;
    }
};
```

## 注意事项

- GraphProviderManager 当前维护的是单例 provider，而不是按多个窗口维度存储完整映射，后续若出现多三维窗口并行场景，需要重新核对设计边界。
- GraphProviderBase 的 getGraphObject 依赖 adaptor key 与数据对象类型一致，README 示例中不应把 key 写成任意字符串。
- 当前模块完全建立在 OCC 渲染后端之上，若未来引入其他渲染实现，需要新增平行实现而不是直接改写现有 README 结论。
- 当前环境只核对了接口和构建依赖，具体 provider 内部缓存策略还应以后续源码阅读为准。

最后更新时间：2026-04-23