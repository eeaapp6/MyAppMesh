# FITKGmshExeDriver - Gmsh执行程序网格生成驱动组件

## 简介

FITKGmshExeDriver 是 FITK_Component 组件层中专门负责调用外部 Gmsh 执行程序进行网格生成的模块。Gmsh 是一款功能强大的开源三维有限元网格生成器，支持从几何模型直接生成高质量的非结构化网格。

组件核心价值：
- 封装与 Gmsh 执行程序的交互逻辑（几何导出、脚本生成、进程管理、结果解析）
- 用户只需配置网格划分参数，组件自动完成与 Gmsh 的所有交互
- 提供 GUI 界面支持图形化配置全局网格尺寸、局部加密策略、算法参数等

## 模块定位

FITKGmshExeDriver 位于 `FITK_Component` 下，主要职责：

- 定义 Gmsh 网格生成的组件接口
- 将 FITK 的网格划分配置转换为 Gmsh Geo 脚本格式
- 管理 Gmsh 执行程序的进程生命周期
- 解析 Gmsh 生成的 MSH 文件，将网格数据导入 FITK 系统
- 支持多种局部网格加密策略（盒子域、圆柱域、球形域）

架构层级：位于 FITK_Interface 接口层之下，直接依赖 FITKAppFramework 和 FITKInterfaceMeshGen 框架，同时依赖外部的 Gmsh 执行程序（gmsh.exe）。

## 架构与目录

```
FITKGmshExeDriver
├── 组件接口层
│   ├── FITKMeshGenGmshExecInterface     # 主接口类
│   └── FITKGmshExeDriverAPI             # 导出宏定义
├── 网格生成核心
│   ├── FITKMesherDriverGmshExec         # Gmsh网格驱动器
│   ├── FITKMeshGenerateProcessorGmshExec # 网格数据处理器
│   ├── FITKMeshSizeGeneratorGmshExec    # 网格尺寸生成器
│   └── FITKMeshAlgorithmGeneratorGmshExec# 网格算法生成器
├── 脚本生成与执行
│   ├── FITKGmshGeoScriptWrite           # Geo脚本写出器
│   ├── FITKGmshGeoScriptManuallyWrite   # 手动脚本写出器
│   ├── FITKGmshExecProgramDriver        # 执行程序驱动
│   └── FITKGmshExecProgramInputInfo     # 执行程序输入信息
├── GUI界面
│   ├── GUIGmshSettings                  # Gmsh网格设置对话框
│   ├── GUIRegionMeshInfoDialog          # 局部区域网格信息对话框
│   ├── GUIRegionMeshSizeBoxDialog       # 盒子域网格尺寸对话框
│   ├── GUIRegionMeshSizeCylinderDialog  # 圆柱域网格尺寸对话框
│   └── GUIRegionMeshSizeSphereDialog     # 球形域网格尺寸对话框
└── 构建配置
    ├── CMakeLists.txt
    ├── FITKGmshExeDriver.pro
    └── FITKGmshExeDriver.pri
```

## 依赖关系

### 编译依赖

- Qt5::Core、Qt5::Gui、Qt5::Widgets
- FITKAppFramework（FITKComponentInterface、FITKExecProgramDriver）
- FITKCore（FITKDirFileTools、FITKLibInfo）
- FITKInterfaceMeshGen（网格生成接口）
- FITKInterfaceModel（网格数据模型）
- FITKInterfaceIO（抽象IO接口）
- FITKInterfaceGeometry（几何接口）
- FITKGmshMshIO（MSH文件读写）
- FITKWidget（Qt界面组件）

### 外部依赖

- **Gmsh执行程序**：组件依赖外部的 gmsh.exe，初始化时会自动部署到应用程序目录下。

## 主要类与功能

### FITKMeshGenGmshExecInterface

组件主接口类，继承自 `FITKComponentInterface`，是网格生成系统的入口点。

初始化流程：
1. 创建 gmsh 目录并部署 gmsh.exe 和 License.txt
2. 获取 `FITKMeshGenInterface` 单例实例
3. 注册网格尺寸生成器、驱动器、处理器和算法生成器

