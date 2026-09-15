# FITKInterfaceCFDPost - CFD 后处理接口与数据对象模块

## 项目简介
FITKInterfaceCFDPost 是 FITK 体系中面向 CFD 后处理场景的接口模块，负责统一封装结果导入对象、派生后处理对象以及显示辅助数据。
该模块位于 FITK_Interface 层，依赖 FITKCore、FITKAppFramework、FITKVTKAlgorithm、VTK 与 CGNS。
它的核心职责不是实现单个算法，而是把“一个后处理结果”组织成可管理、可查询、可显示协作的数据对象体系。

模块提供的主要能力包括：
- 统一描述稳态、非稳态和派生后处理对象。
- 提供场量、范围、极值、网格类型、颜色映射等公共查询接口。
- 封装等值面、裁剪、切片、流线、阈值、变形、探针、标尺等派生结果。
- 提供灯光和色库等显示辅助资源的管理入口。
- 集成 CGNS、PLT、DAT 等读取路径，并向上层暴露统一对象接口。

## 架构与目录
```text
FITKInterfaceCFDPost/
├── 核心抽象与管理
│   ├── FITKAbstractCFDPostData.h
│   ├── FITKCFDPostData.h
│   ├── FITKCFDPost3DManager.h
│   ├── FITKCFDPostEnum.h
│   └── FITKInterfaceCFDPostAPI.h
├── 数据导入与读取
│   ├── FITKCFDPostImport.h
│   ├── FITKCFDPostSteady.h
│   ├── FITKCFDPostUnSteady.h
│   ├── FITKAlgCGNSReader.h
│   ├── FITKAlgPltReader.h
│   ├── FITKAlgDatReader.h
│   └── FITKPltReader.h
├── 派生后处理对象
│   ├── FITKCFDPostIsosurface.h
│   ├── FITKCFDPostClipPlane.h
│   ├── FITKCFDPostSlicePlane.h
│   ├── FITKCFDPostStreamLine.h
│   ├── FITKCFDPostThreshold.h
│   ├── FITKCFDPostDeform.h
│   ├── FITKCFDPostProbe.h
│   └── FITKCFDPostRuler.h
├── 显示辅助对象
│   ├── FITKCFDPostLight.h
│   ├── FITKCFDPostLightManager.h
│   ├── FITKCFDPostColorLibaryData.h
│   └── FITKCFDPostColorLibaryDataManager.h
├── 底层 CGNS/VTK 适配
│   ├── vtkCGNSReader.h
│   ├── vtkCGNSFileSeriesReader.h
│   ├── vtkCGNSReaderInternal.h
│   ├── vtkCGNSCache.h
│   └── vtkFileSeriesHelper.h
└── CMakeLists.txt
```
从职责划分看，模块可以分为五层：核心抽象与统一接口、文件导入与读取适配、派生后处理对象、显示辅助资源管理、底层 VTK 与 CGNS 读取支撑。

## 模块定位
FITKInterfaceCFDPost 在当前仓库中可以理解为“后处理数据对象层”。
- 它不是最终界面层，但会被界面对话框、对象树和属性面板直接驱动。
- 它不是通用 VTK 算法层，算法实现更多依赖 FITKVTKAlgorithm 和 VTK 自身能力。
- 它负责把导入结果和派生结果包装成统一对象，并维护对象查询、更新和父子关系。
典型上层使用方式是先获取 FITKCFDPostData，再从中取得结果管理器、灯光管理器和色库管理器，继续完成导入、编辑、删除和界面同步。

## 核心功能
### 1. 核心抽象与统一接口
#### FITKAbstractCFDPostData
FITKAbstractCFDPostData 是本模块的核心抽象基类，继承自 FITKAbstractNDataObject 与 FITKVarientParams，用于统一描述一个 CFD 后处理对象。
它向上层提供一套稳定接口：
- 对象类型接口，如 getPostDataType()。
- 输出接口，如 getOutput() 与 getOutputPort()。
- 父对象关系接口，如 getCFDPostParentID()。
- 场量查询接口，如点场、单元场、变量类型和变量分量数。
- 统计与显示协作接口，如范围、中心、极值、mapper、交互器和 scalar bar。
这意味着导入对象和派生对象可以被同一套属性面板、对象树和渲染协作逻辑处理。

#### FITKCFDPostData
FITKCFDPostData 是模块级总入口，负责聚合：
- FITKCFDPost3DManager
- FITKCFDPostLightManager
- FITKCFDPostColorLibaryDataManager
应用层通常不直接分别持有多个管理器，而是以 FITKCFDPostData 作为统一访问入口。

#### FITKCFDPost3DManager
FITKCFDPost3DManager 负责统一管理 FITKAbstractCFDPostData 及其派生对象。
在当前调用链中，稳态结果、非稳态结果、流线、阈值、切片、裁剪和等值面等对象都会被追加到该管理器，由其维护对象集合和生命周期关系。

