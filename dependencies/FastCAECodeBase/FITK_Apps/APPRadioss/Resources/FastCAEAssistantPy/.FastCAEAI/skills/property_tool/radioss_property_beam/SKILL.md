# Radioss Beam属性

## 功能描述

本技能用于在Radioss中创建和修改Beam属性，支持设置属性名称、小应变选项、梁膜阻尼、梁弯曲阻尼、横截面面积、截面惯性矩、是否忽略剪切以及两端节点的旋转自由度。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行Beam属性配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建Beam属性

### 1. 创建Beam属性

创建Beam属性操作对象后，可以创建Beam属性。

```python
property=Property.Beam()
property.createProperty('Property-1')
```

- 参数：属性名称（字符串）
- 使用指定名称创建Beam属性
- 注意：如果已存在同名属性会创建失败

**完整示例：创建Beam属性并设置部分参数**

```python
property=Property.Beam()
property.createProperty('Property-1')
property.setCrossSection(1.0)
property.setSmallStrainOption(0)
property.setBeamMembraneDamping(0.0)
property.setBeamFlexuralDamping(0.0)
property.setAreaMomentInertiaIyy(0.0)
property.setAreaMomentInertiaIzz(0.0)
property.setAreaMomentInertiaIxx(0.0)
property.setRotationDOF1('x', 1)
property.setRotationDOF1('y', 0)
property.setRotationDOF1('z', 0)
property.setRotationDOF2('x', 1)
property.setRotationDOF2('y', 0)
property.setRotationDOF2('z', 0)
property.setNeglectsShear(0)
```

## 修改Beam属性的参数

### 1. 获取Beam属性对象

根据属性名称获取已存在的Beam属性对象：
- 参数：属性名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getPropertyType('Property-1')` 判断对象是否为 `Beam` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
property=Property.Beam.GetBeam('Property-1')
```

## 修改Beam属性参数

获取对象后，可修改以下属性：

### 1. 修改属性名称

```python
property.setName('Property-2')
```

设置Beam属性的新名称：
- 参数：属性名称（字符串）

### 2. 设置小应变选项

```python
property.setSmallStrainOption(0)
```

设置小应变选项：
- 参数：参数值（整数）

### 3. 设置梁膜阻尼

```python
property.setBeamMembraneDamping(0.0)
```

设置梁膜阻尼：
- 参数：参数值（浮点数）

### 4. 设置梁弯曲阻尼

```python
property.setBeamFlexuralDamping(0.0)
```

设置梁弯曲阻尼：
- 参数：参数值（浮点数）

### 5. 设置横截面面积

```python
property.setCrossSection(1.0)
```

设置横截面面积：
- 参数：参数值（浮点数）

### 6. 设置截面惯性矩 Iyy

```python
property.setAreaMomentInertiaIyy(0.0)
```

设置截面惯性矩 Iyy：
- 参数：参数值（浮点数）

### 7. 设置截面惯性矩 Izz

```python
property.setAreaMomentInertiaIzz(0.0)
```

设置截面惯性矩 Izz：
- 参数：参数值（浮点数）

### 8. 设置截面惯性矩 Ixx

```python
property.setAreaMomentInertiaIxx(0.0)
```

设置截面惯性矩 Ixx：
- 参数：参数值（浮点数）

### 9. 设置是否忽略剪切

```python
property.setNeglectsShear(0)
```

设置是否忽略剪切：
- 参数：布尔值或 0/1

### 10. 设置节点1旋转自由度

```python
property.setRotationDOF1('x', 1)
```

设置节点1旋转自由度：
- 参数1：方向字符串，支持 `x`、`y`、`z`，不区分大小写
- 参数2：是否启用，支持布尔值或 0/1

### 11. 设置节点2旋转自由度

```python
property.setRotationDOF2('x', 1)
```

设置节点2旋转自由度：
- 参数1：方向字符串，支持 `x`、`y`、`z`，不区分大小写
- 参数2：是否启用，支持布尔值或 0/1

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| Beam | - | 创建Beam对象 |
| Beam.GetBeam | string | 根据名称获取已存在的Beam属性对象 |
| createProperty | string | 创建指定名称的Beam属性 |
| setName | string | 修改属性名称 |
| setSmallStrainOption | int | 设置小应变选项 |
| setBeamMembraneDamping | double | 设置梁膜阻尼 |
| setBeamFlexuralDamping | double | 设置梁弯曲阻尼 |
| setCrossSection | double | 设置横截面面积 |
| setAreaMomentInertiaIyy | double | 设置截面惯性矩 Iyy |
| setAreaMomentInertiaIzz | double | 设置截面惯性矩 Izz |
| setAreaMomentInertiaIxx | double | 设置截面惯性矩 Ixx |
| setNeglectsShear | bool | 设置是否忽略剪切 |
| setRotationDOF1 | string, bool | 设置节点1旋转自由度 |
| setRotationDOF2 | string, bool | 设置节点2旋转自由度 |

## 注意事项

1. 创建前需保证当前存在有效的属性管理环境。
2. 同名Beam属性已存在时，`createProperty` 会失败。
3. 编辑前建议先通过 `Property.Beam.GetBeam` 获取对象。
4. 编辑已有属性时，建议先通过 `AI.AI_Properties().getPropertyType('属性名')` 确认对象类型。
5. 示例脚本按当前界面 `writePythonScript` 的实际输出顺序组织。
6. `setRotationDOF1` 和 `setRotationDOF2` 的方向参数只支持 `x`、`y`、`z`。
7. `setNeglectsShear`、`setRotationDOF1`、`setRotationDOF2` 在脚本中既可传布尔值，也可按界面脚本写法传 0/1。
8. 横截面面积、截面惯性矩和阻尼参数应结合实际梁截面与分析工况设置。
