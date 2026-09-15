# FITKGeoOCCIOHDF5 - OCC 几何 HDF5 读写组件

## 项目简介

FITKGeoOCCIOHDF5 是 FITK 体系中的 OCC 几何数据 HDF5 读写组件。

该模块通过组件接口 `IO::FITKOCCIOHDF5Interface` 统一执行几何命令与基准元素（Datum）的读写，内部由 `FITKOCCHDF5Reader` / `FITKOCCHDF5Writer` 驱动，结合 HDF5 Adaptor 工厂与 OCC 形状序列化工具，实现几何命令树及其 TopoDS_Shape 的持久化。

## 核心功能

1. OCC 几何 HDF5 读取
- 通过 `exec(1)` 触发读取流程。
- 读取 `Geometry` / `Datums` / `PrincipalDatums` 等节点并重建命令数据。
- 依据命令枚举名动态实例化命令：`FITKInterfaceGeometryFactory::createCommand(...)`。
- 读取后执行命令三角化（`triangleShanpe()`），便于后续显示。

2. OCC 几何 HDF5 写出
- 通过 `exec(2)` 触发写出流程。
- 写出几何命令列表、基准元素列表、主基准元素 ID 映射及版本信息。
- 基于 `FITKIOAdaptorFactory` 的 HDF5 适配器写出多种几何命令类型。

3. 统一组件入口与状态控制
- 组件名固定为 `FITKOCCHDF5IO`。
- 支持 `setFileName(...)`、`setDataObject(...)`、`setRunInThread(...)`。
- 支持 `isRunning()`、`isSuccess()` 查询运行状态。
- 支持通过 `ioThreadFinishedSig()` 通知读写完成。

4. 外部 HDF5 Root 复用
- 支持 `setHDF5Root(H5::H5File*)` 注入已有 HDF5 文件句柄。
- 可被上层总工程读写器复用（避免重复开关文件流）。

5. OCC 形状序列化能力
- `FITKOCCIOTool::ShapeToData(...)` / `DataToShape(...)` 支持 TopoDS_Shape 与二进制数据互转。
- `FITKOCCShapeHDF5IOHelper` 将形状数据挂接到命令 HDF5 分组读写。
- 基于 OCC XCAF Bin 文档读写（`FITKOCCShapeReader` / `FITKOCCShapeWriter`）。

## 关键类

- `IO::FITKOCCIOHDF5Interface`
  - 对外组件入口。
  - 关键接口：
    - `setFileName(const QString&)`
    - `setHDF5Root(H5::H5File*)`
    - `setDataObject(Core::FITKAbstractDataObject*)`
    - `setRunInThread(bool)`
    - `exec(int indexPort)`（`1` 读取，`2` 写出）
    - `isRunning()` / `isSuccess()`

- `IO::FITKOCCHDF5Reader`
  - 几何读取执行类（继承 `FITKAbstractHDF5Reader`）。
  - 关键能力：读取命令、基准元素、版本检查、按枚举恢复命令实例。

- `IO::FITKOCCHDF5Writer`
  - 几何写出执行类（继承 `FITKAbstractHDF5Writer`）。
  - 关键能力：写出 `Info/Geometry/Datums/PrincipalDatums` 及版本信息。

- `IO::FITKOCCIOTool` / `IO::FITKOCCShapeIO`
  - OCC 形状序列化与反序列化工具。
  - 关键能力：TopoDS_Shape <-> 二进制数据转换、命令形状恢复。

- `OCC::FITKOCCShapeHDF5IOHelper`
  - 命令形状读写辅助类。
  - 关键能力：将命令关联形状写入/读取 HDF5 子组。

## 使用示例

### 示例 1：独立调用组件进行几何读写

```cpp
#include "FITK_Component/FITKGeoOCCIOHDF5/FITKOCCIOHDF5Interface.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"

bool readOccGeometry(const QString& fileName)
{
    auto* io = FITKAPP->getComponents()->
        getComponentTByName<IO::FITKOCCIOHDF5Interface>("FITKOCCHDF5IO");
    if (io == nullptr || fileName.isEmpty()) return false;

    auto* geoList = FITKAPP->getGlobalData()->
        getGeometryData<Interface::FITKGeoCommandList>();
    if (geoList == nullptr) return false;

    io->setFileName(fileName);
    io->setDataObject(geoList);
    io->setRunInThread(false);

    if (!io->exec(1)) return false;
    return io->isSuccess();
}
```

## 依赖关系

### 外部依赖
- Qt5（Core / Gui / Widgets）
- HDF5
- OpenCascade（OCC）

编写日期：2026-04-09
最后更新日期：2026-04-09
