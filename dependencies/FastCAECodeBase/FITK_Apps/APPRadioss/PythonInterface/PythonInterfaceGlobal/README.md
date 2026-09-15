# PythonInterfaceGlobal

## 简介

PythonInterfaceGlobal 是 AppRadioss 中的 Python 接口模块，位于 PythonInterface 层，负责向 FITKPython 脚本环境暴露全局级操作能力。

与 PythonInterfaceProperties 和 PythonInterfaceSolution 偏向物理属性或求解配置不同，本模块主要覆盖文件操作、几何创建、网格对象编辑、模型集合与模型面访问、网格生成以及求解启动入口，承担脚本自动化流程中的“全局入口层”角色。

## 架构

### 模块定位

- 所属层级：PythonInterface 模块
- 构建产物：共享库 PythonInterfaceGlobal
- 上游依赖：FITKCore、FITKAppFramework、FITKPython、FITKInterfaceModel、FITKInterfaceGeometry、FITKInterfacePhysics、FITKInterfaceMeshGen、FITKRadiossData、FITKHttpPythonDriver、OperatorsInterface
- 下游使用方：AppRadioss 在 PythonRegister 中注册本模块的装饰器类，供脚本环境按包名直接使用

### 内部结构

模块可分为三类能力：

1. 动作型全局接口

   - Files：通过 ActionOperator 触发导入 RAD、导出 RAD、打开工程、保存工程。
   - Solver：通过 actionStartSolver 启动求解，支持基于当前解步或已有配置文件运行。

2. 数据对象包装接口

   - Geometrys：创建并重命名几何对象。
   - Meshs：访问已有网格部件并设置名称、属性、材料。
   - ModelSet：创建节点集合或单元集合，并支持重命名。
   - ModelSurface：按名称访问已有模型面并支持重命名。

3. 流程桥接接口

   - MeshGenerate：把几何对象或几何拓扑转换为网格生成输入，驱动 GmshExec 网格流程并把结果重新导入当前工程。

### 典型调用链

Python 脚本 -> FITKPython 装饰器对象 -> PythonInterfaceGlobal 中的 *Wapper -> ActionOperator / Geometry / Mesh / MeshGen / RadiossCase 数据对象 -> TreeAssemblyEvent / TreeGroupEvent / TreePropertyEvent / TreeSolutionEvent / GraphPreprocess 更新

### 注册到脚本环境的类名与包

- Global：Files、Geometrys、Meshs、MeshGenerate、Solver
- Model：ModelSet、ModelSurface

## 依赖

### 编译依赖

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- FITKAppFramework
- FITKPython
- FITKInterfaceModel
- FITKInterfaceGeometry
- FITKInterfacePhysics
- FITKInterfaceMeshGen
- FITKRadiossData
- FITKHttpPythonDriver
- OperatorsInterface

### 运行依赖

- AppRadioss 已初始化 FITKAppFramework，且全局数据中存在当前 Radioss::FITKRadiossCase。
- 文件操作依赖 actionImportRad、actionExportRad、actionOpen、actionSave 等操作器已注册。
- 求解启动依赖 actionStartSolver 操作器和有效工作目录。
- 网格生成依赖 FITKMeshGenInterface 与 GmshExec 网格驱动可用。
- 几何创建依赖 FITKInterfaceGeometryFactory 以及当前几何命令列表可用。

### 外部与运行时组件

- MeshGenerate 的实际网格划分流程依赖 GmshExec 相关网格生成配置。
- Solver 会根据当前工作目录和 RAD 文件路径组织求解输入，实际求解链路依赖上层求解器启动流程。

### 工程集成关系

- PythonInterface 顶层工程通过 add_subdirectory(PythonInterfaceGlobal) 纳入该模块。
- AppRadioss 主程序通过链接 PythonInterfaceGlobal 集成该模块。
- PythonRegister 在应用启动后把本模块中的装饰器注册到 FITKPython 环境。

