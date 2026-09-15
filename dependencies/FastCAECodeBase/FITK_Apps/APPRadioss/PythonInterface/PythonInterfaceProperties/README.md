# PythonInterfaceProperties

## 简介

PythonInterfaceProperties 是 AppRadioss 中的 Python 接口模块，位于 PythonInterface 层，负责把 Radioss 属性、材料、失效模型、状态方程和曲线等数据对象包装成可被 FITKPython 运行环境调用的类。

该模块本身不直接承担求解或界面功能，而是作为 Python 脚本与 FITKRadiossData 数据对象之间的桥接层，向脚本侧暴露创建对象、按名称查找对象、修改关键参数和触发界面树刷新等能力。

## 架构

### 模块定位

- 所属层级：PythonInterface 模块
- 构建产物：共享库 PythonInterfaceProperties
- 上游依赖：FITKCore、FITKAppFramework、FITKPython、FITKRadiossData、FITKHttpPythonDriver、FITKInterfacePhysics、OperatorsInterface
- 下游使用方：AppRadioss 主程序通过 Python 注册流程加载并注册本模块中的包装类

### 内部结构

模块大致分为五类对象：

1. 基础封装类

   - PyMaterialsBase：封装材料对象的获取、创建、重命名与树刷新。
   - PyPropertyBase：封装属性对象的获取、创建、重命名与树刷新。
   - PyFailureModelBase：封装失效模型的获取、创建、材料关联与树刷新。
   - PyEquationOfStateBase：封装状态方程的获取、创建、材料关联与树刷新。

2. 具体类型包装类

   - 材料：Elastic、Hydro、PlasJohns、PlasBrit、PlasTab
   - 属性：Solid、Shell、SandwichShell、Beam、SPH、BeamSPR
   - 失效模型：BIQUAD、JOHNSON
   - 状态方程：EOSPolynomial

3. 辅助对象

   - Curve：封装非线性曲线对象及点编辑能力。
   - AI_Properties：提供按名称查询材料、属性、失效模型和状态方程类型的辅助接口。

4. Qt 装饰器层

   - 各个 *Wapper 类继承 QObject，通过 slots 向 FITKPython 暴露方法。
   - 包装类内部通常调用基础封装类或具体 Radioss 数据对象，再把执行结果以字符串消息返回给脚本侧。

5. 注册接入层

   - AppRadioss 中的 PythonRegister 会创建并注册本模块的装饰器对象。
   - 注册后，脚本环境可使用 Elastic、Solid、Curve、EOSPolynomial 等类名访问本模块能力。

### 典型调用链

Python 脚本 -> FITKPython 装饰器对象 -> PythonInterfaceProperties 中的 *Wapper -> Py*Base/具体数据对象 -> FITKRadiossCase 中的各类 Manager -> TreePropertyEvent 刷新界面树

## 依赖

### 编译依赖

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- FITKAppFramework
- FITKPython
- FITKRadiossData
- FITKHttpPythonDriver
- FITKInterfacePhysics
- OperatorsInterface

### 运行依赖

- AppRadioss 已初始化 FITKAppFramework，并且全局数据中存在当前 Radioss::FITKRadiossCase。
- 对象创建与查询依赖对应的 Manager 已可用，例如 MaterialManager、SectionManager、FailureModelManager、EquationOfStateManager。
- 若需要把执行消息同步给 Python 侧或服务端，相关包装类会调用 FITKHttpPythonDriver 中的消息发送能力。

### 工程集成关系

- 本模块由 PythonInterface 的顶层 CMake 通过 add_subdirectory(PythonInterfaceProperties) 纳入构建。
- AppRadioss 主程序通过链接 PythonInterfaceProperties 把该模块接入应用。
- 运行时实际注册发生在 AppRadioss 的 PythonRegister 中。

## 主要类与功能说明

### 基础封装类

#### PyInterface::PyMaterialsBase

材料包装的公共基类，负责从当前 RadiossCase 获取 MaterialManager，支持按名称查找材料、创建指定类型材料、修改名称，并在变更后触发 TreePropertyEvent。

#### PyInterface::PyPropertyBase

属性包装的公共基类，负责从当前 RadiossCase 获取 SectionManager，支持创建 Solid、Shell、Beam、BeamSPR、Sandwich、SPH 等属性对象，并在追加对象后刷新属性树。

#### PyInterface::PyFailureModelBase

失效模型包装的公共基类，负责创建 BIQUAD 和 JOHNSON 类型对象，并支持按材料名称添加或移除材料关联关系。

#### PyInterface::PyEquationOfStateBase

状态方程包装的公共基类，负责创建 EOS 对象、管理与材料的关联关系，并提供公共参数设置入口。

### 典型包装类

#### MaterialElasticWapper / Elastic

对应 LAW1 弹性材料，提供 createMaterial、setDensity、setYoungsModulus、setPoissonsRatio 等接口，适合脚本侧快速创建基础结构材料。

#### PropSolidWapper / Solid

对应实体属性对象，提供 createProperty 和多组实体参数设置接口，例如数值阻尼、体积粘性、最小时间步长与单元公式相关参数。

#### EOSPolynomialWapper / EOSPolynomial

对应多项式状态方程对象，支持创建对象、绑定材料以及设置 P0、C0 到 C5、E0、Psh 等参数。

#### FailureModelBIQUADWapper / BIQUAD

