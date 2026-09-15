# Radioss 求解器设置

## 功能描述

本技能用于配置 Radioss 求解器设置对象中的可写参数，覆盖运行时间、全局时间、节点和单元时间步控制、自适应动态松弛、AMS 时间步控制、计算控制、收敛标准、输出控制以及全局 SPH 参数。

## 使用方法

通过 Python 语言接口调用，使用 submitPythonCommand 执行求解器设置脚本。

**执行规则：submitPythonCommand 接口可以执行多条命令，各命令之间使用 ; 分割开。**

## 创建求解器设置对象

### 1. 创建求解器设置对象

创建 SolverSetting 对象后，即可对当前有效 Solution 的求解器设置参数进行修改。

```python
solverSetting=SolverSetting()
```

**完整示例：创建求解器设置对象并设置常用参数**

```python
solverSetting=SolverSetting()
solverSetting.setRunExeTime(0.001)
solverSetting.setScaleFactor(0.9)
solverSetting.setEnableAdditionalCards(False)
solverSetting.setToleranceGTS(0.01)
solverSetting.setNumCycles(20)
solverSetting.setNodalTimeStepControlType(0)
solverSetting.setNodalMinimumTimeStep(0.0)
solverSetting.setNodalScaleFactor(0.0)
solverSetting.setInitialAddMassRatio(0.0)
solverSetting.setElementalTimeStepControlType(0)
solverSetting.setElementalMinimumTimeStep(0.0)
solverSetting.setElementalScaleFactor(0.0)
solverSetting.setNumberOfAddCards(False)
solverSetting.setMinAspectRatio(0.2)
solverSetting.setMinVolumeRatio(0.05)
solverSetting.setEnableDynamicRelaxation(False)
solverSetting.setEnableStartTime(False)
solverSetting.setStartTime(0.0)
solverSetting.setEnableStopTime(False)
solverSetting.setStopTime(0.0)
solverSetting.setEnableCriticalTimeStepScaleFactor(False)
solverSetting.setCriticalTimeStepScaleFactor(0.9)
solverSetting.setEnableMinTimeStep(False)
solverSetting.setAMSMinTimeStep(0.0)
solverSetting.setEnableAMSConvergenceTolerance(False)
solverSetting.setAMSConvergenceTolerance(0.0001)
solverSetting.setEnableMaxIterationsConjugateGradient(False)
solverSetting.setMaxIterationsConjugateGradient(1000)
solverSetting.setEnableNumberOfAdditionalOutputs(False)
solverSetting.setNumberOfAdditionalOutputs(0.0)
solverSetting.setStopComputationWhenNegativeVolume(True)
solverSetting.setParallelArithmetic(True)
solverSetting.setConvergenceEnableState(False)
solverSetting.setEnergyErrorRatio(0.99)
solverSetting.setTotalMassRatio(0.05)
solverSetting.setNodalMassRatio(0.0)
solverSetting.setWriteTimeHistoryFile(True)
solverSetting.setWriteAnimationFile(True)
solverSetting.setEnergyError(True)
solverSetting.setGlobalSPHEnableState(False)
solverSetting.setSPHAsort(0.25)
solverSetting.setSPHMaxsph(0)
solverSetting.setSPHLneigh(120)
solverSetting.setSPHNneigh(0)
solverSetting.setSPHIsol2sph(0)
```

## 修改求解器设置参数

创建对象后，可按以下参数组修改求解器设置。

### 1. 运行时间与全局时间参数

```python
solverSetting.setRunExeTime(0.001)
solverSetting.setScaleFactor(0.9)
solverSetting.setEnableAdditionalCards(False)
solverSetting.setToleranceGTS(0.01)
solverSetting.setNumCycles(20)
```

- setRunExeTime：设置求解运行最终时间，默认值为 0.001。
- setScaleFactor：设置全局时间停止缩放因子，默认值为 0.9。
- setEnableAdditionalCards：控制是否允许用户输入 GTS 收敛的容差与周期数，默认值为 False。
- setToleranceGTS：设置 GTS 收敛容差，默认值为 0.01。
- setNumCycles：设置 GTS 收敛相关周期数，默认值为 20。

