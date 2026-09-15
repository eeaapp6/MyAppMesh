# Radioss Hydro材料

## 功能描述

本技能用于在Radioss中编辑已有Hydro材料或创建，支持设置材料名称、密度、参考密度、运动粘度、最小压力截断值以及状态方程。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行Hydro材料配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建Hydro材料

### 1. 创建Hydro材料

创建Hydro材料操作对象后，可以创建Hydro材料。

```python
material=Material.Hydro()
material.createMaterial('Material-1')
```

- 参数：材料名称（字符串）
- 使用指定名称创建Hydro材料
- 注意：如果已存在同名材料会创建失败

**完整示例：创建Hydro材料并设置部分参数**

```python
material=Material.Hydro()
material.createMaterial('Material-1')
material.setDensity(1.0e-06)
material.setReferenceDensity(1.0e-06)
material.setKinematicViscosity(0.0)
material.setPressureCut(0.0)
material.setEOS('EOS-1')
```

## 修改Hydro材料

### 1. 获取Hydro材料对象

根据材料名称获取已存在的Hydro材料对象：
- 参数：材料名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getMaterialType('Material-1')` 判断对象是否为 `Hydro` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
material=Material.Hydro.GetHydro('Material-1')
```

## 修改Hydro材料参数

获取对象后，可修改以下属性：

### 1. 修改材料名称

```python
material.setName('Material-2')
```

设置Hydro材料的新名称：
- 参数：材料名称（字符串）
- 返回：执行结果信息（字符串）

### 2. 设置密度

```python
material.setDensity(1.0e-06)
```

设置材料密度：
- 参数：密度（浮点数）
- 返回：执行结果信息（字符串）

### 3. 设置参考密度

```python
material.setReferenceDensity(1.0e-06)
```

设置参考密度：
- 参数：参考密度（浮点数）
- 返回：执行结果信息（字符串）

### 4. 设置运动粘度

```python
material.setKinematicViscosity(0.0)
```

设置运动粘度：
- 参数：运动粘度（浮点数）
- 返回：执行结果信息（字符串）

### 5. 设置最小压力截断值

```python
material.setPressureCut(0.0)
```

设置最小压力截断值：
- 参数：压力截断值（浮点数）
- 返回：执行结果信息（字符串）

### 6. 设置状态方程

```python
material.setEOS('EOS-1')
```

设置Hydro材料关联的状态方程：
- 参数：状态方程名称（字符串）
- 返回：执行结果信息（字符串）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| Hydro | - | 创建Hydro对象 |
| Hydro.GetHydro | string | 根据名称获取已存在的Hydro材料对象 |
| createMaterial | string | 创建指定名称的Hydro材料 |
| setName | string | 修改材料名称 |
| setDensity | double | 设置密度 |
| setReferenceDensity | double | 设置参考密度 |
| setKinematicViscosity | double | 设置运动粘度 |
| setPressureCut | double | 设置最小压力截断值 |
| setEOS | string | 设置状态方程名称 |

## 注意事项

1. 创建前需保证当前存在有效的材料管理环境。
2. 同名Hydro材料已存在时，`createMaterial` 会失败。
3. 编辑前建议先通过 `Material.Hydro.GetHydro` 获取对象。
4. 编辑已有材料时，建议先通过 `AI.AI_Properties().getMaterialType('材料名')` 确认对象类型。
5. `setDensity`、`setReferenceDensity`、`setKinematicViscosity` 的参数应为非负数。
6. `setPressureCut` 会直接写入材料参数，取值应结合实际工况设置。
7. `setEOS` 使用的是状态方程名称，不是 ID；对应 EOS 必须已存在于当前 Radioss Case 中。
8. `createMaterial`、`setName`、`setDensity`、`setReferenceDensity`、`setKinematicViscosity`、`setPressureCut`、`setEOS` 会返回执行详细信息，便于脚本中直接判断执行结果。
