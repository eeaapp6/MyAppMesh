# Radioss Solid属性

## 功能描述

本技能用于在Radioss中创建和修改Solid属性，支持设置属性名称、实体单元公式、小应变公式、接触压力公式、四面体公式、单元坐标系、阻尼、数值粘性、最小时间步长以及方向数等参数。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行Solid属性配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建Solid属性

### 1. 创建Solid属性

创建Solid属性操作对象后，可以创建Solid属性。

```python
property=Property.Solid()
property.createProperty('Property-1')
```

- 参数：属性名称（字符串）
- 使用指定名称创建Solid属性
- 注意：如果已存在同名属性会创建失败

**完整示例：创建Solid属性并设置部分参数**

```python
property=Property.Solid()
property.createProperty('Property-1')
property.setIsolid(0)
property.setIsmstr(0)
property.setIcpre(0)
property.setItetra10(0)
property.setItetra4(0)
property.setIFrame(0)
property.setDn(0)
property.setMuV(0)
property.setMinTimeStep(0)
property.setNdir(1)
```

## 修改Solid属性的参数

### 1. 获取Solid属性对象

根据属性名称获取已存在的Solid属性对象：
- 参数：属性名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getPropertyType('Property-1')` 判断对象是否为 `Solid` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
property=Property.Solid.GetSolid('Property-1')
```

## 修改Solid属性参数

获取对象后，可修改以下属性：

### 1. 修改属性名称

```python
property.setName('Property-2')
```

设置Solid属性的新名称：
- 参数：属性名称（字符串）

### 2. 设置稳定化数值阻尼

```python
property.setDn(0)
```

设置稳定化数值阻尼：
- 参数：参数值（浮点数）

### 3. 设置二次体积粘性

```python
property.setQa(0)
```

设置二次体积粘性：
- 参数：参数值（浮点数）

### 4. 设置线性体积粘性

```python
property.setQb(0)
```

设置线性体积粘性：
- 参数：参数值（浮点数）

### 5. 设置沙漏粘性系数

```python
property.setH(0)
```

设置沙漏粘性系数：
- 参数：参数值（浮点数）

### 6. 设置数值粘性 LambdaV

```python
property.setLambdaV(0)
```

设置数值粘性 LambdaV：
- 参数：参数值（浮点数）

### 7. 设置数值粘性 MuV

```python
property.setMuV(0)
```

设置数值粘性 MuV：
- 参数：参数值（浮点数）

### 8. 设置最小时间步长

```python
property.setMinTimeStep(0)
```

设置最小时间步长：
- 参数：参数值（浮点数）

### 9. 设置每个实体单元的方向数

```python
property.setNdir(1)
```

设置每个实体单元的方向数：
- 参数：参数值（整数）

### 10. 设置实体单元公式标志

```python
property.setIsolid(0)
```

设置实体单元公式标志：
- 参数：参数值（整数）

### 11. 设置小应变公式标志

```python
property.setIsmstr(0)
```

设置小应变公式标志：
- 参数：参数值（整数）

### 12. 设置接触压力公式标志

```python
property.setIcpre(0)
```

设置接触压力公式标志：
- 参数：参数值（整数）

### 13. 设置四节点四面体公式标志

```python
property.setItetra4(0)
```

设置四节点四面体公式标志：
- 参数：参数值（整数）

### 14. 设置十节点四面体公式标志

```python
property.setItetra10(0)
```

设置十节点四面体公式标志：
- 参数：参数值（整数）

### 15. 设置节点质量分配标志

```python
property.setImas(0)
```

设置节点质量分配标志：
- 参数：参数值（整数）

### 16. 设置单元坐标系公式标志

```python
property.setIFrame(0)
```

设置单元坐标系公式标志：
- 参数：参数值（整数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| Solid | - | 创建Solid对象 |
| Solid.GetSolid | string | 根据名称获取已存在的Solid属性对象 |
| createProperty | string | 创建指定名称的Solid属性 |
| setName | string | 修改属性名称 |
| setDn | double | 设置稳定化数值阻尼 |
| setQa | double | 设置二次体积粘性 |
| setQb | double | 设置线性体积粘性 |
| setH | double | 设置沙漏粘性系数 |
| setLambdaV | double | 设置数值粘性 LambdaV |
| setMuV | double | 设置数值粘性 MuV |
| setMinTimeStep | double | 设置最小时间步长 |
| setNdir | int | 设置每个实体单元的方向数 |
| setIsolid | int | 设置实体单元公式标志 |
| setIsmstr | int | 设置小应变公式标志 |
| setIcpre | int | 设置接触压力公式标志 |
| setItetra4 | int | 设置四节点四面体公式标志 |
| setItetra10 | int | 设置十节点四面体公式标志 |
| setImas | int | 设置节点质量分配标志 |
| setIFrame | int | 设置单元坐标系公式标志 |

## 注意事项

1. 创建前需保证当前存在有效的属性管理环境。
2. 同名Solid属性已存在时，`createProperty` 会失败。
3. 编辑前建议先通过 `Property.Solid.GetSolid` 获取对象。
4. 编辑已有属性时，建议先通过 `AI.AI_Properties().getPropertyType('属性名')` 确认对象类型。
5. 示例脚本按当前界面 `writePythonScript` 的实际输出顺序组织，优先覆盖界面直接写出的参数。
6. `setNdir` 在界面脚本中通常从 1 开始设置，使用时应与界面含义保持一致。
7. 体积粘性、数值粘性、最小时间步长等参数应结合分析工况设置，避免不合理取值。
