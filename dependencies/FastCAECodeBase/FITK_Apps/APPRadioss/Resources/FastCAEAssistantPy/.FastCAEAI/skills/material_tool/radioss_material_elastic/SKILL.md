# Radioss Elastic材料

## 功能描述

本技能用于在Radioss中编辑已有Elastic材料或创建，支持设置材料名称、密度、弹性模量和泊松比。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行Elastic材料配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建Elastic材料

### 1. 创建Elastic材料

创建Elastic材料操作对象后，可以创建Elastic材料。

```python
material=Material.Elastic()
material.createMaterial('Material-1')
```

- 参数：材料名称（字符串）
- 使用指定名称创建Elastic材料
- 注意：如果已存在同名材料会创建失败

**完整示例：创建Elastic材料并设置部分参数**

```python
material=Material.Elastic()
material.createMaterial('Material-1')
material.setDensity(7.85e-06)
material.setYoungsModulus(210)
material.setPoissonsRatio(0.3)
```

## 修改Elastic材料

### 1. 获取Elastic材料对象

根据材料名称获取已存在的Elastic材料对象：
- 参数：材料名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getMaterialType('Material-1')` 判断对象是否为 `Elastic` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
material=Material.Elastic.GetElastic('Material-1')
```

## 修改Elastic材料参数

获取对象后，可修改以下属性：

### 1. 修改材料名称

```python
material.setName('Material-2')
```

设置Elastic材料的新名称：
- 参数：材料名称（字符串）
- 返回：执行结果信息（字符串）

### 2. 设置密度

```python
material.setDensity(7.85e-06)
```

设置材料密度：
- 参数：密度（浮点数）
- 返回：执行结果信息（字符串）

### 3. 设置弹性模量

```python
material.setYoungsModulus(210)
```

设置材料弹性模量：
- 参数：弹性模量（浮点数）
- 返回：执行结果信息（字符串）

### 4. 设置泊松比

```python
material.setPoissonsRatio(0.3)
```

设置材料泊松比：
- 参数：泊松比（浮点数）
- 返回：执行结果信息（字符串）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| Elastic | - | 创建Elastic对象 |
| Elastic.GetElastic | string | 根据名称获取已存在的Elastic材料对象 |
| createMaterial | string | 创建指定名称的Elastic材料 |
| setName | string | 修改材料名称 |
| setDensity | double | 设置密度 |
| setYoungsModulus | double | 设置弹性模量 |
| setPoissonsRatio | double | 设置泊松比 |

## 注意事项

1. 创建前需保证当前存在有效的材料管理环境。
2. 同名Elastic材料已存在时，`createMaterial` 会失败。
3. 编辑前建议先通过 `Material.Elastic.GetElastic` 获取对象。
4. 编辑已有材料时，建议先通过 `AI.AI_Properties().getMaterialType('材料名')` 确认对象类型。
5. `setDensity` 和 `setYoungsModulus` 的参数应为非负数。
6. `setPoissonsRatio` 的取值范围应满足大于等于0且小于0.5。
7. `createMaterial`、`setName`、`setDensity`、`setYoungsModulus`、`setPoissonsRatio` 会返回执行详细信息，便于脚本中直接判断执行结果。
