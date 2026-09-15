# Radioss 动力学绑定相互作用

## 功能描述

本技能用于在Radioss中创建和修改动力学绑定相互作用，支持设置名称、主面、从节点组、层级、搜索距离以及绑定默认参数和可选参数。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行动力学绑定相互作用配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建动力学绑定相互作用

### 1. 创建动力学绑定相互作用

创建动力学绑定相互作用操作对象后，可以创建动力学绑定相互作用。

```python
p=InterTied.InterTied()
p.createInteraction('Interaction-1')
```

- 参数：相互作用名称（字符串）
- 使用指定名称创建动力学绑定相互作用
- 注意：如果已存在同名相互作用会创建失败

**完整示例：创建动力学绑定相互作用并设置部分参数**

```python
p=InterTied.InterTied()
p.createInteraction('Interaction-1')
p.setMasterSurface('Surface-1')
p.setNodeGroup('NodeGroup-1')
p.setLevel(0)
p.setSearchDistance(0.0)
p.setIgnoreFlag(0)
p.setSpotWeldFormulationFlag(0)
p.setSearchFormulationFlag(2)
p.setNodeDeletionFlag(1000)
```

## 修改动力学绑定相互作用

### 1. 获取动力学绑定相互作用对象

根据相互作用名称获取已存在的动力学绑定相互作用对象：
- 参数：相互作用名称（字符串）
- 编辑前可先通过 `AI.AI_Solution().getInteractionType('Interaction-1')` 判断对象是否为 `KinematicTied` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
p=InterTied.InterTied.GetInterTied('Interaction-1')
```

## 修改动力学绑定相互作用参数

获取对象后，可修改以下属性：

### 1. 修改相互作用名称

```python
p.setName('Interaction-2')
```

### 2. 设置主面

```python
p.setMasterSurface('Surface-1')
```

### 3. 设置从节点组

```python
p.setNodeGroup('NodeGroup-1')
```

### 4. 设置层级

```python
p.setLevel(0)
```

### 5. 设置搜索距离

```python
p.setSearchDistance(0.0)
```

### 6. 设置界面脚本直接输出的默认参数

```python
p.setIgnoreFlag(0)
p.setSpotWeldFormulationFlag(0)
p.setSearchFormulationFlag(2)
p.setNodeDeletionFlag(1000)
```

### 7. 设置绑定可选参数

```python
p.setRupt(0)
p.setIfilter(0)
p.setFctIDsr('Curve-1')
p.setFctIDsn('Curve-2')
p.setFctIDst('Curve-3')
p.setIsym(0)
p.setMaxNDist(1.0)
p.setMaxTDist(1.0)
p.setFscaleStress(1.0)
p.setFscaleStrRate(1.0)
p.setFscaleDist(1.0)
p.setAlpha(1.0)
p.setArea(0.0)
p.setStfac(1.0)
p.setVisc(0.05)
p.setIstf(0)
```

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| InterTied | - | 创建InterTied对象 |
| InterTied.GetInterTied | string | 根据名称获取已存在的动力学绑定对象 |
| createInteraction | string | 创建指定名称的动力学绑定相互作用 |
| setName | string | 修改相互作用名称 |
| setMasterSurface | string | 设置主面名称 |
| setNodeGroup | string | 设置从节点组名称 |
| setLevel | int | 设置层级 |
| setSearchDistance | double | 设置搜索距离 |
| setIgnoreFlag | int | 设置忽略标志 |
| setSpotWeldFormulationFlag | int | 设置点焊公式标志 |
| setSearchFormulationFlag | int | 设置搜索公式标志 |
| setNodeDeletionFlag | int | 设置节点删除标志 |
| setRupt | int | 设置破坏标志 |
| setIfilter | int | 设置滤波标志 |
| setFctIDsr | string | 设置应力率相关函数曲线 |
| setFctIDsn | string | 设置法向应力-相对位移函数曲线 |
| setFctIDst | string | 设置切向应力-相对位移函数曲线 |
| setIsym | int | 设置对称标志 |
| setMaxNDist | double | 设置最大法向距离 |
| setMaxTDist | double | 设置最大切向距离 |
| setFscaleStress | double | 设置应力缩放系数 |
| setFscaleStrRate | double | 设置应变率缩放系数 |
| setFscaleDist | double | 设置距离缩放系数 |
| setAlpha | double | 设置 alpha 系数 |
| setArea | double | 设置面积 |
| setStfac | double | 设置应力比例系数 |
| setVisc | double | 设置粘性系数 |
| setIstf | int | 设置 Istf 标志 |

## 注意事项

1. 默认使用当前有效的Solution，无需额外判断或设置。
2. 同名相互作用已存在时，`createInteraction` 会失败。
3. 编辑前建议先通过 `InterTied.InterTied.GetInterTied` 获取对象。
4. 编辑已有相互作用前，建议先通过 `AI.AI_Solution().getInteractionType('相互作用名')` 确认对象类型。
5. `setMasterSurface` 和 `setNodeGroup` 需要传入当前模型中已存在的主面和节点组名称。
6. 界面 `writePythonScript` 当前只直接输出 `setIgnoreFlag`、`setSpotWeldFormulationFlag`、`setSearchFormulationFlag`、`setNodeDeletionFlag` 这组默认参数；其它可选参数可通过 wrapper 接口补充设置。
7. 曲线相关接口需要传入已存在的曲线名称。