对应 BIQUAD 失效模型，负责创建对象并管理与材料对象的关联。

#### CurveWapper / Curve

负责曲线对象的创建与维护，支持添加点、更新点、删除点、清空曲线点和获取点数量，适合材料卡片或曲线驱动参数的脚本化编辑。

#### AI_PropertiesWapper / AI_Properties

提供查询型接口，用于根据对象名称判断材料、属性、失效模型和状态方程的类型，适合作为脚本检查或 AI 助手问答的辅助能力。

### 注册到脚本环境的类名

- AI：AI_Properties
- Curve：Curve
- EOS：EOSPolynomial
- Failure：BIQUAD、JOHNSON
- Material：Elastic、Hydro、PlasJohns、PlasBrit、PlasTab
- Property：Solid、Shell、SandwichShell、Beam、SPH、BeamSPR

## 目录结构

以下文件最能体现模块职责：

- CMakeLists.txt：定义共享库目标、头源文件列表与链接依赖。
- PythonInterfacePropertiesAPI.h：定义模块导出宏。
- PyMaterialsBase.h/.cpp：材料包装公共基类。
- PyPropertyBase.h/.cpp：属性包装公共基类。
- PyFailureModelBase.h/.cpp：失效模型包装公共基类。
- PyEquationOfStateBase.h/.cpp：状态方程包装公共基类。
- Material*.h/.cpp：各类材料包装实现。
- Prop*.h/.cpp：各类属性包装实现。
- FailureModel*.h/.cpp：失效模型包装实现。
- EOSPolynomialWapper.h/.cpp：多项式 EOS 包装实现。
- CurveWapper.h/.cpp：曲线包装实现。
- AI_Properties.h/.cpp：类型查询辅助实现。

## 构建与集成方式

### 在上层工程中的参与方式

本模块不是独立应用程序，通常不单独运行，而是作为 AppRadioss 的一个共享库模块参与整体构建。

### CMake 集成

1. PythonInterface 顶层工程通过 add_subdirectory(PythonInterfaceProperties) 纳入该模块。
2. 本模块构建目标名为 PythonInterfaceProperties。
3. AppRadioss 主程序通过链接 PythonInterfaceProperties 使用其中的装饰器类。

### 产物位置

- Debug：output/bin_d/PythonInterfaceProperties.dll
- Release：output/bin/PythonInterfaceProperties.dll

### 构建前提

- 需要先完成仓库整体依赖初始化。
- 需要保证 Qt5 和各 FITK 相关库可被当前工程正确链接。
- 该模块依赖 FITKRadiossData、FITKPython 与 OperatorsInterface，因此不能脱离上层工程单独裁剪使用。

## 使用方法

### 使用前提

1. AppRadioss 已启动并完成 PythonRegister::registWapper 注册。
2. 当前工程中已有有效的 RadiossCase，全局数据访问链 FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>() 返回非空。
3. 脚本执行环境能够访问注册后的类名，例如 Elastic、Solid、Curve、EOSPolynomial。

### 典型接入步骤

1. 在脚本环境中创建包装对象。
2. 调用 createMaterial、createProperty、createCurve 或 createEquationOfState 创建底层数据对象。
3. 调用具体 setter 方法设置参数。
4. 需要时通过 AI_Properties 或 static_*_Get* 方式按名称重新获取对象。

### 返回值约定

- 多数 *Wapper slots 返回 QString，用于表示成功或失败消息。
- 部分查询接口返回对象名称、类型字符串或数值。
- 对象创建或名称设置失败时，常见原因是对象为空、名称为空、名称冲突或当前 Manager 不可用。

## 使用示例

下面示例展示脚本侧创建一个弹性材料，并查询其类型。示例中的类名和方法名与当前注册和包装代码保持一致；具体导入方式取决于 FITKPython 运行环境。

```python
# 创建弹性材料
mat = Elastic()
print(mat.createMaterial("Steel"))
print(mat.setDensity(7.85e-9))
print(mat.setYoungsModulus(210000.0))
print(mat.setPoissonsRatio(0.30))

# 查询对象类型
helper = AI_Properties()
print(helper.getMaterialType("Steel"))

# 创建多项式 EOS 并绑定到材料
eos = EOSPolynomial()
print(eos.createEquationOfState("SteelEOS"))
print(eos.addMaterial("Steel"))
print(eos.setP0(0.0))
```

若脚本需要编辑曲线，可使用同样的方式创建 Curve 对象并维护曲线点：

```python
curve = Curve()
print(curve.createCurve("LoadCurve-1"))
print(curve.addPoint(0.0, 0.0))
print(curve.addPoint(1.0, 100.0))
print(curve.getPointCount())
```

## 注意事项与限制

- 本模块依赖当前活动的 RadiossCase；若未加载或未创建算例，多数接口会失败。
- 多数对象通过名称查重，脚本侧应避免重复创建同名材料、属性、失效模型或 EOS。
- 对象变更后会通过 TreePropertyEvent 刷新属性树，因此该模块默认面向已接入界面事件系统的应用环境。
- 部分包装类会把执行消息发送到 HttpPython 通道，脚本侧应按字符串结果判断是否成功。
- 模块中的类名与文件名普遍使用 Wapper 这一现有拼写，README 沿用源码命名，不做额外修正。

## 最后更新时间

最后更新时间：2026-06-15