### 2. 数据导入与读取
#### FITKCFDPostImport
FITKCFDPostImport 是导入类抽象基类，在通用后处理对象接口之上补充了 setFile(QString& fileName)，用于统一导入对象的文件设置入口。

#### FITKCFDPostSteady
FITKCFDPostSteady 表示单文件稳态结果对象，特点是：
- 构造时接收文件类型字符串。
- 通过 setFile() 绑定结果文件。
- 通过 getOutput() 和 getOutputPort() 暴露读取结果。

#### FITKCFDPostUnSteady
FITKCFDPostUnSteady 表示文件组或时序形式的非稳态结果对象，用于承载多时刻数据入口。

#### 读取适配类
| 类名 | 职责 |
|------|------|
| FITKAlgCGNSReader | CGNS 数据读取适配 |
| FITKAlgPltReader | PLT 数据读取适配 |
| FITKAlgDatReader | DAT 数据读取适配 |
| FITKPltReader | PLT 读取辅助 |
| vtkCGNSReader | 基于 VTK 的 CGNS 读取器 |
| vtkCGNSFileSeriesReader | CGNS 文件序列读取 |
这些类共同承担“把外部 CFD 结果文件转换为可进入对象体系的数据输出”的职责。

### 3. 派生后处理对象
#### 等值面、阈值、裁剪与切片
模块内的派生对象围绕已有父对象输出继续加工，主要包括：
- FITKCFDPostIsosurface：按场量提取等值面。
- FITKCFDPostThreshold：按阈值范围筛选区域。
- FITKCFDPostClipPlane、Sphere、Cylinder、Box：裁剪结果。
- FITKCFDPostSlicePlane、Sphere、Cylinder、Box：切片结果。

#### FITKCFDPostStreamLine
FITKCFDPostStreamLine 是流线对象，封装了：
- 线源 vtkLineSource。
- 流线跟踪器 vtkStreamTracer。
- 矢量名称和矢量类型设置。
- 积分方向设置。
- 起终点、分辨率和跟踪长度设置。
当前仓库中的流线对话框会创建该对象，再通过 setLine()、setVector()、setDirection() 和 setLength() 写回参数。

#### 其他派生对象
| 类名 | 职责 |
|------|------|
| FITKCFDPostGlyph | 生成字形化结果 |
| FITKAlgGlyph | Glyph 算法辅助 |
| FITKCFDPostDeform | 变形结果对象 |
| FITKCFDPostProbe | 探针结果对象 |
| FITKCFDPostRuler | 标尺结果对象 |

### 4. 显示辅助与资源管理
模块同时维护一组供多个后处理对象共享的显示资源：
- FITKCFDPostLight 与 FITKCFDPostLightManager：管理场景灯光数据。
- FITKCFDPostColorLibaryData 与 FITKCFDPostColorLibaryDataManager：管理色库配置数据。
这些资源不直接承担算法功能，但会被上层渲染和属性编辑流程持续访问。

### 5. 枚举与基础数据结构
FITKCFDPostEnum.h 定义了模块内的重要基础枚举：
- FITKPostDataType：对象类型。
- FITKPostVariableType：变量类型。
- FITKPostFieldType：点场或单元场。
- FITKPostGridType：结构化或非结构化网格。
- FITKPostStreamDirection：流线积分方向。
- FITKPostLightType：灯光类型。
这些枚举构成了 GUI、Operators 和后处理对象层之间的基本交互约定。

## 关键类
### 核心入口
| 类名 | 说明 |
|------|------|
| FITKCFDPostData | 模块级入口，聚合结果、灯光和色库管理器 |
| FITKCFDPost3DManager | 管理所有 CFD 后处理对象 |
| FITKAbstractCFDPostData | 统一的后处理对象抽象基类 |

### 导入对象
| 类名 | 说明 |
|------|------|
| FITKCFDPostImport | 导入类抽象基类，统一 setFile() 接口 |
| FITKCFDPostSteady | 单文件稳态结果对象 |
| FITKCFDPostUnSteady | 文件组或时序结果对象 |

### 派生对象
| 类名 | 说明 |
|------|------|
| FITKCFDPostIsosurface | 等值面结果 |
| FITKCFDPostClipPlane / Sphere / Cylinder / Box | 裁剪结果 |
| FITKCFDPostSlicePlane / Sphere / Cylinder / Box | 切片结果 |
| FITKCFDPostStreamLine | 流线结果 |
| FITKCFDPostGlyph | 字形结果 |
| FITKCFDPostThreshold | 阈值过滤结果 |
| FITKCFDPostDeform | 变形结果 |
| FITKCFDPostProbe | 探针结果 |
| FITKCFDPostRuler | 标尺结果 |

