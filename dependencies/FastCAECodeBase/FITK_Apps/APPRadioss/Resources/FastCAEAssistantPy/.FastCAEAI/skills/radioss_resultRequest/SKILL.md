# Radioss 结果请求

## 功能描述

本技能用于配置Radioss仿真结果的输出请求，包括动画输出频率、时间历程设置和各类数据的输出类型。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行结果请求配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 配置流程

### 1. 创建结果请求对象

```python
resultRequest=ResultRequest()
```

创建ResultRequest对象，用于配置结果输出参数。

### 2. 时间设置

#### 设置动画开始时间

```python
resultRequest.setStartTime(0)
```

设置动画输出的开始时间：
- 参数：时间值（秒）
- 默认值：0

#### 设置动画频率

```python
resultRequest.setFrequency(0.0005)
```

设置动画输出频率：
- 参数：频率值（秒），控制动画文件的保存间隔
- 频率值越小，输出结果越密集

#### 设置时间历程频率

```python
resultRequest.setHisFrequency(0.0005)
```

设置时间历程输出频率：
- 参数：频率值（秒），控制时间历程文件的保存间隔
- 通常与动画频率设置为相同值

### 3. 节点数据输出

```python
resultRequest.setNodalOutputAnim([0, 1, 2])
```

设置节点数据的动画输出类型：
- 参数：整数列表，指定要输出的节点数据类型
- 支持选择多个类型，用逗号分隔

**追加节点数据类型：**

```python
resultRequest.appendNodalOutputAnim([3, 4])
```

在现有基础上追加节点数据类型

**节点数据类型：**

| 值 | 类型 | 说明 |
|----|------|------|
| 0 | Nodal_Displacement | 位移 |
| 1 | Nodal_Velocity | 速度 |
| 2 | Nodal_Acceleration | 加速度 |
| 3 | Nodal_ContactForces | 接触力 |
| 4 | Nodal_RigidBodyForcesMoments | 刚体的力和力矩 |
| 5 | Nodal_InternalForces | 内部力 |
| 6 | Nodal_ContactPressure | 接触压力 |
| 7 | Nodal_StressAllIntegrationPoints | 所有积分点的应力GPS |
| 8 | Nodal_StrainAllIntegrationPoints | 所有积分点的应变GPS |

### 4. 单元数据输出

```python
resultRequest.setElementalOutputAnim([0])
```

设置单元数据的动画输出类型：
- 参数：整数列表，指定要输出的单元数据类型
- 主要用于梁、杆、弹簧等一维单元的结果输出

**追加单元数据类型：**

```python
resultRequest.appendElementalOutputAnim([1, 2])
```

在现有基础上追加单元数据类型

**单元数据类型：**

| 值 | 类型 | 说明 |
|----|------|------|
| 0 | Elemental_SpecificEnergyDensity | 特殊能量密度 |
| 1 | Elemental_PlasticStrain | 塑性应变 |
| 2 | Elemental_HourglassEnergy | 沙漏能量 |
| 3 | Elemental_VonMisesStress | 等效应力 |
| 4 | Elemental_Pressure | 压力 |
| 5 | Elemental_Density | 密度 |
| 6 | Elemental_BeamPlasticStrain | 梁的塑性应变 |
| 7 | Elemental_BeamVonMisesStress | 梁的等效应力 |
| 8 | Elemental_BeamFORC | 梁的力与力矩 |
| 9 | Elemental_TrussFORC | Truss的力与力矩 |
| 10 | Elemental_SpringFORC | Spring的力与力矩 |

### 5. Brick数据输出

```python
resultRequest.setBrickOutputAnim([0, 2])
```

设置Brick（六面体）数据的动画输出类型：
- 参数：整数列表，指定要输出的Brick数据类型
- 用于实体单元模型的结果输出

**追加Brick数据类型：**

```python
resultRequest.appendBrickOutputAnim([1, 3])
```

在现有基础上追加Brick数据类型

**Brick数据类型：**

| 值 | 类型 | 说明 |
|----|------|------|
| 0 | Elemental_BrickElementStress | 体应力 |
| 1 | Elemental_BrickElementStrain | 体应变 |
| 2 | Elemental_BrickElementTensor | 体张量 |
| 3 | Elemental_BrickElementEPSP | 体塑性应变 |
| 4 | Elemental_BrickDamageOfAll | 实体的损伤 |

### 6. Shell数据输出

```python
resultRequest.setShellOutputAnim([0, 1])
```

设置Shell（壳）数据的动画输出类型：
- 参数：整数列表，指定要输出的Shell数据类型
- 用于壳单元模型的结果输出

**追加Shell数据类型：**

```python
resultRequest.appendShellOutputAnim([2, 3])
```

在现有基础上追加Shell数据类型

**Shell数据类型：**

| 值 | 类型 | 说明 |
|----|------|------|
| 0 | Elemental_ShellElementStress | 壳应力 |
| 1 | Elemental_ShellElementStrain | 壳应变 |
| 2 | Elemental_ShellElementEPSDOT | 壳应变率张量 |
| 3 | Elemental_ShellDamageOfAll | 壳的损伤 |
| 4 | Elemental_ShellPlasticStrain | 壳的塑性应变 |
| 5 | Elemental_ShellFLDFailure | 壳的FLD损伤 |

### 7. 清理结果请求参数

```python
resultRequest.clearResultRequestParam()
```

清理所有已设置的输出类型参数，重置为默认状态

## 参数说明

### 时间参数

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| setStartTime | double | 设置动画开始时间（秒） |
| getStartTime | - | 获取动画开始时间 |
| setFrequency | double | 设置动画频率（秒） |
| getFrequency | - | 获取动画频率 |
| setHisFrequency | double | 设置时间历程频率（秒） |
| getHisFrequency | - | 获取时间历程频率 |

### 输出类型参数

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| setNodalOutputAnim | list[int] | 设置节点数据输出类型列表（替换） |
| appendNodalOutputAnim | list[int] | 追加节点数据输出类型 |
| getNodalOutputAnim | - | 获取节点数据输出类型列表 |
| setElementalOutputAnim | list[int] | 设置单元数据输出类型列表（替换） |
| appendElementalOutputAnim | list[int] | 追加单元数据输出类型 |
| getElementalOutputAnim | - | 获取单元数据输出类型列表 |
| setBrickOutputAnim | list[int] | 设置Brick数据输出类型列表（替换） |
| appendBrickOutputAnim | list[int] | 追加Brick数据输出类型 |
| getBrickOutputAnim | - | 获取Brick数据输出类型列表 |
| setShellOutputAnim | list[int] | 设置Shell数据输出类型列表（替换） |
| appendShellOutputAnim | list[int] | 追加Shell数据输出类型 |
| getShellOutputAnim | - | 获取Shell数据输出类型列表 |
| clearResultRequestParam | - | 清理所有输出类型参数，重置为默认 |

## 注意事项

1. setXxx方法会替换原有的输出类型列表
2. appendXxx方法在原有基础上追加新的输出类型
3. 输出类型使用整数代码表示，具体含义见类型表格
4. 频率值越小，输出结果越密集，占用存储空间越大
5. 合理选择输出类型可以减少计算和存储开销
6. 节点数据主要用于梁、杆、弹簧等一维单元
7. Shell数据用于壳单元模型
8. Brick数据用于实体单元模型
9. 根据仿真需求选择合适的输出类型，避免输出过多无用数据
10. clearResultRequestParam用于重置所有输出类型设置