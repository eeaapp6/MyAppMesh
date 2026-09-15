# Radioss PlasJohns材料

## 功能描述

本技能用于在Radioss中编辑已有PlasJohns材料或创建，支持设置材料名称、材料子类型、密度、弹性模量、泊松比、屈服强度、塑性参数、应变率参数以及温度相关参数。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行PlasJohns材料配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建PlasJohns材料

### 1. 创建PlasJohns材料

创建PlasJohns材料操作对象后，可以创建PlasJohns材料。

```python
material=Material.PlasJohns()
material.createMaterial('Material-1')
```

- 参数：材料名称（字符串）
- 使用指定名称创建PlasJohns材料
- 注意：如果已存在同名材料会创建失败

**完整示例：创建PlasJohns材料并设置部分参数**

```python
material=Material.PlasJohns()
material.createMaterial('Material-1')
material.setType(0)
material.setDensity(7.85e-06)
material.setYoungsModulus(210)
material.setPoissonsRatio(0.3)
material.setYieldStress(0.5)
material.setPlasticHardening(0.5)
material.setPlasticHardeningExponent(0.5)
```

## 修改PlasJohns材料

### 1. 获取PlasJohns材料对象

根据材料名称获取已存在的PlasJohns材料对象：
- 参数：材料名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getMaterialType('Material-1')` 判断对象是否为 `PlasJohns` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
material=Material.PlasJohns.GetPlasJohns('Material-1')
```

## 修改PlasJohns材料参数

获取对象后，可修改以下属性：

### 1. 修改材料名称

```python
material.setName('Material-2')
```

设置PlasJohns材料的新名称：
- 参数：材料名称（字符串）

### 2. 设置材料子类型

```python
material.setType(0)
```

设置PlasJohns材料子类型：
- 参数：子类型编号（整数）
- `0` 表示 `CLASSIC`
- `1` 表示 `SIMPLE`

### 3. 设置密度

```python
material.setDensity(7.85e-06)
```

设置材料密度：
- 参数：密度（浮点数）

### 4. 设置弹性模量

```python
material.setYoungsModulus(210)
```

设置材料弹性模量：
- 参数：弹性模量（浮点数）

### 5. 设置泊松比

```python
material.setPoissonsRatio(0.3)
```

设置材料泊松比：
- 参数：泊松比（浮点数）

### 6. 设置屈服强度

```python
material.setYieldStress(0.5)
```

设置材料屈服强度：
- 参数：屈服强度（浮点数）

### 7. 设置塑性硬化参数

```python
material.setPlasticHardening(0.5)
```

设置塑性硬化参数：
- 参数：塑性硬化参数（浮点数）

### 8. 设置塑性硬化指数

```python
material.setPlasticHardeningExponent(0.5)
```

设置塑性硬化指数：
- 参数：塑性硬化指数（浮点数）

### 9. 设置断裂应变

```python
material.setFailureStrain(0)
```

设置材料断裂应变：
- 参数：断裂应变（浮点数）

### 10. 设置最大应力

```python
material.setMaxStress(0)
```

设置材料最大应力：
- 参数：最大应力（浮点数）

### 11. 设置极限拉伸应力

```python
material.setUTS(0)
```

设置材料极限拉伸应力：
- 参数：极限拉伸应力（浮点数）

### 12. 设置极限拉伸应力对应应变

```python
material.setEpsilonUTS(1)
```

设置极限拉伸应力对应应变：
- 参数：应变值（浮点数）

### 13. 设置应变率系数

```python
material.setStrainRateCoefficient(0)
```

设置应变率系数：
- 参数：应变率系数（浮点数）

### 14. 设置参考应变率

```python
material.setReferenceStrainRate(0)
```

设置参考应变率：
- 参数：参考应变率（浮点数）

### 15. 设置应变率计算标记

```python
material.setStrainRateComputation(0)
```

设置应变率计算标记：
- 参数：标记值（整数）

### 16. 设置应变率平滑标记

```python
material.setStrainRateSmoothing(0)
```

设置应变率平滑标记：
- 参数：标记值（整数）

### 17. 设置应变率平滑截止频率

```python
material.setCutoffFreq(0)
```

设置应变率平滑截止频率：
- 参数：截止频率（浮点数）

### 18. 设置硬化系数

```python
material.setChard(0)
```

设置硬化系数：
- 参数：硬化系数（浮点数）

### 19. 设置温度指数

```python
material.setTempExponent(0)
```

设置温度指数：
- 参数：温度指数（浮点数）

### 20. 设置熔化温度

```python
material.setMeltingTemperature(0)
```

设置熔化温度：
- 参数：熔化温度（浮点数）

### 21. 设置比热容

```python
material.setRhoCp(0)
```

设置比热容：
- 参数：比热容（浮点数）

### 22. 设置参考温度

```python
material.setTref(0)
```

设置参考温度：
- 参数：参考温度（浮点数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| PlasJohns | - | 创建PlasJohns对象 |
| PlasJohns.GetPlasJohns | string | 根据名称获取已存在的PlasJohns材料对象 |
| createMaterial | string | 创建指定名称的PlasJohns材料 |
| setName | string | 修改材料名称 |
| setType | int | 设置材料子类型 |
| setDensity | double | 设置密度 |
| setYoungsModulus | double | 设置弹性模量 |
| setPoissonsRatio | double | 设置泊松比 |
| setYieldStress | double | 设置屈服强度 |
| setPlasticHardening | double | 设置塑性硬化参数 |
| setPlasticHardeningExponent | double | 设置塑性硬化指数 |
| setFailureStrain | double | 设置断裂应变 |
| setMaxStress | double | 设置最大应力 |
| setUTS | double | 设置极限拉伸应力 |
| setEpsilonUTS | double | 设置极限拉伸应力对应应变 |
| setStrainRateCoefficient | double | 设置应变率系数 |
| setReferenceStrainRate | double | 设置参考应变率 |
| setStrainRateComputation | int | 设置应变率计算标记 |
| setStrainRateSmoothing | int | 设置应变率平滑标记 |
| setCutoffFreq | double | 设置应变率平滑截止频率 |
| setChard | double | 设置硬化系数 |
| setTempExponent | double | 设置温度指数 |
| setMeltingTemperature | double | 设置熔化温度 |
| setRhoCp | double | 设置比热容 |
| setTref | double | 设置参考温度 |

## 注意事项

1. 创建前需保证当前存在有效的材料管理环境。
2. 同名PlasJohns材料已存在时，`createMaterial` 会失败。
3. 编辑前建议先通过 `Material.PlasJohns.GetPlasJohns` 获取对象。
4. 编辑已有材料时，建议先通过 `AI.AI_Properties().getMaterialType('材料名')` 确认对象类型。
5. 材料子类型 `0` 和 `1` 分别表示 `CLASSIC` 与 `SIMPLE`。
6. 建议先设置基础参数，再设置应变率和温度相关参数。
