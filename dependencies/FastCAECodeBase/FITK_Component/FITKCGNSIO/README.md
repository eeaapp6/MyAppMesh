# FITKCGNSIO

## 模块简介

FITKCGNSIO 是 FITK 体系中的 CGNS 网格读写组件。

该模块以 `IO::FITKCGNSIOInterface` 作为统一组件入口，对外提供 CGNS 文件读取与写出能力；对内通过 `FITKCGNSReader`、`FITKCGNSWriter` 和 `CGNS::FITKMeshDataProcessToCGNSData` 完成 FITK 网格对象与 CGNS 数据结构之间的转换。

从当前源码可确认的模块定位是：

- 负责 CGNS 文件的打开、读取、写出与关闭
- 负责 FITK 网格对象和集合数据与 CGNS Zone / Section / BC 的映射
- 作为组件层 CGNS IO 能力的统一接入点

当前目录下没有历史 README，本文件内容以当前头文件、实现文件和构建文件为准。

## 架构与目录

按当前源码结构，模块可以分为“组件入口、读写基类、读写实现、数据转换、构建文件”五部分：

```text
FITKCGNSIO
├── 组件入口
│   └── FITKCGNSIOInterface.h / .cpp
├── 读写基类
│   ├── FITKAbstractCGNSReader.h / .cpp
│   └── FITKAbstractCGNSWriter.h / .cpp
├── 读写实现
│   ├── FITKCGNSReader.h / .cpp
│   └── FITKCGNSWriter.h / .cpp
├── 数据转换
│   └── FITKMeshDataProcessToCGNSData.h / .cpp
├── 构建与依赖配置
│   ├── CMakeLists.txt
│   ├── FITKCGNSIO.pro
│   ├── FITKCGNSIO.pri
│   ├── CGNSConfig.cmake
│   └── cgns.pri
└── 其他
    ├── FITKCGNSIOAPI.h
    └── License.txt
```

## 依赖关系

### 编译依赖

根据当前 `CMakeLists.txt` 和 `.pro` 文件，模块显式依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- CGNS::CGNS
- FITKCore
- FITKInterfaceIO
- FITKInterfaceModel
- FITKInterfaceMesh
- FITKAppFramework

### 模块层级关系

- FITKCGNSIO 位于组件层，建立在 IO 接口、网格接口和模型接口之上。
- 上层应用通过组件接口触发 CGNS 文件导入导出。
- 写出链路会消费 `FITKAbstractMesh` 与 `FITKModelSet`。
- 读取链路会创建 `FITKAbstractMesh` 派生对象，并回填集合数据。

## 主要类与数据结构

### `IO::FITKCGNSIOInterface`

组件统一入口，负责：

- 设置文件名 `setFileName(...)`
- 执行读写 `exec(indexPort)`
- 设置与获取 `CGNSDataIO` 数据容器
- 查询执行结果 `isSuccess()`
- 查询当前是否存在 IO 对象 `isRunning()`
- 清理缓存容器 `clearCGNSDataIO()`

当前实现中 `getComponentName()` 返回固定字符串：

```cpp
"IO::FITKCGNSIOInterface"
```

### `IO::CGNSDataIO`

这是组件对外交换数据的核心结构，包含：

- `_mesh`：网格对象列表，类型为 `QList<Interface::FITKAbstractMesh*>`
- `_componentManager`：与网格一一对应的集合列表，类型为 `QList<QList<Interface::FITKModelSet*>>`
- `CreateMeshFun`：读取时用于创建网格对象的工厂函数

读取链路中会直接调用 `CreateMeshFun()` 创建目标网格对象；若该函数未设置或返回空指针，读取会失败。

### `FITKAbstractCGNSReader` / `FITKAbstractCGNSWriter`

两者都继承自 `Interface::FITKAbstractIO`，负责提供：

- 打开 CGNS 文件
- 关闭文件
- 获取 CGNS 文件索引

其中 Reader 额外提供读取进度发送接口 `sendCurrentPercent(...)`。

### `FITKCGNSReader`

负责 CGNS 文件读取。当前源码中可看到的读取链路包括：

- 读取文件版本和精度信息
- 读取 Base / Family
- 读取非结构网格 Zone
- 读取节点坐标
- 读取 Section 单元数据
- 读取边界集合数据

### `FITKCGNSWriter`

负责 CGNS 文件写出。当前源码中可看到的写出链路包括：

