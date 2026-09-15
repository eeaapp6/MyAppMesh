# Radioss SandwichShell属性

## 功能描述

本技能用于在Radioss中创建和修改SandwichShell属性，支持设置属性名称、壳单元公式、阻尼、层数、应变选项、厚度、剪切因子、厚度应力、塑性、局部方向、铺层位置以及各层的角度、厚度、位置、材料和失效权重。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行SandwichShell属性配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建SandwichShell属性

### 1. 创建SandwichShell属性

创建SandwichShell属性操作对象后，可以创建SandwichShell属性。

```python
property=Property.SandwichShell()
property.createProperty('Property-1')
```

- 参数：属性名称（字符串）
- 使用指定名称创建SandwichShell属性
- 注意：如果已存在同名属性会创建失败

**完整示例：创建SandwichShell属性并设置部分参数**

```python
property=Property.SandwichShell()
property.createProperty('Property-1')
property.setThick(1.8)
property.setIshell(24)
property.setIsmstr(0)
property.setIsh3n(0)
property.setIdril(0)
property.setPthickFail(0)
property.setHm(0)
property.setHf(0)
property.setHr(0)
property.setDm(0)
property.setDn(0)
property.setNumOfLayers(2)
property.setIstrain(0)
property.setAshear(0)
property.setIthick(0)
property.setIplas(0)
property.setVx(1.0)
property.setVy(0.0)
property.setVz(0.0)
property.setIorth(0)
property.setIpos(0)
property.setLayerAngle(1, 0.0)
property.setLayerThickness(1, 0.9)
property.setLayerPositionZ(1, -0.45)
property.setLayerMaterial(1, 'Material-1')
property.setLayerFailureWeight(1, 1.0)
property.setLayerAngle(2, 0.0)
property.setLayerThickness(2, 0.9)
property.setLayerPositionZ(2, 0.45)
property.setLayerMaterial(2, 'Material-1')
property.setLayerFailureWeight(2, 1.0)
```

## 修改SandwichShell属性的参数

### 1. 获取SandwichShell属性对象

根据属性名称获取已存在的SandwichShell属性对象：
- 参数：属性名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getPropertyType('Property-1')` 判断对象是否为 `Sandwich` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
property=Property.SandwichShell.GetSandwichShell('Property-1')
```

## 修改SandwichShell属性参数

获取对象后，可修改以下属性：

### 1. 修改属性名称

```python
property.setName('Property-2')
```

设置SandwichShell属性的新名称：
- 参数：属性名称（字符串）

### 2. 设置壳单元公式标志

```python
property.setIshell(24)
```

设置壳单元公式标志：
- 参数：参数值（整数）

### 3. 设置壳小应变公式标志

```python
property.setIsmstr(0)
```

设置壳小应变公式标志：
- 参数：参数值（整数）

### 4. 设置三节点壳单元公式标志

```python
property.setIsh3n(0)
```

设置三节点壳单元公式标志：
- 参数：参数值（整数）

### 5. 设置钻转自由度刚度标志

```python
property.setIdril(0)
```

设置钻转自由度刚度标志：
- 参数：参数值（整数）

### 6. 设置厚度失效比例

```python
property.setPthickFail(0)
```

设置厚度失效比例：
- 参数：参数值（浮点数）

### 7. 设置膜向沙漏系数

```python
property.setHm(0)
```

设置膜向沙漏系数：
- 参数：参数值（浮点数）

### 8. 设置离面沙漏系数

```python
property.setHf(0)
```

设置离面沙漏系数：
- 参数：参数值（浮点数）

### 9. 设置转动沙漏系数

```python
property.setHr(0)
```

设置转动沙漏系数：
- 参数：参数值（浮点数）

### 10. 设置膜向阻尼

```python
property.setDm(0)
```

设置膜向阻尼：
- 参数：参数值（浮点数）

### 11. 设置数值阻尼

```python
property.setDn(0)
```

设置数值阻尼：
- 参数：参数值（浮点数）

### 12. 设置层数

```python
property.setNumOfLayers(2)
```

设置层数：
- 参数：参数值（整数）
- 建议先设置层数，再设置逐层参数

### 13. 设置应变选项

```python
property.setIstrain(0)
```

设置应变选项：
- 参数：参数值（整数）

### 14. 设置总厚度

```python
property.setThick(1.8)
```

设置总厚度：
- 参数：参数值（浮点数）

### 15. 设置剪切因子

```python
property.setAshear(0)
```

设置剪切因子：
- 参数：参数值（浮点数）

### 16. 设置厚度应力计算标志

```python
property.setIthick(0)
```

设置厚度应力计算标志：
- 参数：参数值（整数）

