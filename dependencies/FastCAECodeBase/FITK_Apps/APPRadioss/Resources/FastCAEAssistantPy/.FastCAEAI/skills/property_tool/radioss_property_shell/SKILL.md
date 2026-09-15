# Radioss Shell属性

## 功能描述

本技能用于在Radioss中创建和修改Shell属性，支持设置属性名称、壳厚度、积分点数、剪切因子、壳单元公式标志以及沙漏、阻尼、厚度应力和塑性相关参数。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行Shell属性配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建Shell属性

### 1. 创建Shell属性

创建Shell属性操作对象后，可以创建Shell属性。

```python
property=Property.Shell()
property.createProperty('Property-1')
```

- 参数：属性名称（字符串）
- 使用指定名称创建Shell属性
- 注意：如果已存在同名属性会创建失败

**完整示例：创建Shell属性并设置部分参数**

```python
property=Property.Shell()
property.createProperty('Property-1')
property.setThick(1.8)
property.setIshell(24)
property.setIsmstr(0)
property.setIsh3n(0)
property.setIdril(0)
property.setIpinch(0)
property.setPthickFail(0)
```

## 修改Shell属性的参数

### 1. 获取Shell属性对象

根据属性名称获取已存在的Shell属性对象：
- 参数：属性名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getPropertyType('Property-1')` 判断对象是否为 `Shell` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
property=Property.Shell.GetShell('Property-1')
```

## 修改Shell属性参数

获取对象后，可修改以下属性：

### 1. 修改属性名称

```python
property.setName('Property-2')
```

设置Shell属性的新名称：
- 参数：属性名称（字符串）

### 2. 设置壳厚度

```python
property.setThick(1.8)
```

设置Shell属性壳厚度：
- 参数：厚度值（浮点数）

### 3. 设置壳单元公式标志

```python
property.setIshell(24)
```

设置壳单元公式标志：
- 参数：公式标志（整数）

### 4. 设置壳小应变公式标志

```python
property.setIsmstr(0)
```

设置壳小应变公式标志：
- 参数：公式标志（整数）

### 5. 设置三节点壳单元公式标志

```python
property.setIsh3n(0)
```

设置三节点壳单元公式标志：
- 参数：公式标志（整数）

### 6. 设置钻转自由度刚度标志

```python
property.setIdril(0)
```

设置钻转自由度刚度标志：
- 参数：标志值（整数）

### 7. 设置夹挤自由度标志

```python
property.setIpinch(0)
```

设置夹挤自由度标志：
- 参数：标志值（整数）

### 8. 设置厚度失效比例

```python
property.setPthickFail(0)
```

设置厚度失效比例：
- 参数：比例值（浮点数）

### 9. 设置膜向沙漏系数

```python
property.setHm(0)
```

设置膜向沙漏系数：
- 参数：系数值（浮点数）

### 10. 设置离面沙漏系数

```python
property.setHf(0)
```

设置离面沙漏系数：
- 参数：系数值（浮点数）

### 11. 设置转动沙漏系数

```python
property.setHr(0)
```

设置转动沙漏系数：
- 参数：系数值（浮点数）

### 12. 设置膜向阻尼

```python
property.setDm(0)
```

设置膜向阻尼：
- 参数：阻尼值（浮点数）

### 13. 设置数值阻尼

```python
property.setDn(0)
```

设置数值阻尼：
- 参数：阻尼值（浮点数）

### 14. 设置厚度积分点数

```python
property.setN(0)
```

设置厚度积分点数：
- 参数：积分点数（整数）

### 15. 设置剪切因子

```python
property.setAshear(0)
```

设置剪切因子：
- 参数：剪切因子（浮点数）

### 16. 设置厚度应力计算标志

```python
property.setIthick(0)
```

设置厚度应力计算标志：
- 参数：标志值（整数）

### 17. 设置平面应力塑性标志

```python
property.setIplas(1)
```

设置平面应力塑性标志：
- 参数：标志值（整数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| Shell | - | 创建Shell对象 |
| Shell.GetShell | string | 根据名称获取已存在的Shell属性对象 |
| createProperty | string | 创建指定名称的Shell属性 |
| setName | string | 修改属性名称 |
| setThick | double | 设置壳厚度 |
| setIshell | int | 设置壳单元公式标志 |
| setIsmstr | int | 设置壳小应变公式标志 |
| setIsh3n | int | 设置三节点壳单元公式标志 |
| setIdril | int | 设置钻转自由度刚度标志 |
| setIpinch | int | 设置夹挤自由度标志 |
| setPthickFail | double | 设置厚度失效比例 |
| setHm | double | 设置膜向沙漏系数 |
| setHf | double | 设置离面沙漏系数 |
| setHr | double | 设置转动沙漏系数 |
| setDm | double | 设置膜向阻尼 |
| setDn | double | 设置数值阻尼 |
| setN | int | 设置厚度积分点数 |
| setAshear | double | 设置剪切因子 |
| setIthick | int | 设置厚度应力计算标志 |
| setIplas | int | 设置平面应力塑性标志 |

## 注意事项

1. 创建前需保证当前存在有效的属性管理环境。
2. 同名Shell属性已存在时，`createProperty` 会失败。
3. 编辑前建议先通过 `Property.Shell.GetShell` 获取对象。
4. 编辑已有属性时，建议先通过 `AI.AI_Properties().getPropertyType('属性名')` 确认对象类型。
5. 建议先设置壳厚度和公式相关参数，再设置沙漏与阻尼参数。
6. 厚度积分点数、厚度应力和塑性相关标志应按分析要求配套设置。