### 显示资源
| 类名 | 说明 |
|------|------|
| FITKCFDPostLight | 灯光对象 |
| FITKCFDPostLightManager | 灯光管理器 |
| FITKCFDPostColorLibaryData | 色库数据 |
| FITKCFDPostColorLibaryDataManager | 色库管理器 |

## 典型调用关系
当前仓库中的典型调用流程如下：
1. 应用层通过全局数据对象获取 FITKCFDPostData。
2. 由 FITKCFDPostData 提供 FITKCFDPost3DManager 等管理器。
3. Operators 或对话框创建导入对象或派生对象。
4. 对象被追加到 FITKCFDPost3DManager。
5. GUI 树、属性面板和渲染逻辑通过统一接口读取对象状态和 VTK 输出。

## 使用示例
### 示例1：创建稳态导入对象
```cpp
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostSteady.h"

QString fileType = "cgns";
QString fileName = "case.cgns";

Interface::FITKCFDPostSteady* steady = new Interface::FITKCFDPostSteady(fileType);
steady->setFile(fileName);
steady->setDataObjectName("case.cgns");

vtkDataSet* output = steady->getOutput();
Q_UNUSED(output);
```

### 示例2：配置一个流线对象
```cpp
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostStreamLine.h"

int parentID = 1001;
Interface::FITKCFDPostStreamLine* streamLine = new Interface::FITKCFDPostStreamLine(parentID);
streamLine->setDataObjectName("stream-line-1");

double startPoint[3] = {0.0, 0.0, 0.0};
double endPoint[3] = {1.0, 0.0, 0.0};

streamLine->setLine(startPoint, endPoint, 50);
streamLine->setVector(Interface::FITKPostFieldType::Post_Point, "Velocity");
streamLine->setDirection(Interface::FITKPostStreamDirection::Post_Forward);
streamLine->setLength(10.0);
```

## 依赖关系
### 内部依赖
- FITKCore：提供抽象数据对象、变参能力和基础宏。
- FITKAppFramework：提供应用层全局数据与运行框架支撑。
- FITKVTKAlgorithm：提供 VTK 算法和可视化辅助封装。

### 外部依赖
- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- VTK::VTK
- CGNS::CGNS

### 构建属性
- CMake 中将本模块定义为共享库 FITKInterfaceCFDPost。
- Debug 输出目录为 output/bin_d。
- Release 输出目录为 output/bin。
- 模块通过链接 FITKCore、FITKAppFramework 和 FITKVTKAlgorithm 完成内部接入。

## 构建与集成方式
本模块不是独立应用程序，而是作为 FITK_Interface 层共享库参与上层工程构建。
在当前仓库中：
1. FITK_Interface 顶层 CMake 通过 add_subdirectory(FITKInterfaceCFDPost) 引入该模块。
2. 本模块在自身 CMakeLists.txt 中声明头源文件、Qt 自动生成配置和链接依赖。
3. 构建完成后，库文件输出到 output/bin_d 或 output/bin，供应用层和其他模块链接。

若在新的上层模块中使用本库，至少需要：
- 包含 FITK_Interface/FITKInterfaceCFDPost 下的相关头文件。
- 确保运行时可找到 FITKCore、FITKAppFramework、FITKVTKAlgorithm、VTK 与 CGNS 相关库。
- 沿用当前对象管理方式，通过 FITKCFDPostData 或 FITKCFDPost3DManager 组织后处理对象。

## 使用方法
### 典型接入步骤
1. 准备 FITKCFDPostData 作为模块数据入口。
2. 获取 FITKCFDPost3DManager 或其他辅助管理器。
3. 根据场景创建导入对象或派生对象。
4. 调用对象参数接口写入文件、向量、裁剪面、切片参数或显示参数。
5. 将对象加入管理器，并在上层界面逻辑中根据对象 ID 同步树节点、属性面板和渲染状态。

### 适用场景
- 导入稳态 CFD 结果并展示场量。
- 导入文件组形式的非稳态结果。
- 基于已有结果生成等值面、裁剪、切片和流线。
- 管理灯光与色库等渲染辅助数据。

## 注意事项
- 本模块大量对象围绕 VTK 输出协作，实际渲染效果依赖上层渲染窗口组件与 mapper、actor 的连接方式。
- FITKCFDPost3DManager 的具体容器能力来自上层模板基类，使用时应保持当前仓库既有的数据对象管理模式。
- 非稳态文件组的时间步控制细节不在本 README 展开，当前文档只描述已从代码与调用链确认的对象组织方式。
- 灯光和色库对象在当前仓库中主要作为共享显示资源使用，而不是独立渲染框架。
- 当前说明基于 Windows 工作区源码和 CMake 配置整理，未额外验证 Linux 环境行为。

## 最后更新时间
最后更新时间：2026-04-23