### 17. 设置平面应力塑性标志

```python
property.setIplas(0)
```

设置平面应力塑性标志：
- 参数：参数值（整数）

### 18. 设置局部方向 Vx

```python
property.setVx(1.0)
```

设置局部方向 Vx：
- 参数：参数值（浮点数）

### 19. 设置局部方向 Vy

```python
property.setVy(0.0)
```

设置局部方向 Vy：
- 参数：参数值（浮点数）

### 20. 设置局部方向 Vz

```python
property.setVz(0.0)
```

设置局部方向 Vz：
- 参数：参数值（浮点数）

### 21. 设置正交选项

```python
property.setIorth(0)
```

设置正交选项：
- 参数：参数值（整数）

### 22. 设置位置选项

```python
property.setIpos(0)
```

设置位置选项：
- 参数：布尔值或 0/1

### 23. 设置层角度

```python
property.setLayerAngle(1, 0.0)
```

设置指定层的角度：
- 参数1：层索引（整数，从1开始）
- 参数2：角度值（浮点数）

### 24. 设置层厚度

```python
property.setLayerThickness(1, 0.9)
```

设置指定层的厚度：
- 参数1：层索引（整数，从1开始）
- 参数2：厚度值（浮点数）

### 25. 设置层位置 Z

```python
property.setLayerPositionZ(1, -0.45)
```

设置指定层的位置 Z：
- 参数1：层索引（整数，从1开始）
- 参数2：位置值（浮点数）

### 26. 设置层材料

```python
property.setLayerMaterial(1, 'Material-1')
```

设置指定层的材料：
- 参数1：层索引（整数，从1开始）
- 参数2：材料名称（字符串）

### 27. 设置层失效权重

```python
property.setLayerFailureWeight(1, 1.0)
```

设置指定层的失效权重：
- 参数1：层索引（整数，从1开始）
- 参数2：权重值（浮点数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| SandwichShell | - | 创建SandwichShell对象 |
| SandwichShell.GetSandwichShell | string | 根据名称获取已存在的SandwichShell属性对象 |
| createProperty | string | 创建指定名称的SandwichShell属性 |
| setName | string | 修改属性名称 |
| setIshell | int | 设置壳单元公式标志 |
| setIsmstr | int | 设置壳小应变公式标志 |
| setIsh3n | int | 设置三节点壳单元公式标志 |
| setIdril | int | 设置钻转自由度刚度标志 |
| setPthickFail | double | 设置厚度失效比例 |
| setHm | double | 设置膜向沙漏系数 |
| setHf | double | 设置离面沙漏系数 |
| setHr | double | 设置转动沙漏系数 |
| setDm | double | 设置膜向阻尼 |
| setDn | double | 设置数值阻尼 |
| setNumOfLayers | int | 设置层数 |
| setIstrain | int | 设置应变选项 |
| setThick | double | 设置总厚度 |
| setAshear | double | 设置剪切因子 |
| setIthick | int | 设置厚度应力计算标志 |
| setIplas | int | 设置平面应力塑性标志 |
| setVx | double | 设置局部方向 Vx |
| setVy | double | 设置局部方向 Vy |
| setVz | double | 设置局部方向 Vz |
| setIorth | int | 设置正交选项 |
| setIpos | bool | 设置位置选项 |
| setLayerAngle | int, double | 设置指定层的角度 |
| setLayerThickness | int, double | 设置指定层的厚度 |
| setLayerPositionZ | int, double | 设置指定层的位置 Z |
| setLayerMaterial | int, string | 设置指定层的材料名称 |
| setLayerFailureWeight | int, double | 设置指定层的失效权重 |

## 注意事项

1. 创建前需保证当前存在有效的属性管理环境。
2. 同名SandwichShell属性已存在时，`createProperty` 会失败。
3. 编辑前建议先通过 `Property.SandwichShell.GetSandwichShell` 获取对象。
4. 编辑已有属性时，建议先通过 `AI.AI_Properties().getPropertyType('属性名')` 确认对象类型。
5. 示例脚本按当前界面 `writePythonScript` 的实际输出顺序组织，但未包含 `setSkewID`，因为当前 Python 包装类尚未暴露该接口。
6. 设置逐层参数前，必须先通过 `setNumOfLayers` 分配层数，否则层索引会越界。
7. `setLayerAngle`、`setLayerThickness`、`setLayerPositionZ`、`setLayerMaterial`、`setLayerFailureWeight` 的层索引从 1 开始。
8. `setLayerMaterial` 使用的是材料名称，不是材料 ID；对应材料必须已存在于当前算例中。
9. `setIpos` 在脚本中既可传布尔值，也可按界面脚本写法传 0/1。
