# Radioss PlasBrit材料

## 功能描述

本技能用于在Radioss中编辑已有PlasBrit材料或创建，支持设置材料名称、密度、弹性模量、泊松比、塑性参数、应变率参数以及两个方向上的拉伸失效相关参数。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行PlasBrit材料配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建PlasBrit材料

### 1. 创建PlasBrit材料

创建PlasBrit材料操作对象后，可以创建PlasBrit材料。

```python
material=Material.PlasBrit()
material.createMaterial('Material-1')
```

- 参数：材料名称（字符串）
- 使用指定名称创建PlasBrit材料
- 注意：如果已存在同名材料会创建失败

**完整示例：创建PlasBrit材料并设置部分参数**

```python
material=Material.PlasBrit()
material.createMaterial('Material-1')
material.setDensity(7.85e-06)
material.setYoungsModulus(210)
material.setPoissonsRatio(0.3)
material.setPlasticityYieldStress(0.5)
material.setPlasticityHardeningParameter(0.5)
material.setPlasticityHardeningExponent(0.5)
material.setPlasticityMaximumStress(0.8)
```

## 修改PlasBrit材料

### 1. 获取PlasBrit材料对象

根据材料名称获取已存在的PlasBrit材料对象：
- 参数：材料名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getMaterialType('Material-1')` 判断对象是否为 `PlasBrit` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
material=Material.PlasBrit.GetPlasBrit('Material-1')
```

## 修改PlasBrit材料参数

获取对象后，可修改以下属性：

### 1. 修改材料名称

```python
material.setName('Material-2')
```

设置PlasBrit材料的新名称：
- 参数：材料名称（字符串）

### 2. 设置密度

```python
material.setDensity(7.85e-06)
```

设置材料密度：
- 参数：密度（浮点数）

### 3. 设置弹性模量

```python
material.setYoungsModulus(210)
```

设置材料弹性模量：
- 参数：弹性模量（浮点数）

### 4. 设置泊松比

```python
material.setPoissonsRatio(0.3)
```

设置材料泊松比：
- 参数：泊松比（浮点数）

### 5. 设置塑性屈服应力

```python
material.setPlasticityYieldStress(0.5)
```

设置塑性屈服应力：
- 参数：屈服应力（浮点数）

### 6. 设置塑性硬化参数

```python
material.setPlasticityHardeningParameter(0.5)
```

设置塑性硬化参数：
- 参数：塑性硬化参数（浮点数）

### 7. 设置塑性硬化指数

```python
material.setPlasticityHardeningExponent(0.5)
```

设置塑性硬化指数：
- 参数：塑性硬化指数（浮点数）

### 8. 设置塑性最大应力

```python
material.setPlasticityMaximumStress(0.8)
```

设置塑性最大应力：
- 参数：最大应力（浮点数）

### 9. 设置应变率系数

```python
material.setStrainRateCoefficient(0)
```

设置应变率系数：
- 参数：应变率系数（浮点数）

### 10. 设置参考应变率

```python
material.setReferenceStrainRate(0)
```

设置参考应变率：
- 参数：参考应变率（浮点数）

### 11. 设置应变率计算标记

```python
material.setStrainRateComputation(0)
```

设置应变率计算标记：
- 参数：标记值（整数）

### 12. 设置应变率平滑标记

```python
material.setStrainRateSmoothing(0)
```

设置应变率平滑标记：
- 参数：标记值（整数）

### 13. 设置应变率平滑截止频率

```python
material.setCutoffFreq(0)
```

设置应变率平滑截止频率：
- 参数：截止频率（浮点数）

### 14. 设置方向1拉伸破坏应变

```python
material.setTensileFilureStrain1(0)
```

设置方向1拉伸破坏应变：
- 参数：应变值（浮点数）

### 15. 设置方向1最大拉伸破坏应变

```python
material.setMaximumTensileFilureStrain1(0)
```