### FITKMesherDriverGmshExec

Gmsh 网格驱动器，继承自 `FITKAbstractMesherDriver`。核心引擎，负责启动和管理 Gmsh 执行进程。

主要方法：
- `startMesher()` - 启动网格生成
- `stopMesher()` - 终止网格生成
- `gmshMeshGenSetting()` - 弹出网格设置窗口

### FITKMeshGenerateProcessorGmshExec

网格数据处理器，继承自 `FITKAbstractMeshProcessor`。将 Gmsh 输出的 MSH 文件转换为 FITK 的网格数据结构。

### FITKMeshSizeGeneratorGmshExec / FITKMeshAlgorithmGeneratorGmshExec

网格尺寸生成器和网格算法生成器，负责生成 Gmsh 特有的网格尺寸和算法配置信息。

### 脚本生成与执行

| 类名 | 功能 |
|------|------|
| FITKGmshGeoScriptWrite | 将 FITK 配置转换为 Geo 脚本格式 |
| FITKGmshGeoScriptManuallyWrite | 手动编写 Geo 脚本 |
| FITKGmshExecProgramDriver | 启动和管理 Gmsh 执行进程 |
| FITKGmshExecProgramInputInfo | 定义传递给 Gmsh 的参数 |

### GUI界面

| 类名 | 功能 |
|------|------|
| GUIGmshSettings | Gmsh 网格设置对话框 |
| GUIRegionMeshInfoDialog | 局部区域网格信息管理 |
| GUIRegionMeshSizeBoxDialog | 盒子域网格尺寸配置 |
| GUIRegionMeshSizeCylinderDialog | 圆柱域网格尺寸配置 |
| GUIRegionMeshSizeSphereDialog | 球形域网格尺寸配置 |

## 构建与集成

### CMake

```cmake
project(FITKGmshExeDriver VERSION 2.0.0 LANGUAGES CXX)
add_library(${PROJECT_NAME} SHARED)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
link_fitk_library(${PROJECT_NAME} ...)
link_fitk_library(${PROJECT_NAME} FITKGmshMshIO)
```

### qmake

同时保留 `.pro` 和 `.pri` 文件，兼容现有工程组织方式。

## 使用示例

### 1. 注册组件

```cpp
#include "ComponentFactory.h"
#include "FITK_Component/FITKGmshExeDriver/FITKMeshGenGmshExecInterface.h"

QList<AppFrame::FITKComponentInterface *> ComponentFactory::createComponents()
{
    QList<AppFrame::FITKComponentInterface *> componentList;
    componentList << new Gmsh::FITKMeshGenGmshExecInterface;
    return componentList;
}
```

### 2. 配置网格生成参数

```cpp
Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
Interface::FITKGlobalMeshSizeInfo* sizeInfo = mf->getGlobalMeshSizeInfo("GmshExec");
Interface::FITKGlobalMeshGenerateAlgorithmInfo* algorithmInfo = 
    mf->getGlobalMeshGenerateAlgorithmInfo("GmshExec");

sizeInfo->setMaxSize(0.5);
sizeInfo->setMinSize(0.1);
algorithmInfo->setMeshGenerateDimension(3);
```

### 3. 启动网格生成（4种驱动方式）

```cpp
Interface::FITKAbstractMesherDriver* mesher = 
    meshGenInterface->getMesherDriver("GmshExec");

// 方式0：自动生成脚本（模型文件 + 网格文件）
mesher->setValue("Method", 0);
mesher->setValue("MeshFile", "D:/mesh.msh");
mesher->setValue("ShapeFile", "D:/geometry.brep");

// 方式1：自动调用组件内部配置窗口
mesher->setValue("Method", 1);
mesher->setValue("MeshFile", "D:/mesh.msh");

// 方式2：外部导入模型和脚本
mesher->setValue("Method", 2);
mesher->setValue("MeshFile", "D:/mesh.msh");
mesher->setValue("ShapeFile", "D:/geometry.brep");
mesher->setValue("ScriptFile", "D:/script.geo");

// 方式3：多模型并行
mesher->setValue("Method", 3);
mesher->setValue("ProcessNum", 2);
mesher->setValue("MeshFiles", meshFiles);
mesher->setValue("ShapeFiles", shapeFiles);

// 启动
connect(mesher, &Interface::FITKAbstractMesherDriver::mesherFinished, 
        this, &MyClass::onFinished);
mesher->startMesher();
```

