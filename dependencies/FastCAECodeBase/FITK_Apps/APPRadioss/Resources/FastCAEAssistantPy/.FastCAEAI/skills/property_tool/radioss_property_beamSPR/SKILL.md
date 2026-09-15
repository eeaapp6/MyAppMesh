# Radioss BeamSPR属性

## 功能描述

本技能用于在Radioss中创建和修改BeamSPR属性，支持设置属性名称、质量、惯量、失效与速率参数，以及 6 个自由度上的刚度、阻尼、非线性参数、函数曲线、失效阈值和比例系数等参数。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行BeamSPR属性配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建BeamSPR属性

### 1. 创建BeamSPR属性

创建BeamSPR属性操作对象后，可以创建BeamSPR属性。

```python
property=Property.BeamSPR()
property.createProperty('Property-1')
```

- 参数：属性名称（字符串）
- 使用指定名称创建BeamSPR属性
- 注意：如果已存在同名属性会创建失败

**完整示例：创建BeamSPR属性并设置部分参数**

```python
property=Property.BeamSPR()
property.createProperty('Property-1')
property.setMass(0.0)
property.setInertia(0.0)
property.setIsflag(0)
property.setIfail(0)
property.setIleng(0)
property.setIfail2(0)
property.setV0(0.0)
property.setW0(0.0)
property.setFcut(0.0)
property.setFsmooth(0)
property.setK(1, 0.0)
property.setC(1, 0.0)
property.setA(1, 0.0)
property.setB(1, 0.0)
property.setD(1, 0.0)
property.setH(1, 0)
property.setMin(1, 0.0)
property.setMax(1, 0.0)
property.setF(1, 0.0)
property.setE(1, 0.0)
property.setAscale(1, 0.0)
property.setHscale(1, 0.0)
property.setVc(1, 0.0)
property.setN(1, 0.0)
property.setAlpha(1, 0.0)
property.setExponent(1, 0.0)
```

## 修改BeamSPR属性的参数

### 1. 获取BeamSPR属性对象

根据属性名称获取已存在的BeamSPR属性对象：
- 参数：属性名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getPropertyType('Property-1')` 判断对象是否为 `BeamSPR` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
property=Property.BeamSPR.GetBeamSPR('Property-1')
```

## 修改BeamSPR属性参数

获取对象后，可修改以下属性：

### 1. 修改属性名称

```python
property.setName('Property-2')
```

设置BeamSPR属性的新名称：
- 参数：属性名称（字符串）

### 2. 设置质量

```python
property.setMass(0.0)
```

设置质量：
- 参数：参数值（浮点数）

### 3. 设置弹簧惯量

```python
property.setInertia(0.0)
```

设置弹簧惯量：
- 参数：参数值（浮点数）

### 4. 设置传感器标志

```python
property.setIsflag(0)
```

设置传感器标志：
- 参数：参数值（整数）

### 5. 设置失效标准

```python
property.setIfail(0)
```

设置失效标准：
- 参数：参数值（整数）

### 6. 设置每单位长度输入标志

```python
property.setIleng(0)
```

设置每单位长度输入标志：
- 参数：参数值（整数）

### 7. 设置故障模型标志

```python
property.setIfail2(0)
```

设置故障模型标志：
- 参数：参数值（整数）

### 8. 设置参考速度

```python
property.setV0(0.0)
```

设置参考速度：
- 参数：参数值（浮点数）

### 9. 设置参考角速度

```python
property.setW0(0.0)
```

设置参考角速度：
- 参数：参数值（浮点数）

### 10. 设置应变率切割频率

```python
property.setFcut(0.0)
```

设置应变率切割频率：
- 参数：参数值（浮点数）

### 11. 设置平滑应变率标志

```python
property.setFsmooth(0)
```

设置平滑应变率标志：
- 参数：参数值（整数）

### 12. 设置自由度参数