### 2. 节点时间步控制参数

```python
solverSetting.setNodalTimeStepControlType(0)
solverSetting.setNodalMinimumTimeStep(0.0)
solverSetting.setNodalScaleFactor(0.0)
solverSetting.setInitialAddMassRatio(0.0)
```

- setNodalTimeStepControlType：设置节点时间步控制类型。
  - 0：SolverDefault
  - 1：CST
  - 2：SET
  - 3：STOP
- setNodalMinimumTimeStep：设置节点控制的最小时间步，默认值为 0。
- setNodalScaleFactor：设置节点时间步缩放因子，默认值为 0。
- setInitialAddMassRatio：设置节点时间步控制中的初始附加质量比，默认值为 0。

### 3. 单元时间步控制参数

```python
solverSetting.setElementalTimeStepControlType(0)
solverSetting.setElementalMinimumTimeStep(0.0)
solverSetting.setElementalScaleFactor(0.0)
solverSetting.setNumberOfAddCards(False)
solverSetting.setMinAspectRatio(0.2)
solverSetting.setMinVolumeRatio(0.05)
```

- setElementalTimeStepControlType：设置单元时间步控制类型。
  - 0：SolverDefault
  - 1：CST
  - 2：DEL
  - 3：STOP
- setElementalMinimumTimeStep：设置单元控制的最小时间步，默认值为 0。
- setElementalScaleFactor：设置单元时间步缩放因子，默认值为 0。
- setNumberOfAddCards：设置单元时间步附加卡片开关，默认值为 False。
- setMinAspectRatio：设置单元最小长宽比阈值，默认值为 0.2。
- setMinVolumeRatio：设置单元最小体积比阈值，默认值为 0.05。

### 4. 自适应动态松弛参数

```python
solverSetting.setEnableDynamicRelaxation(False)
solverSetting.setEnableStartTime(False)
solverSetting.setStartTime(0.0)
solverSetting.setEnableStopTime(False)
solverSetting.setStopTime(0.0)
```

- setEnableDynamicRelaxation：是否启用自适应动态松弛，默认值为 False。
- setEnableStartTime：是否启用动态松弛开始时间，默认值为 False。
- setStartTime：设置动态松弛开始时间，默认值为 0.0。
- setEnableStopTime：是否启用动态松弛停止时间，默认值为 False。
- setStopTime：设置动态松弛停止时间，默认值为 0.0。

### 5. AMS 时间步与收敛辅助参数

```python
solverSetting.setEnableCriticalTimeStepScaleFactor(False)
solverSetting.setCriticalTimeStepScaleFactor(0.9)
solverSetting.setEnableMinTimeStep(False)
solverSetting.setAMSMinTimeStep(0.0)
solverSetting.setEnableAMSConvergenceTolerance(False)
solverSetting.setAMSConvergenceTolerance(0.0001)
solverSetting.setEnableMaxIterationsConjugateGradient(False)
solverSetting.setMaxIterationsConjugateGradient(1000)
solverSetting.setEnableNumberOfAdditionalOutputs(False)
solverSetting.setNumberOfAdditionalOutputs(0.0)
```

- setEnableCriticalTimeStepScaleFactor：是否启用 AMS 临界时间步缩放因子。
- setCriticalTimeStepScaleFactor：设置 AMS 临界时间步缩放因子，默认值为 0.9。
- setEnableMinTimeStep：是否启用 AMS 最小时间步。
- setAMSMinTimeStep：设置 AMS 最小时间步，默认值为 0.0。
- setEnableAMSConvergenceTolerance：是否启用 AMS 收敛容差。
- setAMSConvergenceTolerance：设置 AMS 收敛容差，默认值为 0.0001。
- setEnableMaxIterationsConjugateGradient：是否启用共轭梯度最大迭代次数。
- setMaxIterationsConjugateGradient：设置共轭梯度最大迭代次数，默认值为 1000。
- setEnableNumberOfAdditionalOutputs：是否启用附加输出数量。
- setNumberOfAdditionalOutputs：设置附加输出数量，默认值为 0.0。

