# FITKOFDictWriter

## 项目描述

FITKOFDictWriter 是 FITK 框架的 OpenFOAM 字典文件写入组件，专门用于生成和管理 OpenFOAM 的各种字典文件。该组件提供了完整的 OpenFOAM 配置文件生成功能，支持 controlDict、fvSchemes、fvSolution、边界条件等多种字典类型，并提供了灵活的模板系统和参数管理。

## 主要功能

- **字典文件生成**：生成各种 OpenFOAM 字典文件
- **参数配置**：提供 OpenFOAM 求解器参数的配置接口
- **模板管理**：支持字典文件模板的使用和定制
- **验证功能**：验证生成的字典文件格式正确性
- **批量生成**：支持批量生成多个字典文件
- **版本兼容**：支持不同 OpenFOAM 版本的字典格式

## 技术栈

- **编程语言**：C++
- **框架**：Qt
- **构建系统**：qmake
- **外部依赖**：OpenFOAM（格式兼容）

## 项目结构

```
FITKOFDictWriter/
├── 基础字典类
│   ├── FITKAbstractDict.cpp/.h              # 抽象字典基类
│   ├── FITKDictObject.cpp/.h                # 字典对象类
│   ├── FITKDictValue.cpp/.h                 # 字典值类
│   ├── FITKDictGroup.cpp/.h                 # 字典组类
│   ├── FITKDictArray.cpp/.h                 # 字典数组类
│   └── FITKDictVector.cpp/.h                # 字典向量类
├── 写入器基类
│   ├── FITKOFAbstractDictWriter.cpp/.h      # 抽象写入器基类
│   └── FITKOFDictWriterIO.cpp/.h            # 字典文件 IO 类
├── 控制和求解器字典
│   ├── FITKOFControlDictWriter.cpp/.h       # controlDict 写入器
│   ├── FITKOFFVSchemesWriter.cpp/.h         # fvSchemes 写入器
│   ├── FITKOFFVSolutionWriter.cpp/.h        # fvSolution 写入器
│   └── FITKOFFVOptionsWriter.cpp/.h         # fvOptions 写入器
├── 网格生成字典
│   ├── FITKOFBlockMeshDictWriter.cpp/.h     # blockMeshDict 写入器
│   ├── FITKOFSnappyHexMeshDictWriter.cpp/.h # snappyHexMeshDict 写入器
│   ├── FITKOFCreatePatchDictWriter.cpp/.h   # createPatchDict 写入器
│   └── FITKOFMeshBoundaryW.cpp/.h           # 边界条件写入器
├── 物理属性字典
│   ├── FITKOFTransportPropertiesWrite.cpp/.h    # 传输属性写入器
│   ├── FITKOFTurbulencePropertiesWrite.cpp/.h   # 湍流属性写入器
│   ├── FITKOFThermoPhyPropDictWriter.cpp/.h     # 热物理属性写入器
│   ├── FITKOFOperatingConditionsW.cpp/.h         # 操作条件写入器
│   └── FITKOFRadiationPropDictWriter.cpp/.h     # 辐射属性写入器
├── 初始和后处理字典
│   ├── FITKOFInitialDirWriter.cpp/.h        # 初始条件写入器
│   ├── FITKOFSetFieldsDictWriter.cpp/.h     # setFieldsDict 写入器
│   └── FITKOFChangeDictionaryDictW.cpp/.h   # changeDictionaryDict 写入器
├── FITKOFDictWriterAPI.h                    # API 导出宏定义
├── FITKOFDictWriter.pri                     # qmake 项目文件
└── FITKOFDictWriter.pro                     # 主项目文件
```

## 核心类

### 基础字典类
- **FITKAbstractDict**：抽象字典基类，定义字典的基本结构和操作
- **FITKDictObject**：字典对象类，表示字典中的键值对
- **FITKDictValue**：字典值类，支持各种数据类型的值
- **FITKDictGroup**：字典组类，表示嵌套的字典结构
- **FITKDictArray**：字典数组类，表示数组类型的值
- **FITKDictVector**：字典向量类，表示向量类型的值

### 写入器类
- **FITKOFAbstractDictWriter**：抽象写入器基类，定义写入接口
- **FITKOFDictWriterIO**：字典文件 IO 类，处理文件读写操作

## 支持的字典文件类型

### 控制字典
- **controlDict**：求解器控制参数
  ```foam
  application     simpleFoam;
  startFrom       startTime;
  startTime       0;
  stopAt          endTime;
  endTime         1000;
  deltaT          1;
  writeControl    timeStep;
  writeInterval   50;
  ```

### 数值格式字典
- **fvSchemes**：离散格式配置
  ```foam
  ddtSchemes
  {
      default         steadyState;
  }
  gradSchemes
  {
      default         Gauss linear;
      grad(p)         Gauss linear;
  }
  divSchemes
  {
      default         Gauss linear;
      div(phi,U)      Gauss linear;
  }
  ```

### 求解器字典
- **fvSolution**：求解器配置
  ```foam
  solvers
  {
      p
      {
          solver          GAMG;
          tolerance       1e-06;
          relTol          0.1;
          smoother        GaussSeidel;
      }
      U
      {
          solver          smoothSolver;
          smoother        GaussSeidel;
          tolerance       1e-05;
          relTol          0;
      }
  }
  ```