### 4. 处理生成的网格数据

```cpp
Interface::FITKAbstractMeshProcessor* processor = 
    meshGenInterface->getMeshProcessor("GmshExec");
//获取网格数据
Interface::FITKUnstructuredMesh* mesh = new Interface::FITKUnstructuredMesh;
processor->setValue("File", "D:/mesh.msh");
processor->insertDataObject("Mesh", mesh);
processor->insertDataObject("ComponentManager", mesh->getComponentManager());
processor->setValue("FilterDim", QList<QVariant>() << 0 << 1);
processor->start(QStringList() << "MSH");
```

### 5. 配置局部网格加密

```cpp
Interface::FITKAbstractRegionMeshSize* boxSize = 
    sizeInfo->createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionType::Box);
boxSize->setMinPoint(-1, -1, -1);
boxSize->setMaxPoint(1, 1, 1);
boxSize->setMeshSize(0.1);

Interface::FITKAbstractRegionMeshSize* cylinderSize = 
    sizeInfo->createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionType::Cylinder);
cylinderSize->setCenter(0.0, 0.0, 0.0);
cylinderSize->setRadius(0.2);
cylinderSize->setMeshSize(0.1);

Interface::FITKAbstractRegionMeshSize* sphereSize = 
    sizeInfo->createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionType::Sphere);
sphereSize->setCenter(0.0, 0.0, 0.0);
sphereSize->setRadius(0.3);
sphereSize->setMeshSize(0.02);
```

## 支持的网格类型

### 单元类型

| 维度 | 单元类型 | 节点数 | 说明 |
|------|----------|--------|------|
| 0D | Vertex | 1 | 顶点单元 |
| 1D | Line2 | 2 | 两节点线单元 |
| 1D | Line3 | 3 | 三节点线单元（二次） |
| 2D | Tri3 | 3 | 三节点三角形单元 |
| 2D | Tri6 | 6 | 六节点三角形单元（二次） |
| 2D | Quad4 | 4 | 四节点四边形单元 |
| 2D | Quad8 | 8 | 八节点四边形单元（二次） |
| 3D | Tet4 | 4 | 四节点四面体单元 |
| 3D | Tet10 | 10 | 十节点四面体单元（二次） |
| 3D | Hex8 | 8 | 八节点六面体单元 |
| 3D | Hex20 | 20 | 二十节点六面体单元（二次） |
| 3D | Wedge6 | 6 | 六节点棱柱单元 |
| 3D | Wedge15 | 15 | 十五节点棱柱单元（二次） |

### 支持的几何格式

- **输入格式**：STEP（.stp、.step）、BREP（.brep）、IGES（.iges、.igs）
- **输出格式**：MSH（.msh，包括版本2和版本4格式）

### 网格算法

- **Delaunay**：Delaunay 三角化算法
- **Frontal**：波前法
- **MMG**：多尺度网格生成
- **HXT**：高性能三角化算法

## 使用建议

- 使用前确保 gmsh.exe 已正确部署
- 大型复杂模型建议先在小范围测试网格参数
- 合理使用局部网格加密：在需要精细分析的区域使用局部加密
- 调整网格尺寸时注意最大/最小尺寸比例，避免过大的增长率
- 声学分析等需要高质量网格的场景建议开启网格优化

## 注意事项

- Gmsh 执行程序是外部依赖，组件初始化时会自动部署
- 网格生成可能耗时较长，建议在后台线程中执行
- Geo 脚本生成依赖于几何模型，确保几何模型无拓扑错误
- 线程安全：不应在多个线程中同时操作同一个驱动器实例
- 网格生成失败时检查 Gmsh 错误输出，常见问题包括几何拓扑错误、网格尺寸不合理等

最后更新时间：2026-04-30