## 功能列表

- 导入 RAD、导出 RAD、打开工程和保存工程。
- 创建 Box、Sphere、Cylinder 等几何体。
- 获取已有网格部件并设置名称、属性和材料。
- 创建节点集合或单元集合，并访问已有模型面。
- 基于完整几何或指定面/体拓扑执行 2D/3D 网格生成。
- 启动当前解步求解或基于已存在的 RAD 配置文件启动求解。
- 在几何、网格和集合发生变化后刷新预处理图形和相关树结构。

## 主要类与功能说明

### 动作型接口

#### Files / FilesWapper

FilesWapper 不直接管理复杂数据对象，而是通过 ActionOperator 触发文件级动作。它提供导入 RAD、导出 RAD、打开工程和保存工程等静态入口，并在动作执行后等待线程池完成，适合脚本批处理场景中的工程读写步骤。

#### Solver / SolverWapper

Solver 负责启动求解流程。它可以基于当前解步名称组织 starter/engine 文件路径，也可以直接基于已有 RAD 配置文件启动求解；线程数和工作目录可在脚本侧设置。

### 数据对象包装接口

#### Geometrys / GeometrysWapper

这是几何脚本入口类，负责创建 Box、Sphere 和 Cylinder 几何对象，并支持按名称获取已有几何对象后重命名。几何创建成功后会同步刷新装配树、分组树、属性树、求解树和预处理图形。

#### Meshs / MeshsWapper

用于访问已有网格部件对象，并设置名称、属性和材料。它依赖当前算例中的 MeshModel、MaterialManager 和 SectionManager，因此更适合在几何与网格已存在的前提下做后续绑定。

#### ModelSet / ModelSetWapper

用于创建节点集合或单元集合。创建时会自动生成不重复的名称，单元集合当前实现使用 FITKRadiossElementGroup，并默认设置为 GRSHEL 类型。

#### ModelSurface / ModelSurfaceWapper

用于按名称访问已有模型面对象，并支持重命名。当前实现不提供创建模型面的接口，更偏向已有对象的脚本管理入口。

### 流程桥接接口

#### MeshGenerate / MeshGenerateWapper

这是本模块中流程性最强的入口。它支持两种用法：

- 通过 static_MeshGenerate_GenerateMesh 直接对完整几何命令对象生成网格。
- 通过 addFace 或 addVolume 先收集几何拓扑，再设置网格尺寸并执行 2D 或 3D 网格生成。

内部实现会导出几何到临时目录、驱动 GmshExec 执行网格划分、等待 mesherFinished 信号，并通过 actionGmshSettings 把 msh 结果重新导入为网格部件。

## 目录结构

以下文件最能体现模块职责：

- CMakeLists.txt：定义共享库目标、头源文件列表与链接依赖。
- PythonInterfaceGlobalAPI.h：定义模块导出宏。
- GlobalFiles.h/.cpp：文件操作入口。
- GlobalGeometrys.h/.cpp：几何创建与重命名入口。
- GlobalMeshs.h/.cpp：网格部件访问与材料/属性绑定入口。
- ModelSetWapper.h/.cpp：模型集合创建与命名入口。
- ModelSurfaceWapper.h/.cpp：模型面访问与命名入口。
- MeshGenerateWapper.h/.cpp：网格生成流程桥接入口。
- SolverWapper.h/.cpp：求解启动入口。

## 构建与集成方式

### 在上层工程中的参与方式

本模块不是独立应用程序，而是 AppRadioss 的共享库模块之一。它在应用启动后注册到 Python 环境，作为全局自动化脚本入口使用。

### CMake 集成

1. PythonInterface 顶层工程通过 add_subdirectory(PythonInterfaceGlobal) 纳入该模块。
2. 本模块构建目标名为 PythonInterfaceGlobal。
3. AppRadioss 主程序通过链接 PythonInterfaceGlobal 使用其中的装饰器类。