- 写出 Base
- 写出非结构网格 Zone
- 写出节点坐标
- 写出 Section 单元数据
- 写出 BC 与 Family 数据

### `CGNS::FITKMeshDataProcessToCGNSData`

写出前的数据转换工具，负责把 FITK 网格和集合对象整理为 CGNS 所需的中间数据，包括：

- 区域名称 `m_name_zone`
- 网格类型 `m_mesh_type`
- 单元最大维度 `m_element_max_dim`
- 节点坐标数组 `m_node_coord`
- 单元分段数据 `m_element_section`
- 边界集合数据 `m_set_BC`

## 执行方式

`IO::FITKCGNSIOInterface::exec(const int indexPort)` 当前使用如下端口约定：

- `1`：读取 CGNS 文件
- `2`：写出 CGNS 文件

实际执行流程为：

1. 检查文件名是否为空
2. 根据 `indexPort` 创建 `FITKCGNSReader` 或 `FITKCGNSWriter`
3. 设置文件名与结果标记
4. 直接调用 `run()` 执行读写
5. 发出 `ioThreadFinishedSig()`
6. 返回 `m_success`

需要特别说明的是：当前实现虽然保留了 `setRunInThread(bool)` 和 `m_runInThread`，但 `exec(...)` 中没有根据该标记切换执行路径，而是直接同步调用 `run()`。因此，当前版本的实际行为应视为同步执行，而不是后台线程执行。

## 当前能力边界

基于当前实现，模块的已实现能力与未实现能力应区分如下：

### 当前已实现

- 非结构 CGNS 网格读取
- 非结构 CGNS 网格写出
- 节点、单元 Section、边界 BC / Family 的读写
- 读取 CGNS 文件版本与精度信息

### 当前未实现或未完成

- 结构网格读取：`FITKCGNSReader::readBase_ZoneStructured()` 当前直接返回 `false`
- 结构网格写出：`FITKCGNSWriter::writeBase_ZoneStructured()` 当前直接返回 `false`

因此，README 不应将当前模块描述为“完整支持结构网格与非结构网格双向读写”。

## 典型用法

### 在应用中注册组件

在当前应用里，`FITKCGNSIOInterface` 不是临时栈对象，而是在组件工厂中统一注册到应用框架，再通过组件管理器按名称获取：

```cpp
#include "FITK_Component/FITKCGNSIO/FITKCGNSIOInterface.h"

QList<AppFrame::FITKComponentInterface *> ComponentFactory::createComponents()
{
    QList<AppFrame::FITKComponentInterface *> componentList;
    componentList << new IO::FITKCGNSIOInterface;
    return componentList;
}
```

这意味着业务代码中的典型接入方式不是 `new IO::FITKCGNSIOInterface`，而是：

```cpp
IO::FITKCGNSIOInterface* interfaceIO =
    FITKAPP->getComponents()->getComponentTByName<IO::FITKCGNSIOInterface>(
        "IO::FITKCGNSIOInterface");
if (!interfaceIO) return;
```

### 写出 CGNS

当前应用中的写出逻辑位于 `OperatorExportCGNSFile` 的线程任务里，流程是“预处理 MeshKernel -> 填充 `CGNSDataIO` -> 从框架取组件 -> 执行写出”：

```cpp
void ExportCGNSWriteThread::run()
{
    IO::CGNSDataIO data;
    data.CreateMeshFun = [&]() { return new Interface::FITKUnstructuredMesh; };

    this->writeCGNSMeshPreProcessing(data);

    auto ofMeshWriter =
        FITKAPP->getComponents()->getComponentTByName<IO::FITKCGNSIOInterface>(
            "IO::FITKCGNSIOInterface");
    if (!ofMeshWriter) return;

    ofMeshWriter->setFileName(_fileName);
    ofMeshWriter->setCGNSDataIO(&data);
    ofMeshWriter->exec(2);

    this->writeCGNSMeshFinishProcessing();
}
```

其中预处理阶段会把应用层 `MeshKernel` 转成组件可识别的数据容器：

```cpp
bool conversionSuccess = convertMeshKernelToCGNS(meshKernel, mesh, setList);
if (conversionSuccess)
{
    data._mesh.append(mesh);
    data._componentManager.append(setList);
}
```

这个例子对应当前应用框架下的真实职责分工：