### 6. 计算控制与收敛标准参数

```python
solverSetting.setStopComputationWhenNegativeVolume(True)
solverSetting.setParallelArithmetic(True)
solverSetting.setConvergenceEnableState(False)
solverSetting.setEnergyErrorRatio(0.99)
solverSetting.setTotalMassRatio(0.05)
solverSetting.setNodalMassRatio(0.0)
```

- setStopComputationWhenNegativeVolume：是否在出现负体积时停止计算，默认值为 True。
- setParallelArithmetic：是否启用并行运算，默认值为 True。
- setConvergenceEnableState：收敛标准总开关，默认值为 False。
- setEnergyErrorRatio：设置能量误差比阈值，默认值为 0.99。
- setTotalMassRatio：设置总质量比阈值，默认值为 0.05。
- setNodalMassRatio：设置节点质量比阈值，默认值为 0.0。

### 7. 输出控制参数

```python
solverSetting.setWriteTimeHistoryFile(True)
solverSetting.setWriteAnimationFile(True)
solverSetting.setEnergyError(True)
```

- setWriteTimeHistoryFile：是否写入时间历史文件，默认值为 True。
- setWriteAnimationFile：是否写入动画文件，默认值为 True。
- setEnergyError：是否启用能量误差相关输出或控制，默认值为 True。

### 8. 全局 SPH 参数

```python
solverSetting.setGlobalSPHEnableState(True)
solverSetting.setSPHAsort(0.25)
solverSetting.setSPHMaxsph(0)
solverSetting.setSPHLneigh(120)
solverSetting.setSPHNneigh(0)
solverSetting.setSPHIsol2sph(0)
```

- setGlobalSPHEnableState：启用或关闭全局 SPH 参数对象。
- setSPHAsort：设置邻居搜索安全系数，默认值为 0.25。
- setSPHMaxsph：设置单次允许创建的幽灵粒子最大数量，默认值为 0。
- setSPHLneigh：设置参与 SPH 近似计算的邻居粒子最大数量，默认值为 120。
- setSPHNneigh：设置邻居粒子的最大数量上限，默认值为 0。
- setSPHIsol2sph：设置 Sol2SPH 粒子活化过程类型，默认值为 0。

### 9. 清理求解器设置参数

```python
solverSetting.clearSolverSettingParam()
```