### 产物位置

- Debug：output/bin_d/PythonInterfaceGlobal.dll
- Release：output/bin/PythonInterfaceGlobal.dll

### 构建前提

- 需要先完成仓库整体初始化和依赖拉取。
- 需要保证 Qt5、FITKInterfaceGeometry、FITKInterfaceMeshGen 等依赖模块可被当前工程正确链接。
- 若要使用 MeshGenerate，还需保证 GmshExec 相关驱动和设置链路在当前环境可用。

## 使用方法

### 使用前提

1. AppRadioss 已启动并完成 PythonRegister::registWapper 注册。
2. 当前环境中存在有效的 RadiossCase。
3. 若脚本要进行网格部件属性或材料绑定，对应属性和材料对象应已存在。
4. 若脚本要启动求解，应先确认工作目录有效，且存在当前解步或可用的 RAD 配置文件。

### 典型接入步骤

1. 使用 Files 打开工程或导入 RAD 数据。
2. 使用 Geometrys 创建基础几何，或按名称获取已有几何对象。
3. 使用 MeshGenerate 为几何或指定拓扑生成网格。
4. 使用 Meshs、ModelSet、ModelSurface 对网格与模型对象做后处理绑定。
5. 使用 Solver 启动求解任务。

### 返回值约定

- Geometrys、Meshs、MeshGenerate、Solver 的大多数脚本接口返回 QString，用于描述成功或失败结果。
- Files 的导入、导出、打开、保存接口为动作型静态入口，不返回状态字符串。
- ModelSet 和 ModelSurface 的部分接口为 void，失败时更多依赖内部消息输出而非结构化返回值。

## 使用示例

下面示例展示脚本侧创建几何、生成网格并启动求解。示例中的类名和方法名与当前注册和包装代码保持一致。

```python
# 创建一个长方体几何
geo = Geometrys()
print(geo.createBox(0.0, 0.0, 0.0, 100.0, 50.0, 20.0))
geo_name = geo.getName()

# 基于几何体生成 3D 网格
mesher = MeshGenerate()
print(mesher.addVolume(geo_name, []))
print(mesher.setMeshSize(2.0, 5.0))
print(mesher.generate(3))

# 启动当前求解步
solver = Solver()
print(solver.setThreadNum(4))
print(solver.startRun())
```

如果只需要执行完整几何的静态网格生成，也可以直接调用：

```python
print(MeshGenerate.static_MeshGenerate_GenerateMesh(geo_name, 3))
```

文件级操作通常用于脚本流程的开始或结束阶段：

```python
Files.static_Files_OpenProject(r"D:/cases/demo.fitk")
Files.static_Files_SaveProject(r"D:/cases/demo_saved.fitk")
```

## 注意事项与限制

- Geometrys.createBox、createSphere、createCylinder 当前内部使用 Box-1、Sphere-1、Cylinder-1 作为基础名，再由管理器自动去重；接口本身不直接接受自定义名称参数。
- Meshs 只能访问已有网格部件，不负责创建网格部件；若名称不存在，对象包装后后续设置会失败。
- ModelSurface 当前只支持按名称获取和重命名，不提供创建模型面的入口。
- ModelSet 的创建接口不返回结构化状态字符串，脚本侧若需要稳健校验，应结合后续名称查询或界面结果确认。
- Solver.stopSolve 当前实现没有实际停止求解器的操作链路，仅保留了接口壳；脚本侧不应假定它已经具备完整停止能力。
- MeshGenerate 强依赖 GmshExec 相关驱动与配置；若外部网格驱动不可用，网格生成会失败。
- Files 接口通过 ActionOperator 和线程池执行动作，更适合在已完成应用初始化的宿主环境中使用，而不是脱离 AppRadioss 的独立脚本环境。

## 最后更新时间

最后更新时间：2026-06-15