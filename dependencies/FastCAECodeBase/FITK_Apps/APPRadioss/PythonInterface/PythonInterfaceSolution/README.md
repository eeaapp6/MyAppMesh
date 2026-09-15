# PythonInterfaceSolution

## 简介

PythonInterfaceSolution 是 AppRadioss 中的 Python 接口模块，位于 PythonInterface 层，负责把 Radioss 求解方案相关的数据对象包装成 FITKPython 可调用的脚本类。

它面向求解配置阶段，覆盖求解步、重力场、边界条件、相互作用、初速度场、探针、刚性墙、求解器设置和结果请求等对象，主要作用是让脚本能够在当前算例或当前解步上下文中创建对象、修改关键参数，并同步刷新树和预处理图形。

## 架构

### 模块定位

- 所属层级：PythonInterface 模块
- 构建产物：共享库 PythonInterfaceSolution
- 上游依赖：FITKCore、FITKAppFramework、FITKPython、FITKInterfaceModel、FITKRadiossData、FITKHttpPythonDriver、OperatorsInterface
- 下游使用方：AppRadioss 通过 PythonRegister 注册本模块的装饰器类，脚本环境据此访问求解配置对象

### 内部结构

模块可分为四类能力：

1. 解步上下文管理

   - Solution / SolutionWapper：创建求解步、设置当前求解步，并作为其他大多数包装类的上下文入口。

2. 公共基类封装

   - PyBCSBase：边界条件公共封装。
   - PyInteractionBase：相互作用公共封装。
   - PyInitialValueBase：初速度场公共封装。
   - PyProbeBase：探针公共封装。
   - PyRWallBase：刚性墙公共封装。

3. 具体对象包装类

   - Gravity：重力场
   - BCS：边界条件
   - InterTied、InterMultiUsageImpact：相互作用
   - InterTiedDefault、InterMultiUsageImpactDefault：默认相互作用
   - InitialValueTra、InitialValueAxis：初速度场
   - ProbeAccelerometer、ProbeSection：探针
   - RigidBody：刚体连接
   - RWallPlane、RWallCylinder：刚性墙
   - SolverSetting：求解器设置
   - ResultRequest：结果请求
   - AI_Solution：查询型辅助接口

4. 渲染与消息同步

   - 大多数对象修改后会触发 TreeSolutionEvent 刷新求解树。
   - 相互作用、刚性墙等对象还会更新 GraphPreprocess 预处理图形。
   - 多数 slots 通过 QString 返回成功或失败消息，并可同步发送到 HttpPython 通道。

### 两类运行上下文

本模块中的对象并不全部依赖同一层级上下文：

- 依赖当前求解步的对象：Gravity、BCS、Interaction、InitialValue、Probe、RWall、RigidBody、AI_Solution
- 直接访问当前算例级配置的对象：SolverSetting、ResultRequest

这意味着脚本在编辑解步对象前通常要先确保当前解步存在；而求解器设置与结果请求则直接从当前 RadiossCase 获取。

### 典型调用链

Python 脚本 -> FITKPython 装饰器对象 -> PythonInterfaceSolution 中的 *Wapper -> Py*Base 或具体包装类 -> 当前 FITKRadiossSolution 或 FITKRadiossCase -> TreeSolutionEvent / GraphPreprocess 更新

## 依赖

### 编译依赖

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- FITKAppFramework
- FITKPython
- FITKInterfaceModel
- FITKRadiossData
- FITKHttpPythonDriver
- OperatorsInterface

### 运行依赖

- AppRadioss 已初始化 FITKAppFramework，且全局数据中存在当前 Radioss::FITKRadiossCase。
- 需要当前算例能够返回 SolutionManager 以及各类求解相关 manager。
- 对于解步级对象，必须存在当前求解步，或先通过 Solution 创建并切换到某个求解步。
- 若对象修改需要界面联动，运行环境应已注册 TreeSolutionEvent 和 GraphPreprocess 等操作器。

### 工程集成关系

- PythonInterface 顶层工程通过 add_subdirectory(PythonInterfaceSolution) 纳入该模块。
- AppRadioss 主程序通过链接 PythonInterfaceSolution 集成该模块。
- 运行时注册发生在 AppRadioss 的 PythonRegister 中。

