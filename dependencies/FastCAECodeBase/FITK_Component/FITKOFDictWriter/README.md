# FITKOFDictWriter - OpenFOAM 字典文件写出组件

## 简介

FITKOFDictWriter 是 FastCAE 集成工具包（FITK）中负责 OpenFOAM 字典文件生成与写出的功能组件。它提供了一套完整的字典数据模型和文件写出机制，支持生成 OpenFOAM 仿真所需的各类字典文件，包括网格生成、求解控制、物理属性等配置文件。

该模块解决的核心问题是：如何将用户在图形界面中配置的仿真参数转换为符合 OpenFOAM 格式规范的字典文件，确保参数的正确传递和格式一致性。

## 模块定位

FITKOFDictWriter 位于 `FITK_Component` 下，属于功能组件层，主要承担以下职责：

- 提供统一的字典数据模型，支持多种数据类型（bool、int、double、string、vector、array、group）
- 封装各类 OpenFOAM 字典文件的写出逻辑
- 确保生成的字典文件符合 OpenFOAM 格式规范
- 作为流体仿真流程中预处理阶段的核心支撑模块

## 架构与目录

从当前目录中的头文件和源文件看，FITKOFDictWriter 可以概括为以下几组能力：

```text
FITKOFDictWriter
├── 核心接口与数据模型
│   ├── FITKOFDictWriterAPI          # API声明宏
│   ├── FITKAbstractDict             # 字典抽象基类
│   ├── FITKDictObject               # 字典工程（顶层容器）
│   ├── FITKDictValue                # 字典数据适配器
│   ├── FITKDictGroup                # 字典组件（name { ... }）
│   ├── FITKDictVector               # 字典向量（vector）
│   └── FITKDictArray                # 字典数组（(...)）
├── 字典写出器基类
│   └── FITKOFAbstractDictWriter     # 字典文件写出基类接口
└── 具体字典写出器
    ├── FITKOFControlDictWriter      # controlDict 写出器
    ├── FITKOFBlockMeshDictWriter    # blockMeshDict 写出器
    ├── FITKOFSnappyHexMeshDictWriter # snappyHexMeshDict 写出器
    ├── FITKOFFVSchemesWriter        # fvSchemes 写出器
    ├── FITKOFFVSolutionWriter       # fvSolution 写出器
    ├── FITKOFFVOptionsWriter        # fvOptions 写出器
    ├── FITKOFMeshBoundaryW          # 网格边界写出器
    ├── FITKOFCreatePatchDictWriter  # createPatchDict 写出器
    ├── FITKOFSetFieldsDictWriter    # setFieldsDict 写出器
    ├── FITKOFChangeDictionaryDictW  # changeDictionaryDict 写出器
    ├── FITKOFTurbulencePropertiesWrite # 湍流属性写出器
    ├── FITKOFTransportPropertiesWrite  # 输运属性写出器
    ├── FITKOFThermoPhyPropDictWriter   # 热物理属性写出器
    ├── FITKOFOperatingConditionsW      # 运行条件写出器
    ├── FITKOFRadiationPropDictWriter   # 辐射属性写出器
    └── FITKOFInitialDirWriter          # 初始目录写出器
```

## 依赖关系

### 编译依赖

根据模块源码中的头文件引用，该模块当前显式依赖：

- Qt5::Core
- FITKCore（基础对象、数据对象）

### 模块层级关系

- FITKOFDictWriter 依赖 `FITKCore`
- 上层流体仿真模块通过该组件生成 OpenFOAM 所需的各类字典文件
- 该组件与 `FITKMeshGenOF` 配合，完成网格生成前的字典配置工作

## 主要类与功能说明

### 核心数据模型类

#### `FITKAbstractDict`

字典抽象基类，定义了字典数据的基本类型和写出接口。

**字典类型枚举：**
- `DictNone`：空/普通键值对
- `DictBool`：布尔值
- `DictInt`：整数值
- `DictDouble`：浮点数值
- `DictString`：字符串
- `DictStrList`：字符串链表
- `DictVector`：向量
- `DictArray`：数组
- `DictGroup`：字典组件（嵌套结构）
- `DictInclude`：文件引用声明

#### `FITKDictObject`

字典工程类，作为字典数据的顶层容器，管理一组键值对数据。

**主要方法：**
- `count()`：获取字典数据数量
- `value(key)`：通过名称获取字典数据
- `at(index)`：通过下标获取字典数据
- `append(key, value)`：追加字典数据（支持多种类型）
- `insert(index, key, value)`：在指定位置插入字典数据
- `removeAt(index)`：移除指定位置的数据
- `removeKey(key)`：移除指定键的数据
- `writeObjectDict(stream)`：写出字典数据到文件流

#### `FITKDictValue`

字典数据适配器类，封装不同类型的数据值，并提供类型转换功能。

**主要方法：**
- `getDictType()`：获取数据类型
- `setKey(key)` / `getKey()`：设置/获取键名
- `toBool()` / `toDouble()` / `toInt()` / `toString()`：类型转换
- `toArray()` / `toGroup()` / `toVector()`：转换为复合类型

#### `FITKDictGroup`