### 网格生成字典
- **blockMeshDict**：结构化网格生成
- **snappyHexMeshDict**：复杂几何网格生成
- **createPatchDict**：边界创建和修改

### 物理属性字典
- **transportProperties**：传输属性
- **turbulenceProperties**：湍流模型属性
- **thermophysicalProperties**：热物理属性
- **radiationProperties**：辐射属性

### 边界条件字典
- **U**：速度边界条件
- **p**：压力边界条件
- **T**：温度边界条件
- **k**、**epsilon**、**omega**：湍流量边界条件

## 使用示例

### 基本字典生成
```cpp
#include "FITKOFControlDictWriter.h"

// 创建 controlDict 写入器
FITKOFControlDictWriter* controlDict = new FITKOFControlDictWriter();

// 设置基本参数
controlDict->setApplication("simpleFoam");
controlDict->setStartFrom("startTime");
controlDict->setStartTime(0);
controlDict->setEndTime(1000);
controlDict->setDeltaT(1);
controlDict->setWriteControl("timeStep");
controlDict->setWriteInterval(50);

// 设置求解器控制
controlDict->setPISOControls(2, 2, 1);
controlDict->setResidualControl(1e-5, 1e-5);

// 生成文件
bool success = controlDict->writeToFile("case/system/controlDict");
```

### 网格字典生成
```cpp
#include "FITKOFBlockMeshDictWriter.h"

// 创建 blockMeshDict 写入器
FITKOFBlockMeshDictWriter* blockMesh = new FITKOFBlockMeshDictWriter();

// 设置转换参数
blockMesh->setConvertToMeters(0.001);

// 添加顶点
blockMesh->addVertex(0, 0, 0);
blockMesh->addVertex(1, 0, 0);
blockMesh->addVertex(1, 1, 0);
blockMesh->addVertex(0, 1, 0);
blockMesh->addVertex(0, 0, 1);
blockMesh->addVertex(1, 0, 1);
blockMesh->addVertex(1, 1, 1);
blockMesh->addVertex(0, 1, 1);

// 添加块
blockMesh->addBlock(0, 1, 2, 3, 4, 5, 6, 7, 100, "simpleGrid");

// 添加边界
blockMesh->addBoundary("inlet", "patch", 0, 1, 5, 4);
blockMesh->addBoundary("outlet", "patch", 2, 3, 7, 6);
blockMesh->addBoundary("walls", "wall", 0, 3, 2, 1);
blockMesh->addBoundary("top", "symmetryPlane", 4, 5, 6, 7);

// 设置边界条件
blockMesh->setBoundaryZones("inlet", 1);
blockMesh->setBoundaryZones("outlet", 1);

// 生成文件
bool success = blockMesh->writeToFile("case/system/blockMeshDict");
```

### 边界条件字典生成
```cpp
#include "FITKOFMeshBoundaryW.h"

// 创建边界条件写入器
FITKOFMeshBoundaryW* boundaryWriter = new FITKOFMeshBoundaryW();

// 设置场类型
boundaryWriter->setFieldType("volVectorField");

// 设置维度信息
boundaryWriter->setDimensions("[0 1 -1 0 0 0 0]");

// 设置内部场
boundaryWriter->setInternalField("uniform (0 0 0)");

// 添加边界条件
boundaryWriter->addBoundary("inlet", "fixedValue");
boundaryWriter->setBoundaryValue("inlet", "uniform (1 0 0)");

boundaryWriter->addBoundary("outlet", "zeroGradient");
boundaryWriter->addBoundary("walls", "noSlip");

// 生成文件
bool success = boundaryWriter->writeToFile("case/0/U");
```

### 湍流模型字典生成
```cpp
#include "FITKOFTurbulencePropertiesWrite.h"

// 创建湍流属性写入器
FITKOFTurbulencePropertiesWrite* turbulenceWriter = new FITKOFTurbulencePropertiesWrite();

// 设置湍流模型
turbulenceWriter->setSimulationType("RAS");
turbulenceWriter->setRASModel("kEpsilon");

// 设置 kEpsilon 模型参数
turbulenceWriter->setTurbulenceModel("kEpsilon");
turbulenceWriter->setParameter("Cmu", 0.09);
turbulenceWriter->setParameter("C1", 1.44);
turbulenceWriter->setParameter("C2", 1.92);
turbulenceWriter->setParameter("sigmaEpsilon", 1.3);

// 设置壁面函数
turbulenceWriter->setWallFunction("kqRWallFunction");
turbulenceWriter->setParameter("kWallFunction", "kqRWallFunction");
turbulenceWriter->setParameter("epsilonWallFunction", "epsilonWallFunction");

// 生成文件
bool success = turbulenceWriter->writeToFile("case/constant/turbulenceProperties");
```

## 字典对象操作

