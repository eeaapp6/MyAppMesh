# FITKInterfaceStructuralPost - 结构后处理数据接口模块

## 项目简介

FITKInterfaceStructuralPost 是 FITK 体系中的结构后处理接口层模块。

该模块基于 VTK 管线组织结构结果可视化与后处理数据，支持静态/动态结果读取、变量提取、变形显示、切片裁切、阈值过滤、等值面处理和后处理对象分组管理；并支持 HDF5/VTK 等多文件类型数据接入。

该模块为结构分析后处理应用提供统一的数据抽象层，使得上层渲染组件无需关心具体后处理文件格式的解析细节。

## 架构与目录

```
FITKInterfaceStructuralPost/
├── 后处理基类
│   ├── FITKAbstractStruPostVTK.h         # 结构后处理抽象基类
│   └── FITKInterfaceStructuralPostAPI.h   # API宏定义
├── 静态后处理
│   └── FITKStaticStruPostVTK.h          # 静态结构后处理对象
├── 动态后处理
│   └── FITKDynamicStruPostVTK.h          # 动态结构后处理对象
├── 裁切与切片
│   ├── FITKCutViewManager.h             # 裁切/切片管理器
│   └── FITKAbstractViewCut.h            # 裁切/切片抽象基类
├── 阈值与等值面
│   ├── FITKStruThresholdPostData.h      # 阈值后处理数据
│   └── FITKStruIsosurfPostData.h         # 等值面后处理数据
├── 管理器
│   └── FITKStruPost3DManager.h          # 后处理对象管理器
├── 显示与辅助
│   ├── FITKStruPostDisplayGroup.h       # 显示分组
│   └── FITKStruPostSetting.h            # 后处理设置
├── 数据读取
│   ├── FITKIBEStrDataSetReader.h        # IBE格式数据读取器
│   └── FITKMeshFeatureVTK.h             # VTK网格特征
├── 消息与阈值
│   ├── FITKStruPostMessageText.h        # 后处理消息文本
│   └── FITKStruThresholdPostData.h     # 阈值过滤数据
└── CMakeLists.txt
```

## 核心功能

### 1. 后处理数据模型

#### FITKAbstractStructuralPostVTK

结构后处理抽象基类，定义所有后处理对象的基本接口：

- **文件类型**：VTK、VTP、VTU、IBE 等格式支持
- **后处理类型**：静态（SPTStatic）、动态（SPTDynamicExplict）

#### FITKStaticStruPostVTK

静态结构后处理对象，用于加载和显示单帧结果数据：

- 单文件加载模式
- 支持变形显示配置
- 变量范围查询

#### FITKDynamicStruPostVTK

动态结构后处理对象，用于加载和显示多帧时程结果数据：

- 多文件模式支持
- 帧切换控制（首帧、末帧、上一帧、下一帧）
- 时间步管理

### 2. 结果读取与帧控制

#### 变量类型

- **VariableLocation**：变量位置枚举
  - `VALOCNode`：节点位置变量
  - `VALOCEle`：单元位置变量

#### 文件类型

- `SPFTNone`：空类型
- `SPFTVTK`：VTK 格式
- `SPFTVTP`：VTP 格式
- `SPFTVTU`：VTU 格式
- `SPFTIBE`：IBE 格式

#### 帧控制方法

- `firstFrame()`：跳转到首帧
- `lastFrame()`：跳转到末帧
- `nextFrame()`：前进到下一帧
- `previousFrame()`：后退到上一帧
- `setFrame(index)`：跳转到指定帧

### 3. 变量与变形显示

#### 变量获取

- 支持按节点/单元位置获取变量
- 支持获取变量分量（如 S11、S22、S12 等应力分量）
- 支持变量范围查询（最小值、最大值）

#### 变形显示

- `enableDeform(bool)`：启用/禁用变形显示
- `setDeformFactor(double)`：设置变形系数
- `setUndeformedFactor(double)`：设置不变形系数

### 4. 裁切与切片

#### FITKAbstractViewCut

裁切/切片数据抽象基类，支持多种裁切形状：

- **CutShapeType**：裁切器形状
  - `Plane`：平面裁切
  - `Cylinder`：圆柱裁切
  - `Sphere`：球形裁切