字典组件类，表示 `name { v1 v2 v3 ... }` 形式的嵌套结构。

**主要方法：**
- 继承自 `FITKAbstractDict`
- 支持嵌套的键值对管理

#### `FITKDictVector`

字典向量类，表示三维向量数据。

#### `FITKDictArray`

字典数组类，表示数组数据 `(v1 v2 v3 ...)`。

### 字典写出器类

#### `FITKOFAbstractDictWriter`

字典文件写出基类接口，定义了字典写出的标准流程。

**主要方法：**
- `setFilePath(filePath)`：设置输出文件路径
- `getFileName()`：获取文件名
- `run()`：执行写出操作

**保护方法：**
- `openDictObject()`：打开字典工程
- `writeDictFile()`：写出字典文件
- `closeDictObject()`：关闭字典工程

#### `FITKOFControlDictWriter`

controlDict 文件写出器，负责生成求解控制字典。

#### `FITKOFBlockMeshDictWriter`

blockMeshDict 文件写出器，负责生成结构化网格字典，支持 Box、Cylinder、Sphere 等区域类型。

#### `FITKOFSnappyHexMeshDictWriter`

snappyHexMeshDict 文件写出器，负责生成自适应网格字典，支持几何网格区域配置。

## 构建与集成

### CMake

当前模块通过 CMake 构建为共享库：

- `add_library(FITKOFDictWriter SHARED)`
- 启用 `AUTOMOC`
- 链接依赖：`FITKCore`

### 在上层工程中的参与方式

FITKOFDictWriter 作为组件层模块，通常通过以下方式接入应用：

1. 创建具体的字典写出器实例
2. 设置输出文件路径
3. 调用 `run()` 方法执行写出操作
4. 配合仿真流程生成所需的字典文件

## 使用示例

### 1. 创建并写出字典对象

```cpp
#include "FITKDictObject.h"
#include "FITKOFDictWriterIO.h"

// 创建字典对象
DICT::FITKDictObject dict;

// 添加键值对
dict.append("FoamFile", "");
dict.append("version", 2.0);
dict.append("format", "ascii");
dict.append("class", "dictionary");
dict.append("location", "system");
dict.append("object", "controlDict");

// 添加嵌套组件
DICT::FITKDictGroup* group = new DICT::FITKDictGroup();
group->append("startTime", 0);
group->append("endTime", 100);
group->append("deltaT", 0.01);
dict.append("timeControl", group);

// 写出到文件
// 实际写出通常通过具体的 Writer 类完成
```

### 2. 使用字典写出器基类

```cpp
#include "FITKOFAbstractDictWriter.h"
#include "FITKOFControlDictWriter.h"

// 创建 controlDict 写出器
IO::FITKOFControlDictWriter* writer = new IO::FITKOFControlDictWriter();

// 设置输出路径
writer->setFilePath("/path/to/case/system/controlDict");

// 执行写出
bool success = writer->run();

if (success) {
    qDebug() << "controlDict written successfully";
}

// 释放资源
delete writer;
```

### 3. 使用字典向量

```cpp
#include "FITKDictVector.h"
#include "FITKDictObject.h"

// 创建字典对象
DICT::FITKDictObject dict;

// 创建向量
DICT::FITKDictVector* vector = new DICT::FITKDictVector();
vector->setValue(1.0, 2.0, 3.0);

// 添加到字典
dict.append("origin", vector);

// 释放资源
delete vector;
```

### 4. 使用字典组件（Group）

```cpp
#include "FITKDictGroup.h"
#include "FITKDictObject.h"

// 创建顶层字典对象
DICT::FITKDictObject dict;

// 创建嵌套组件
DICT::FITKDictGroup* fvSchemes = new DICT::FITKDictGroup();
fvSchemes->append("default", "none");

// 创建子组件
DICT::FITKDictGroup* ddtSchemes = new DICT::FITKDictGroup();
ddtSchemes->append("default", "Euler");
fvSchemes->append("ddtSchemes", ddtSchemes);

// 添加到顶层字典
dict.append("FoamFile", "");
dict.append("version", 2.0);
dict.append("format", "ascii");
dict.append("fvSchemes", fvSchemes);

// 释放资源
delete ddtSchemes;
delete fvSchemes;
```

### 5. 使用 blockMeshDict 写出器

```cpp
#include "FITKOFBlockMeshDictWriter.h"

// 创建 blockMeshDict 写出器
IO::FITKOFBlockMeshDictWriter* writer = new IO::FITKOFBlockMeshDictWriter();

// 设置输出路径
writer->setFilePath("/path/to/case/system/blockMeshDict");

// 执行写出
bool success = writer->run();

if (success) {
    qDebug() << "blockMeshDict written successfully";
}

// 释放资源
delete writer;
```

## 注意事项

- FITKOFDictWriter 生成的字典文件格式需严格符合 OpenFOAM 规范
- 字典数据的键名和值类型需要与 OpenFOAM 要求一致
- 复合类型（Group、Vector、Array）需要正确嵌套
- 写出前需确保输出目录存在且有写入权限
- 不同版本的 OpenFOAM 可能对字典格式有细微差异，需根据目标版本调整

最后更新时间：2026-04-30