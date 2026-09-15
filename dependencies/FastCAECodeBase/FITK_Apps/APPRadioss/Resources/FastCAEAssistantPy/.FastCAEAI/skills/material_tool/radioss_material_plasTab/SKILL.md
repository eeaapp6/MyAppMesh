# Radioss PlasTab材料

## 功能描述

本技能用于在Radioss中编辑已有PlasTab材料或创建，支持设置材料名称、密度、弹性模量、泊松比、失效参数、函数数量、屈服应力函数列表、应变率相关参数以及若干附加控制参数。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行PlasTab材料配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建PlasTab材料

### 1. 创建PlasTab材料

创建PlasTab材料操作对象后，可以创建PlasTab材料。

```python
material=Material.PlasTab()
material.createMaterial('Material-1')
```

- 参数：材料名称（字符串）
- 使用指定名称创建PlasTab材料
- 注意：如果已存在同名材料会创建失败

**完整示例：创建PlasTab材料并设置部分参数**

```python
material=Material.PlasTab()
material.createMaterial('Material-1')
material.setDensity(7.85e-06)
material.setYoungsModulus(210)
material.setPoissonsRatio(0.3)
material.setFailureStrain(0)
material.setTensileFailureStrain(0)
material.setMaxTensileFailureStrain(0)
material.setNumberOfFunctions(2)
material.setFunction(1, 'Curve-1')
material.setFctScale(1, 1.0)
material.setEpsilon(1, 0.0)
material.setFunction(2, 'Curve-2')
material.setFctScale(2, 1.0)
material.setEpsilon(2, 1.0)
material.setSmoothStrainRateOptFlag(0)
material.setChard(0)
material.setFcut(0)
material.setVpFlag(0)
material.setTensileStrainForElementDeletion(0)
material.setFctIDp(0)
material.setFscaleFactor(1.0)
material.setFctIODE(0)
material.setEint(0)
material.setCe(0)
```

## 修改PlasTab材料

### 1. 获取PlasTab材料对象

根据材料名称获取已存在的PlasTab材料对象：
- 参数：材料名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getMaterialType('Material-1')` 判断对象是否为 `PlasTab` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
material=Material.PlasTab.GetPlasTab('Material-1')
```

## 修改PlasTab材料参数

获取对象后，可修改以下属性：

### 1. 修改材料名称

```python
material.setName('Material-2')
```

设置PlasTab材料的新名称：
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

### 5. 设置失效应变

```python
material.setFailureStrain(0)
```

设置失效应变：
- 参数：失效应变（浮点数）

### 6. 设置拉伸失效应变

```python
material.setTensileFailureStrain(0)
```

设置拉伸失效应变：
- 参数：拉伸失效应变（浮点数）

### 7. 设置最大拉伸失效应变

```python
material.setMaxTensileFailureStrain(0)
```

设置最大拉伸失效应变：
- 参数：最大拉伸失效应变（浮点数）

### 8. 设置函数数量

```python
material.setNumberOfFunctions(2)
```

设置函数数量：
- 参数：函数数量（整数）
- 建议先设置该值，再设置各个函数对应的曲线、缩放因子和应变率

### 9. 设置屈服应力函数曲线

```python
material.setFunction(1, 'Curve-1')
```

设置指定序号的屈服应力函数曲线：
- 参数1：列表索引（整数，从1开始）
- 参数2：曲线名称（字符串）

### 10. 设置函数缩放因子

```python
material.setFctScale(1, 1.0)
```

设置指定序号的函数缩放因子：
- 参数1：列表索引（整数，从1开始）
- 参数2：缩放因子（浮点数）

### 11. 设置应变率参数

```python
material.setEpsilon(1, 0.0)
```

设置指定序号的应变率参数：
- 参数1：列表索引（整数，从1开始）
- 参数2：应变率值（浮点数）

### 12. 设置平滑应变率选项标志

```python
material.setSmoothStrainRateOptFlag(0)
```

设置平滑应变率选项标志：
- 参数：标志值（整数）

### 13. 设置硬化系数

```python
material.setChard(0)
```