设置方向1最大拉伸破坏应变：
- 参数：应变值（浮点数）

### 16. 设置方向1最大损伤因子

```python
material.setMaximumDamageFactor1(0)
```

设置方向1最大损伤因子：
- 参数：损伤因子（浮点数）

### 17. 设置方向1最大拉伸应变

```python
material.setMaximumTensileStrain1(0)
```

设置方向1最大拉伸应变：
- 参数：应变值（浮点数）

### 18. 设置方向2拉伸破坏应变

```python
material.setTensileFilureStrain2(0)
```

设置方向2拉伸破坏应变：
- 参数：应变值（浮点数）

### 19. 设置方向2最大拉伸破坏应变

```python
material.setMaximumTensileFilureStrain2(0)
```

设置方向2最大拉伸破坏应变：
- 参数：应变值（浮点数）

### 20. 设置方向2最大损伤因子

```python
material.setMaximumDamageFactor2(0)
```

设置方向2最大损伤因子：
- 参数：损伤因子（浮点数）

### 21. 设置方向2最大拉伸应变

```python
material.setMaximumTensileStrain2(0)
```

设置方向2最大拉伸应变：
- 参数：应变值（浮点数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| PlasBrit | - | 创建PlasBrit对象 |
| PlasBrit.GetPlasBrit | string | 根据名称获取已存在的PlasBrit材料对象 |
| createMaterial | string | 创建指定名称的PlasBrit材料 |
| setName | string | 修改材料名称 |
| setDensity | double | 设置密度 |
| setYoungsModulus | double | 设置弹性模量 |
| setPoissonsRatio | double | 设置泊松比 |
| setPlasticityYieldStress | double | 设置塑性屈服应力 |
| setPlasticityHardeningParameter | double | 设置塑性硬化参数 |
| setPlasticityHardeningExponent | double | 设置塑性硬化指数 |
| setPlasticityMaximumStress | double | 设置塑性最大应力 |
| setStrainRateCoefficient | double | 设置应变率系数 |
| setReferenceStrainRate | double | 设置参考应变率 |
| setStrainRateComputation | int | 设置应变率计算标记 |
| setStrainRateSmoothing | int | 设置应变率平滑标记 |
| setCutoffFreq | double | 设置应变率平滑截止频率 |
| setTensileFilureStrain1 | double | 设置方向1拉伸破坏应变 |
| setMaximumTensileFilureStrain1 | double | 设置方向1最大拉伸破坏应变 |
| setMaximumDamageFactor1 | double | 设置方向1最大损伤因子 |
| setMaximumTensileStrain1 | double | 设置方向1最大拉伸应变 |
| setTensileFilureStrain2 | double | 设置方向2拉伸破坏应变 |
| setMaximumTensileFilureStrain2 | double | 设置方向2最大拉伸破坏应变 |
| setMaximumDamageFactor2 | double | 设置方向2最大损伤因子 |
| setMaximumTensileStrain2 | double | 设置方向2最大拉伸应变 |

## 注意事项

1. 创建前需保证当前存在有效的材料管理环境。
2. 同名PlasBrit材料已存在时，`createMaterial` 会失败。
3. 编辑前建议先通过 `Material.PlasBrit.GetPlasBrit` 获取对象。
4. 编辑已有材料时，建议先通过 `AI.AI_Properties().getMaterialType('材料名')` 确认对象类型。
5. `setDensity`、`setYoungsModulus`、`setPlasticityYieldStress`、`setPlasticityHardeningParameter`、`setPlasticityHardeningExponent`、`setPlasticityMaximumStress` 的参数建议为非负数。
6. `setPoissonsRatio` 的取值范围应满足大于等于0且小于0.5。
7. 方向1和方向2的拉伸失效参数应结合实际材料模型含义设置，避免输入不合理的负值。
8. 当前 Python 接口方法名中使用 `Filure` 拼写，应按接口实际名称调用。