## 功能列表

- 创建和切换当前求解步。
- 创建和修改重力场、边界条件、相互作用、初速度场、探针和刚性墙对象。
- 查询刚性墙、初速度场、相互作用、探针和边界条件的类型。
- 调整求解器时间步、动态松弛、AMS 相关参数等求解设置。
- 配置动画输出、时间历程频率和结果请求参数。
- 在对象更新后同步刷新求解树和预处理图形。

## 主要类与功能说明

### 解步入口类

#### Solution / SolutionWapper

这是本模块最核心的入口类。它负责创建求解步、按名称切换当前求解步，并维护一个静态当前解步指针；其他多数包装类都会通过 Solution::getCurrentSolution 获取工作上下文。

### 公共基类

#### PyInterface::PyBCSBase

封装边界条件对象的获取、创建、重命名、节点组绑定和界面刷新。BCSWapper 在此基础上补充自由度固定等具体设置接口。

#### PyInterface::PyInteractionBase

封装相互作用对象的创建、主面设置、从节点组设置和界面同步。它依赖当前解步中的 InteractionManager，并在修改后同时更新求解树和预处理图形。

#### PyInterface::PyRWallBase

封装刚性墙对象的公共逻辑，负责创建对象、设置滑移类型、搜索距离、滤波参数和节点组。

#### PyInterface::PyInitialValueBase 与 PyInterface::PyProbeBase

分别负责初速度场和探针对象的公共访问与创建逻辑，供具体类型包装类复用。

### 典型包装类

#### Gravity / GravityWapper

用于创建重力场，并设置时间函数、重力方向、节点组和缩放参数。它依赖当前解步中的 GravityManager，同时可引用算例级曲线和网格组件对象。

#### BCS / BCSWapper

用于创建边界条件并设置节点组与自由度固定状态，适合脚本快速生成基本约束条件。

#### InterTied / InterMultiUsageImpact

用于创建和设置相互作用对象，包括主面和从节点组。默认相互作用包装类 InterTiedDefault、InterMultiUsageImpactDefault 面向默认相互作用配置。

#### RWallPlane / RWallCylinder

用于创建刚性墙对象并设置几何与接触相关参数。RWallPlane 侧重平面基点与法向量，RWallCylinder 侧重圆柱几何参数。

#### SolverSetting / SolverSettingWapper

这组接口不创建新的求解器设置对象，而是获取当前算例中的 FITKRadiossSolverSettings，并对其中的时间步控制、动态松弛、AMS 和输出相关参数进行修改。

#### ResultRequest / ResultRequestWapper

这组接口不创建新的结果请求数据对象，而是访问当前算例中的 FITKRadiossResultRequest，配置动画输出、时间历程频率和输出项列表。

#### AI_Solution / AI_SolutionWapper

提供查询型能力，用于根据名称判断刚性墙、初速度场、相互作用、探针和边界条件的类型，适合作为脚本校验或 AI 助手辅助分析入口。

### 注册到脚本环境的类名

- Solution：Solution
- Gravity：Gravity
- BCS：BCS
- Interaction：InterTied、InterMultiUsageImpact、InterTiedDefault、InterMultiUsageImpactDefault
- InitialValue：InitialValueTra、InitialValueAxis
- Probe：ProbeAccelerometer、ProbeSection
- Connection：RigidBody
- RWall：RWallPlane、RWallCylinder
- Solve：SolverSetting、ResultRequest
- AI：AI_Solution

## 目录结构

以下文件最能体现模块职责：

- CMakeLists.txt：定义共享库目标、头源文件列表与链接依赖。
- PythonInterfaceSolutionAPI.h：定义模块导出宏。
- SolutionWapper.h/.cpp：当前求解步入口与上下文切换。
- PyBCSBase.h/.cpp：边界条件公共封装。
- PyInteractionBase.h/.cpp：相互作用公共封装。
- PyInitialValueBase.h/.cpp：初速度场公共封装。
- PyProbeBase.h/.cpp：探针公共封装。
- PyRWallBase.h/.cpp：刚性墙公共封装。
- GravityWapper.h/.cpp：重力场包装实现。
- SolverSettingWapper.h/.cpp：求解器设置包装实现。
- ResultRequestWapper.h/.cpp：结果请求包装实现。
- AI_Solution.h/.cpp：类型查询辅助实现。