设置硬化系数：
- 参数：硬化系数（浮点数）

### 14. 设置应变率平滑截止频率

```python
material.setFcut(0)
```

设置应变率平滑截止频率：
- 参数：截止频率（浮点数）

### 15. 设置应变率选择标志

```python
material.setVpFlag(0)
```

设置应变率选择标志：
- 参数：标志值（整数）

### 16. 设置元素删除拉伸应变

```python
material.setTensileStrainForElementDeletion(0)
```

设置元素删除拉伸应变：
- 参数：拉伸应变（浮点数）

### 17. 设置压力屈服因子函数 ID

```python
material.setFctIDp(0)
```

设置压力屈服因子函数 ID：
- 参数：函数 ID（整数）

### 18. 设置压力屈服因子缩放因子

```python
material.setFscaleFactor(1.0)
```

设置压力屈服因子缩放因子：
- 参数：缩放因子（浮点数）

### 19. 设置杨氏模量函数标识

```python
material.setFctIODE(0)
```

设置杨氏模量函数标识：
- 参数：函数标识（整数）

### 20. 设置饱和杨氏模量

```python
material.setEint(0)
```

设置饱和杨氏模量：
- 参数：杨氏模量（浮点数）

### 21. 设置杨氏模量演化参数

```python
material.setCe(0)
```

设置杨氏模量演化参数：
- 参数：演化参数（浮点数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| PlasTab | - | 创建PlasTab对象 |
| PlasTab.GetPlasTab | string | 根据名称获取已存在的PlasTab材料对象 |
| createMaterial | string | 创建指定名称的PlasTab材料 |
| setName | string | 修改材料名称 |
| setDensity | double | 设置密度 |
| setYoungsModulus | double | 设置弹性模量 |
| setPoissonsRatio | double | 设置泊松比 |
| setFailureStrain | double | 设置失效应变 |
| setTensileFailureStrain | double | 设置拉伸失效应变 |
| setMaxTensileFailureStrain | double | 设置最大拉伸失效应变 |
| setNumberOfFunctions | int | 设置函数数量 |
| setFunction | int, string | 设置指定序号的曲线名称 |
| setFctScale | int, double | 设置指定序号的函数缩放因子 |
| setEpsilon | int, double | 设置指定序号的应变率参数 |
| setSmoothStrainRateOptFlag | int | 设置平滑应变率选项标志 |
| setChard | double | 设置硬化系数 |
| setFcut | double | 设置应变率平滑截止频率 |
| setVpFlag | int | 设置应变率选择标志 |
| setTensileStrainForElementDeletion | double | 设置元素删除拉伸应变 |
| setFctIDp | int | 设置压力屈服因子函数 ID |
| setFscaleFactor | double | 设置压力屈服因子缩放因子 |
| setFctIODE | int | 设置杨氏模量函数标识 |
| setEint | double | 设置饱和杨氏模量 |
| setCe | double | 设置杨氏模量演化参数 |

## 注意事项

1. 创建前需保证当前存在有效的材料管理环境。
2. 同名PlasTab材料已存在时，`createMaterial` 会失败。
3. 编辑前建议先通过 `Material.PlasTab.GetPlasTab` 获取对象。
4. 编辑已有材料时，建议先通过 `AI.AI_Properties().getMaterialType('材料名')` 确认对象类型。
5. 使用 `setFunction` 时，必须先通过 `setNumberOfFunctions` 分配函数槽位，否则索引会越界。
6. `setFunction`、`setFctScale`、`setEpsilon` 的索引从 1 开始，不是从 0 开始。
7. `setFunction` 使用的是曲线名称，不是曲线 ID；对应曲线必须已存在于当前 Radioss Case 中。
8. `setDensity`、`setYoungsModulus`、`setFailureStrain`、`setTensileFailureStrain`、`setMaxTensileFailureStrain`、`setChard`、`setFcut`、`setTensileStrainForElementDeletion`、`setFscaleFactor`、`setEint`、`setCe` 的参数建议结合实际材料模型取合理非负值。
9. `setPoissonsRatio` 的取值范围应满足大于等于0且小于0.5。