### 基本操作
```cpp
// 创建字典对象
FITKDictObject* dictObj = new FITKDictObject("key", "value");

// 设置数值类型
dictObj->setValue(3.14159);
dictObj->setValue("stringValue");
dictObj->setValue(true);

// 设置向量
FITKDictVector* vector = new FITKDictVector();
vector->append(1.0);
vector->append(2.0);
vector->append(3.0);

// 设置数组
FITKDictArray* array = new FITKDictArray();
array->append("item1");
array->append("item2");
array->append("item3");
```

### 嵌套字典
```cpp
// 创建嵌套字典结构
FITKDictGroup* solvers = new FITKDictGroup("solvers");

// 添加求解器配置
FITKDictGroup* pSolver = new FITKDictGroup("p");
pSolver->addParameter("solver", "GAMG");
pSolver->addParameter("tolerance", 1e-6);
pSolver->addParameter("relTol", 0.1);

FITKDictGroup* uSolver = new FITKDictGroup("U");
uSolver->addParameter("solver", "smoothSolver");
uSolver->addParameter("tolerance", 1e-5);
uSolver->addParameter("relTol", 0);

// 添加到主字典
solvers->addChild(pSolver);
solvers->addChild(uSolver);
```

## 模板系统

### 使用模板
```cpp
// 从模板创建字典
FITKOFControlDictWriter* controlDict = new FITKOFControlDictWriter();
controlDict->loadFromTemplate("templates/controlDict.template");

// 修改模板参数
controlDict->setParameter("endTime", 2000);
controlDict->setParameter("deltaT", 0.5);

// 生成定制化字典
controlDict->writeToFile("case/system/controlDict");
```

### 自定义模板
```cpp
// 创建自定义模板
FITKDictTemplate* customTemplate = new FITKDictTemplate();

// 添加模板参数
customTemplate->addParameter("application", "simpleFoam");
customTemplate->addParameter("startTime", 0);
customTemplate->addParameter("endTime", "${MAX_TIME}");
customTemplate->addParameter("deltaT", "${TIME_STEP}");

// 保存模板
customTemplate->saveToFile("templates/custom.template");
```

## 高级功能

### 参数验证
```cpp
// 设置验证规则
controlDict->addValidationRule("endTime", FITKValidationRule::PositiveNumber);
controlDict->addValidationRule("deltaT", FITKValidationRule::NonNegative);

// 验证参数
FITKValidationResult result = controlDict->validate();
if (!result.isValid()) {
    qDebug() << "Validation errors:" << result.getErrors();
}
```

### 版本兼容性
```cpp
// 设置 OpenFOAM 版本
controlDict->setOpenFOAMVersion("8");

// 自动调整格式兼容性
controlDict->adaptToVersion();

// 检查兼容性
bool isCompatible = controlDict->isCompatibleWithVersion("7");
```

### 批量生成
```cpp
// 创建批量生成器
FITKOFDictBatchGenerator* batchGen = new FITKOFDictBatchGenerator();

// 添加字典文件
batchGen->addDictWriter("controlDict", controlDict);
batchGen->addDictWriter("fvSchemes", schemesWriter);
batchGen->addDictWriter("fvSolution", solutionWriter);

// 批量生成
batchGen->generateAll("case/system/");
```

## 配置选项

### 全局配置
```cpp
// 设置默认格式
FITKOFDictWriter::setDefaultFormat(FITKOFDictWriter::StandardFormat);

// 设置缩进风格
FITKOFDictWriter::setIndentationStyle(FITKOFDictWriter::FourSpaces);

// 设置注释风格
FITKOFDictWriter::setCommentStyle(FITKOFDictWriter::CStyle);
```

### 编码设置
```cpp
// 设置文件编码
dictWriter->setEncoding("UTF-8");

// 设置行结束符
dictWriter->setLineEnding(FITKOFDictWriter::Unix);
```

## 错误处理

### 错误类型
- **语法错误**：字典语法错误
- **参数错误**：参数值错误
- **文件错误**：文件读写错误
- **兼容性错误**：版本兼容性问题

### 错误处理示例
```cpp
try {
    bool success = dictWriter->writeToFile(filePath);
    if (!success) {
        FITKError error = dictWriter->getLastError();
        qDebug() << "Error:" << error.getMessage();
        qDebug() << "Line:" << error.getLine();
        qDebug() << "Type:" << error.getType();
    }
} catch (const FITKDictException& e) {
    qDebug() << "Exception:" << e.what();
}
```

## 依赖关系

- **Qt Core**：基础 Qt 功能
- **FITK_Kernel**：依赖 FITK 核心模块
- **OpenFOAM**：格式兼容性（运行时可选）

## 编译要求

- Qt 5.x 或更高版本
- C++11 或更高标准
- 支持 qmake 构建系统

## 应用场景

- **OpenFOAM 前处理**：自动生成 OpenFOAM 案例配置文件
- **参数化研究**：批量生成不同参数的配置文件
- **工作流程自动化**：集成到自动化工作流程中
- **教学和培训**：快速生成教学案例
- **工程设计**：工程项目的配置管理



## 贡献指南

1. 遵循项目的编码规范
2. 新增字典类型时请添加完整的测试用例
3. 模板功能请保持向后兼容性
4. 提交前请确保所有示例代码正常运行
5. 文档更新请与代码同步