## 构建与集成方式

### 在上层工程中的参与方式

本模块不是独立应用程序，通常作为 AppRadioss 的共享库模块参与整体构建，并在 Python 脚本环境中被注册使用。

### CMake 集成

1. PythonInterface 顶层工程通过 add_subdirectory(PythonInterfaceSolution) 纳入该模块。
2. 本模块构建目标名为 PythonInterfaceSolution。
3. AppRadioss 主程序通过链接 PythonInterfaceSolution 将其接入应用。

### 产物位置

- Debug：output/bin_d/PythonInterfaceSolution.dll
- Release：output/bin/PythonInterfaceSolution.dll

### 构建前提

- 需要先完成仓库整体初始化和依赖拉取。
- 需要保证 Qt5 与 FITK 相关库可被当前工程正确链接。
- 该模块依赖 FITKRadiossData、FITKPython 和 OperatorsInterface，不能脱离上层工程单独运行验证。

## 使用方法

### 使用前提

1. AppRadioss 已启动并完成 PythonRegister::registWapper 注册。
2. 当前环境存在有效的 RadiossCase。
3. 若要编辑解步级对象，应先创建或切换到当前求解步。
4. 若脚本依赖返回消息做判断，应检查各个 slots 返回的 QString 内容。

### 典型接入步骤

1. 使用 Solution 创建求解步，或切换到已有求解步。
2. 创建并配置重力场、边界条件、相互作用、刚性墙等解步级对象。
3. 通过 SolverSetting 和 ResultRequest 调整算例级求解控制与输出请求。
4. 需要校验对象类型时，通过 AI_Solution 按名称查询。

### 返回值约定

- 多数 *Wapper 接口返回 QString，表示执行成功或失败信息。
- SolutionWapper 中 createSolution、setName 等个别接口以 void 方式暴露，但底层仍依赖当前算例与名称合法性。
- 对象创建失败的常见原因包括：当前解步为空、当前算例为空、名称冲突、目标节点组或主面不存在。

## 使用示例

下面示例展示脚本侧创建一个求解步，并在其下配置重力、边界条件、求解器设置和结果请求。示例中的类名与方法名均来自当前注册与包装代码。

```python
# 创建并切换到一个求解步
sol = Solution()
sol.createSolution("CrashStep")

# 创建重力场
gravity = Gravity()
print(gravity.createGravity("Gravity-1"))
print(gravity.setGravityDirection("z"))
print(gravity.setNodeGroup("Global"))

# 创建边界条件并固定部分自由度
bcs = BCS()
print(bcs.createBCS("FixBase"))
print(bcs.setNodeGroup("BaseNodes"))
print(bcs.setFreedomFixed(1, True))
print(bcs.setFreedomFixed(2, True))
print(bcs.setFreedomFixed(3, True))

# 调整求解器设置
solver = SolverSetting()
print(solver.setRunExeTime(0.02))
print(solver.setScaleFactor(0.9))

# 配置结果请求
result = ResultRequest()
print(result.setStartTime(0.0))
print(result.setHisFrequency(1000.0))
```

若脚本只需要判断某个对象的类型，可使用 AI_Solution：

```python
ai = AI_Solution()
print(ai.getBCSType("FixBase"))
print(ai.getRWallType("Wall-1"))
```

## 注意事项与限制

- 大多数解步级包装类都依赖 Solution::getCurrentSolution；如果当前解步为空，构造对象后相关操作会失败。
- SolverSetting 和 ResultRequest 访问的是当前算例级对象，不是为每个解步单独创建的新实例。
- 默认相互作用包装应按默认管理器中的第一个对象理解和使用，脚本侧不应依赖同一 solution 下多个默认相互作用对象同时生效。
- 相互作用、刚性墙等对象更新后会触发 GraphPreprocess 图形刷新；若运行环境未完整初始化相关操作器，界面联动可能不完整。
- ResultRequest 中的时间历程输出频率接口对应算例级结果请求配置，脚本不应把它误当作某个单独探针对象的局部参数。
- 模块中的类名与文件名沿用源码中的 Wapper 拼写，README 不对其命名做额外修正。

## 最后更新时间

最后更新时间：2026-06-15