- **DisplayModelType**：显示模式
  - `Clip`：裁切模式
  - `Slice`：切片模式
  - `RevClip`：旋转裁切模式

#### FITKCutViewManager

裁切/切片管理器：

- 管理多个裁切/切片对象
- 提供裁切面的创建、编辑、删除功能
- 支持裁切结果的实时更新

### 5. 阈值与等值面

#### FITKStruThresholdPostData

阈值过滤后处理数据：

- 支持设置变量类型（节点/单元）
- 支持设置阈值范围（最小值、最大值）
- 支持数据过滤与提取

#### FITKStruIsosurfPostData

等值面后处理数据：

- 支持设置等值面值
- 支持多等值面值同时显示
- 支持等值面颜色映射

### 6. 后处理数据管理

#### FITKStruPost3DManager

后处理对象管理器（继承自 FITKAbstractDataManager）：

- **getDataByID()**：根据 ID 获取后处理数据
- **getDataIndependence()**：获取独立于前处理模型的后处理数据
- 支持多后处理数据同时管理

## 关键类

### 后处理基类

| 类名 | 说明 |
|------|------|
| FITKAbstractStructuralPostVTK | 结构后处理抽象基类 |
| FITKStaticStruPostVTK | 静态结构后处理对象 |
| FITKDynamicStruPostVTK | 动态结构后处理对象 |

### 裁切与切片

| 类名 | 说明 |
|------|------|
| FITKCutViewManager | 裁切/切片管理器 |
| FITKAbstractViewCut | 裁切/切片抽象基类 |

### 过滤器

| 类名 | 说明 |
|------|------|
| FITKStruThresholdPostData | 阈值后处理数据 |
| FITKStruIsosurfPostData | 等值面后处理数据 |

### 管理器

| 类名 | 说明 |
|------|------|
| FITKStruPost3DManager | 后处理对象管理器 |
| FITKStruPostSetting | 后处理设置 |

## 使用示例

### 示例1：创建静态后处理并加载结果

```cpp
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStaticStruPostVTK.h"

void loadStaticPostData(const QString& resultFile)
{
    auto* post = new Interface::FITKStaticStruPostVTK(Interface::StruPostFileType::SPFTVTK);
    post->setFile(resultFile);
    post->enableDeform(true);
    post->setDeformFactor(1.0);
}
```

### 示例2：创建动态后处理并切换帧

```cpp
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKDynamicStruPostVTK.h"

void loadDynamicPostData(const QStringList& files)
{
    auto* post = new Interface::FITKDynamicStruPostVTK(Interface::StruPostFileType::SPFTVTK);
    post->setFiles(files);
    post->firstFrame();
    post->nextFrame();
}
```

### 示例3：应用阈值后处理

```cpp
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStruThresholdPostData.h"

void applyThreshold(vtkDataSet* input)
{
    auto* threshold = new Interface::FITKStruThresholdPostData();
    threshold->setFileName("S");
    threshold->setType(Interface::VariableLocation::VALOCNode);
    threshold->setThresholdRange(10.0, 200.0);
    threshold->setInput(input);
    threshold->update();
}
```

### 示例4：使用后处理对象管理器

```cpp
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStruPost3DManager.h"

void managePostData(Interface::FITKStruPost3DManager* manager)
{
    if (manager == nullptr) return;

    // 获取所有后处理数据
    auto allData = manager->getDataAll();

    // 获取独立后处理数据
    auto indepData = manager->getDataIndependence();

    // 根据ID获取指定数据
    auto data = manager->getDataByID(1);
}
```

## 依赖关系

### 外部依赖
- Qt5：Core
- VTK

### 内部依赖
- FITK_Kernel/FITKCore
- FITK_Interface/FITKVTKAlgorithm

### HDF5 可选依赖
- HDF5（当使用 HDF5 格式数据时）

## 注意事项

1. 后处理对象的创建应通过具体子类（如 FITKStaticStruPostVTK），而非直接实例化抽象基类
2. 动态后处理对象在帧切换时会触发数据重新加载，需要注意性能影响
3. 阈值和等值面后处理使用 VTK 管线，需要在主线程中执行 Update 操作
4. 后处理管理器支持独立后处理数据的查询，便于显示组管理

---

最后更新时间：2026-04-21