- clearSolverSettingParam：清理当前实现已覆盖的求解器设置启用状态。按当前实现，会关闭收敛标准启用状态，并关闭全局 SPH 参数对象。

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| SolverSetting | - | 创建 SolverSetting 对象 |
| setRunExeTime | double | 设置求解运行最终时间，默认 0.001 |
| setScaleFactor | double | 设置全局时间停止缩放因子，默认 0.9 |
| setEnableAdditionalCards | bool | 是否启用用户输入 GTS 收敛容差与周期数 |
| setToleranceGTS | double | 设置 GTS 收敛容差，默认 0.01 |
| setNumCycles | int | 设置 GTS 收敛周期数，默认 20 |
| setNodalTimeStepControlType | int | 设置节点时间步控制类型：0 SolverDefault，1 CST，2 SET，3 STOP |
| setNodalMinimumTimeStep | double | 设置节点控制的最小时间步，默认 0 |
| setNodalScaleFactor | double | 设置节点时间步缩放因子，默认 0 |
| setInitialAddMassRatio | double | 设置节点时间步控制中的初始附加质量比，默认 0 |
| setElementalTimeStepControlType | int | 设置单元时间步控制类型：0 SolverDefault，1 CST，2 DEL，3 STOP |
| setElementalMinimumTimeStep | double | 设置单元控制的最小时间步，默认 0 |
| setElementalScaleFactor | double | 设置单元时间步缩放因子，默认 0 |
| setNumberOfAddCards | bool | 设置单元时间步附加卡片开关，默认 False |
| setMinAspectRatio | double | 设置单元最小长宽比阈值，默认 0.2 |
| setMinVolumeRatio | double | 设置单元最小体积比阈值，默认 0.05 |
| setEnableDynamicRelaxation | bool | 是否启用自适应动态松弛，默认 False |
| setEnableStartTime | bool | 是否启用动态松弛开始时间 |
| setStartTime | double | 设置动态松弛开始时间，默认 0.0 |
| setEnableStopTime | bool | 是否启用动态松弛停止时间 |
| setStopTime | double | 设置动态松弛停止时间，默认 0.0 |
| setEnableCriticalTimeStepScaleFactor | bool | 是否启用 AMS 临界时间步缩放因子 |
| setCriticalTimeStepScaleFactor | double | 设置 AMS 临界时间步缩放因子，默认 0.9 |
| setEnableMinTimeStep | bool | 是否启用 AMS 最小时间步 |
| setAMSMinTimeStep | double | 设置 AMS 最小时间步，默认 0.0 |
| setEnableAMSConvergenceTolerance | bool | 是否启用 AMS 收敛容差 |
| setAMSConvergenceTolerance | double | 设置 AMS 收敛容差，默认 0.0001 |
| setEnableMaxIterationsConjugateGradient | bool | 是否启用共轭梯度最大迭代次数 |
| setMaxIterationsConjugateGradient | double | 设置共轭梯度最大迭代次数，默认 1000 |
| setEnableNumberOfAdditionalOutputs | bool | 是否启用附加输出数量 |
| setNumberOfAdditionalOutputs | double | 设置附加输出数量，默认 0.0 |
| setStopComputationWhenNegativeVolume | bool | 是否在出现负体积时停止计算，默认 True |
| setParallelArithmetic | bool | 是否启用并行运算，默认 True |
| setConvergenceEnableState | bool | 收敛标准总开关，默认 False |
| setEnergyErrorRatio | double | 设置能量误差比阈值，默认 0.99 |
| setTotalMassRatio | double | 设置总质量比阈值，默认 0.05 |
| setNodalMassRatio | double | 设置节点质量比阈值，默认 0.0 |
| setWriteTimeHistoryFile | bool | 是否写入时间历史文件，默认 True |
| setWriteAnimationFile | bool | 是否写入动画文件，默认 True |
| setEnergyError | bool | 是否启用能量误差相关输出或控制，默认 True |
| setGlobalSPHEnableState | bool | 启用或关闭全局 SPH 参数对象 |
| setSPHAsort | double | 设置邻居搜索安全系数，默认 0.25 |
| setSPHMaxsph | int | 设置单次允许创建的幽灵粒子最大数量，默认 0 |
| setSPHLneigh | int | 设置参与 SPH 近似计算的邻居粒子最大数量，默认 120 |
| setSPHNneigh | int | 设置邻居粒子的最大数量上限，默认 0 |
| setSPHIsol2sph | int | 设置 Sol2SPH 粒子活化过程类型，默认 0 |
| clearSolverSettingParam | 无 | 清理当前实现已覆盖的求解器设置启用状态，当前会关闭收敛标准和全局 SPH |

## 注意事项

1. 默认使用当前有效的 Solution，无需额外切换求解器设置对象。
2. 大部分参数都有默认值；如无特殊需求，可只修改必要字段。
3. 启用类设置通常需要与对应数值参数配合使用，例如先启用再设置阈值、时间或次数。
4. setEnableAdditionalCards 应与 setToleranceGTS、setNumCycles 结合使用。
5. setNumberOfAddCards 是单元时间步控制中的布尔开关，不是数值数量参数。
6. setGlobalSPHEnableState(False) 时，SPH 相关数值即使被设置，也通常不参与生效。
7. clearSolverSettingParam 不是完全恢复全部默认值的重置接口；按当前实现，它只关闭收敛标准和全局 SPH 启用状态。