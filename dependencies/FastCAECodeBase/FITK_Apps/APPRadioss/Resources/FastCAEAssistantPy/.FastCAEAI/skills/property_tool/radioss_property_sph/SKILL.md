# Radioss SPH属性

## 功能描述

本技能用于在Radioss中创建和修改SPH属性，支持设置属性名称、粒子质量、体积粘性参数、保守平滑系数、平滑长度变化选项、SPH修正阶次、平滑长度以及抗拉不稳定系数。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行SPH属性配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建SPH属性

### 1. 创建SPH属性

创建SPH属性操作对象后，可以创建SPH属性。

```python
property=Property.SPH()
property.createProperty('Property-1')
```

- 参数：属性名称（字符串）
- 使用指定名称创建SPH属性
- 注意：如果已存在同名属性会创建失败

**完整示例：创建SPH属性并设置部分参数**

```python
property=Property.SPH()
property.createProperty('Property-1')
property.setMP(1.0)
property.setQA(0.0)
property.setQB(0.0)
property.setACS(0.0)
property.setHID(0)
property.setOrderID(0)
property.setH(0.0)
property.setStab(0.0)
```

## 修改SPH属性的参数

### 1. 获取SPH属性对象

根据属性名称获取已存在的SPH属性对象：
- 参数：属性名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getPropertyType('Property-1')` 判断对象是否为 `SPH` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
property=Property.SPH.GetSPH('Property-1')
```

## 修改SPH属性参数

获取对象后，可修改以下属性：

### 1. 修改属性名称

```python
property.setName('Property-2')
```

设置SPH属性的新名称：
- 参数：属性名称（字符串）

### 2. 设置粒子质量

```python
property.setMP(1.0)
```

设置粒子质量：
- 参数：参数值（浮点数）

### 3. 设置二次体积粘性

```python
property.setQA(0.0)
```

设置二次体积粘性：
- 参数：参数值（浮点数）

### 4. 设置线性体积粘性

```python
property.setQB(0.0)
```

设置线性体积粘性：
- 参数：参数值（浮点数）

### 5. 设置保守平滑系数

```python
property.setACS(0.0)
```

设置保守平滑系数：
- 参数：参数值（浮点数）

### 6. 设置平滑长度变化选项

```python
property.setHID(0)
```

设置平滑长度变化选项：
- 参数：参数值（整数）

### 7. 设置SPH修正阶次

```python
property.setOrderID(0)
```

设置SPH修正阶次：
- 参数：参数值（整数）

### 8. 设置平滑长度

```python
property.setH(0.0)
```

设置平滑长度：
- 参数：参数值（浮点数）

### 9. 设置抗拉不稳定系数

```python
property.setStab(0.0)
```

设置抗拉不稳定系数：
- 参数：参数值（浮点数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| SPH | - | 创建SPH对象 |
| SPH.GetSPH | string | 根据名称获取已存在的SPH属性对象 |
| createProperty | string | 创建指定名称的SPH属性 |
| setName | string | 修改属性名称 |
| setMP | double | 设置粒子质量 |
| setQA | double | 设置二次体积粘性 |
| setQB | double | 设置线性体积粘性 |
| setACS | double | 设置保守平滑系数 |
| setHID | int | 设置平滑长度变化选项 |
| setOrderID | int | 设置SPH修正阶次 |
| setH | double | 设置平滑长度 |
| setStab | double | 设置抗拉不稳定系数 |

## 注意事项

1. 创建前需保证当前存在有效的属性管理环境。
2. 同名SPH属性已存在时，`createProperty` 会失败。
3. 编辑前建议先通过 `Property.SPH.GetSPH` 获取对象。
4. 编辑已有属性时，建议先通过 `AI.AI_Properties().getPropertyType('属性名')` 确认对象类型。
5. 示例脚本按当前界面 `writePythonScript` 的实际输出顺序组织。
6. `setMP`、`setQA`、`setQB`、`setACS`、`setH`、`setStab` 的取值应结合 SPH 模型与分析工况设置。
7. `setHID` 和 `setOrderID` 为离散选项值，建议与界面选项含义保持一致。
