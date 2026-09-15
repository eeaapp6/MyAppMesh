# FITKOFMeshIO - OpenFOAM 网格读取组件

## 项目简介

FITKOFMeshIO 是 FITK 体系中的 OpenFOAM 网格读取组件。

该模块通过组件接口 `IO::FITKOFMeshReader` 对外提供统一读取入口，内部由 `IO::FITKOpenFOAMMeshReader` 完成 OpenFOAM `polyMesh` 结构解析（`points`、`faces`、`owner`、`neighbour`、`boundary`），并组装为 FITK 的流体网格数据对象（体网格与边界网格）。

## 核心功能

1. 统一网格读取组件入口
- 组件名固定为 `IO::FITKOFMeshReader`，可通过组件容器按名称获取。
- 通过 `setMeshFolder(...)` 与 `setMeshObj(...)` 设置输入目录与输出对象后，调用 `exec(...)` 完成读取。

2. OpenFOAM polyMesh 文件解析
- 支持读取并解析 `points`、`faces`、`owner`、`neighbour`、`boundary` 五类核心文件。
- 支持读取 FoamFile 头信息与注释过滤，兼容常见 OpenFOAM 字典格式。

3. 多区域目录处理
- 支持在给定目录下扫描区域网格，自动定位 `polyMesh` 子目录。
- 每个有效区域构建一个 `FITKFluidRegionsMesh` 并追加到 `FITKUnstructuredFluidMeshVTK`。

4. 网格对象组装
- 将节点/单元关系装配为场网格（`FITKUnstructuredMeshVTK`）。
- 按 boundary 信息构建 `FITKBoundaryMeshVTKManager` 下的边界网格数据。

5. 与网格流程组件联动
- 可被 `FITKMeshGenOF` 的网格后处理器调用，在 OpenFOAM 划分完成后自动回读网格。

## 关键类

- `IO::FITKOFMeshReader`
  - 对外组件入口（继承 `AppFrame::FITKComponentInterface`）。
  - 关键接口：
    - `getComponentName()`：返回组件名 `IO::FITKOFMeshReader`。
    - `setMeshFolder(QString)`：设置网格目录。
    - `setMeshObj(Interface::FITKUnstructuredFluidMeshVTK*)`：设置目标网格对象。
    - `exec(const int indexPort)`：执行读取流程。

- `IO::FITKOpenFOAMMeshReader`
  - OpenFOAM 网格解析器（继承 `Interface::FITKAbstractIO`）。
  - 关键能力：
    - `read()`：读取入口。
    - `readPolyMesh(...)`：读取 polyMesh 目录。
    - `readPoints(...)` / `readFaces(...)` / `readOwner(...)` / `readNeighbour(...)` / `readBoundary(...)`。
    - `setupBoundarys()`：边界网格组装。

## 使用示例

### 示例1：在组件工厂中注册 OpenFOAM 网格读取组件

```cpp
#include "FITK_Component/FITKOFMeshIO/FITKOFMeshReader.h"

QList<AppFrame::FITKComponentInterface *> ComponentFactory::createComponents()
{
    QList<AppFrame::FITKComponentInterface *> componentList;
    ...
    componentList << new IO::FITKOFMeshReader;
    ...
    return componentList;
}
```

## 依赖关系

### 外部依赖
- Qt5

编写日期：2026-04-10
最后更新日期：2026-04-10