- 业务层负责从 `MeshKernel` 提取网格与集合
- CGNS 组件只负责把 `CGNSDataIO` 写到文件
- 写出完成后，调用方仍需自行释放 `data._mesh` 和 `data._componentManager` 中创建的对象，再调用 `clearCGNSDataIO()` 清空容器

### 读取 CGNS

当前应用中的读取逻辑也是先准备 `CGNSDataIO`，再把它交给框架内已注册的 CGNS 组件。以 `OperMeshGenerator` 和 `FastCAEGridMeshProcessor` 的组合为例：

```cpp
IO::CGNSDataIO data;
data.CreateMeshFun = [&]() { return new Interface::FITKUnstructuredMeshVTK; };

void *d = &data;
processorMesh->setValue("DataIO", QVariant::fromValue(d));
processorMesh->setValue("File", meshFile);
processorMesh->start(QStringList() << "CGNS");
```

处理器内部再通过应用框架获取组件并执行读取：

```cpp
IO::FITKCGNSIOInterface* interfaceIO =
    FITKAPP->getComponents()->getComponentTByName<IO::FITKCGNSIOInterface>(
        "IO::FITKCGNSIOInterface");
if (!interfaceIO) return;

interfaceIO->setFileName(this->getValueT<QString>("File"));
interfaceIO->setCGNSDataIO((IO::CGNSDataIO*)d);
interfaceIO->setRunInThread(false);
interfaceIO->exec(1);
```

读取完成后，上层业务代码再从组件里取回结果，并把网格接管到应用自己的数据模型中：

```cpp
IO::CGNSDataIO data = interfaceIO->getCGNSDataIO();
for (int i = 0; i < data._mesh.size(); ++i)
{
    Interface::FITKAbstractMesh* mesh = data._mesh.at(i);
    if (!mesh) continue;

    ModelData::MeshKernel* meshKernel = new ModelData::MeshKernel;
    meshData->appendDataObj(meshKernel);
    meshKernel->setMesh(mesh);
}

for (int i = 0; i < data._componentManager.size(); ++i)
{
    QList<Interface::FITKModelSet*> setList = data._componentManager.at(i);
    for (int j = 0; j < setList.size(); ++j)
    {
        Interface::FITKModelSet* set = setList.at(j);
        if (set) delete set;
    }
}
interfaceIO->clearCGNSDataIO();
```

这里有几个和框架集成强相关的点：

- `CreateMeshFun` 返回类型必须匹配调用方后续要接管的网格类型；当前工程里读取 CGNS 时实际返回的是 `Interface::FITKUnstructuredMeshVTK`
- 读取完成后，`_mesh` 中对象的所有权会被上层业务代码转交给 `MeshKernel::setMesh(...)`
- `_componentManager` 中的集合对象当前由调用方手动释放
- `setRunInThread(false)` 在当前实现下不会改变 `exec(...)` 的同步行为，但应用代码仍保留了这层显式设置

## 注意事项

- `setRunInThread(...)` 当前不会改变执行模型，不应把它当作真正的线程开关。
- `ioThreadFinishedSig()` 当前是在 `run()` 返回后同步发出，语义更接近“执行结束通知”。
- `CreateMeshFun` 是读取前的必要条件，未设置时读取会失败。
- 写出时 `_mesh` 与 `_componentManager` 需要按相同索引对应。
- `clearCGNSDataIO()` 只清空容器，不释放容器内对象。
- 当前结构网格读写接口仍是占位实现，集成时应避免走结构网格路径。

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `project(FITKCGNSIO VERSION 0.8.0 LANGUAGES CXX)`
- `add_library(${PROJECT_NAME} SHARED)`
- 启用 `AUTOMOC`、`AUTOUIC`、`AUTORCC`
- 通过 `CGNSConfig.cmake` 引入 `CGNS::CGNS`
- 通过 `link_fitk_library(...)` 链接 FITK 内部模块

### qmake

当前目录同时保留了 `FITKCGNSIO.pro`、`FITKCGNSIO.pri` 与 `cgns.pri`，用于 qmake 构建链路。

## 文档边界

- 本 README 只描述当前仓库中已经能从源码验证的接口和行为。
- 更细粒度的参数、错误语义与对象所有权，应以头文件和实现为准。
- 若后续补齐结构网格能力，应同步更新“当前能力边界”和“典型用法”。

最后更新时间：2026-04-30