```python
property.setK(1, 0.0)
property.setC(1, 0.0)
property.setA(1, 0.0)
property.setB(1, 0.0)
property.setD(1, 0.0)
property.setFunction1(1, 'Curve-1')
property.setH(1, 0)
property.setFunction2(1, 'Curve-2')
property.setFunction3(1, 'Curve-3')
property.setFunction4(1, 'Curve-4')
property.setMin(1, 0.0)
property.setMax(1, 0.0)
property.setF(1, 0.0)
property.setE(1, 0.0)
property.setAscale(1, 0.0)
property.setHscale(1, 0.0)
property.setVc(1, 0.0)
property.setN(1, 0.0)
property.setAlpha(1, 0.0)
property.setExponent(1, 0.0)
```

设置指定自由度上的参数：
- 自由度参数 `dof` 只能输入 1 到 6
- 曲线参数使用的是曲线名称，不是曲线 ID

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| BeamSPR | - | 创建BeamSPR对象 |
| BeamSPR.GetBeamSPR | string | 根据名称获取已存在的BeamSPR属性对象 |
| createProperty | string | 创建指定名称的BeamSPR属性 |
| setName | string | 修改属性名称 |
| setMass | double | 设置质量 |
| setInertia | double | 设置弹簧惯量 |
| setIsflag | int | 设置传感器标志 |
| setIfail | int | 设置失效标准 |
| setIleng | int | 设置每单位长度输入标志 |
| setIfail2 | int | 设置故障模型标志 |
| setV0 | double | 设置参考速度 |
| setW0 | double | 设置参考角速度 |
| setFcut | double | 设置应变率切割频率 |
| setFsmooth | int | 设置平滑应变率标志 |
| setK | int, double | 设置指定自由度的刚度 |
| setC | int, double | 设置指定自由度的阻尼 |
| setA | int, double | 设置指定自由度的非线性刚度函数比例系数 |
| setB | int, double | 设置指定自由度的对数速率效应比例系数 B |
| setD | int, double | 设置指定自由度的对数速率效应比例系数 D |
| setFunction1 | int, string | 设置指定自由度的非线性刚度函数 ID1 |
| setH | int, int | 设置指定自由度的弹簧硬化标志 H |
| setFunction2 | int, string | 设置指定自由度的函数 ID2 |
| setFunction3 | int, string | 设置指定自由度的函数 ID3 |
| setFunction4 | int, string | 设置指定自由度的函数 ID4 |
| setMin | int, double | 设置指定自由度的最小失效阈值 |
| setMax | int, double | 设置指定自由度的最大失效阈值 |
| setF | int, double | 设置指定自由度的阻尼函数横坐标比例系数 F |
| setE | int, double | 设置指定自由度的阻尼函数纵坐标比例系数 E |
| setAscale | int, double | 设置指定自由度的刚度函数横坐标比例系数 Ascale |
| setHscale | int, double | 设置指定自由度的阻尼函数 h() 纵坐标比例系数 Hscale |
| setVc | int, double | 设置指定自由度的相对速度系数 vc |
| setN | int, double | 设置指定自由度的相对速度指数 n |
| setAlpha | int, double | 设置指定自由度的故障比例系数 alpha |
| setExponent | int, double | 设置指定自由度的指数 beta |

## 注意事项

1. 创建前需保证当前存在有效的属性管理环境。
2. 同名BeamSPR属性已存在时，`createProperty` 会失败。
3. 编辑前建议先通过 `Property.BeamSPR.GetBeamSPR` 获取对象。
4. 编辑已有属性时，建议先通过 `AI.AI_Properties().getPropertyType('属性名')` 确认对象类型。
5. 示例脚本按当前界面 `writePythonScript` 的实际输出顺序组织，但未包含 `setSensID`，因为当前 Python 包装类尚未暴露该接口。
6. `dof` 参数只能输入 1 到 6，对应六个弹簧自由度。
7. `setFunction1`、`setFunction2`、`setFunction3`、`setFunction4` 使用的是曲线名称，不是曲线 ID；对应曲线必须已存在于当前算例中。
8. 界面脚本还会涉及未暴露的 `setSensID`，若后续包装类补齐，该 skill 可以再补充对应说明与示例。
