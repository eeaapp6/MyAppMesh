# Radioss Polynomial状态方程

## 功能描述

本技能用于在Radioss中创建和修改Polynomial状态方程，支持设置状态方程名称、关联材料、参考密度 P0、压力定律系数 C0-C5、初始单位参考体积内能 E0 以及压力偏移 Psh。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行Polynomial状态方程配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建Polynomial状态方程

### 1. 创建Polynomial状态方程

创建Polynomial状态方程操作对象后，可以创建Polynomial状态方程。

```python
eos=EOS.EOSPolynomial()
eos.createEquationOfState('EOS-1')
```

- 参数：状态方程名称（字符串）
- 使用指定名称创建Polynomial状态方程
- 注意：如果已存在同名状态方程会创建失败

**完整示例：创建Polynomial状态方程并设置部分参数**

```python
eos=EOS.EOSPolynomial()
eos.createEquationOfState('EOS-1')
eos.addMaterial('Material-1')
eos.setC0(0.0)
eos.setC1(0.0)
eos.setC2(0.0)
eos.setC3(0.0)
eos.setC4(0.0)
eos.setC5(0.0)
eos.setE0(0.0)
eos.setPsh(0.0)
eos.setP0(1.0)
```

## 修改Polynomial状态方程

### 1. 获取Polynomial状态方程对象

根据名称获取已存在的Polynomial状态方程对象：
- 参数：状态方程名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getEquationOfStateType('EOS-1')` 判断对象是否为 `Polynomial` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
eos=EOS.EOSPolynomial.GetEOSPolynomial('EOS-1')
```

## 修改Polynomial状态方程参数

获取对象后，可修改以下属性：

### 1. 修改状态方程名称

```python
eos.setName('EOS-2')
```

设置Polynomial状态方程的新名称：
- 参数：名称（字符串）

### 2. 添加材料

```python
eos.addMaterial('Material-1')
```

向Polynomial状态方程添加材料：
- 参数：材料名称（字符串）

### 3. 移除材料

```python
eos.removeMaterial('Material-1')
```

从Polynomial状态方程移除材料：
- 参数：材料名称（字符串）

### 4. 设置参考密度 P0

```python
eos.setP0(1.0)
```

设置参考密度 P0：
- 参数：参数值（浮点数）

### 5. 设置系数 C0

```python
eos.setC0(0.0)
```

设置系数 C0：
- 参数：参数值（浮点数）

### 6. 设置系数 C1

```python
eos.setC1(0.0)
```

设置系数 C1：
- 参数：参数值（浮点数）

### 7. 设置系数 C2

```python
eos.setC2(0.0)
```

设置系数 C2：
- 参数：参数值（浮点数）

### 8. 设置系数 C3

```python
eos.setC3(0.0)
```

设置系数 C3：
- 参数：参数值（浮点数）

### 9. 设置系数 C4

```python
eos.setC4(0.0)
```

设置系数 C4：
- 参数：参数值（浮点数）

### 10. 设置系数 C5

```python
eos.setC5(0.0)
```

设置系数 C5：
- 参数：参数值（浮点数）

### 11. 设置初始单位参考体积内能 E0

```python
eos.setE0(0.0)
```

设置初始单位参考体积内能 E0：
- 参数：参数值（浮点数）

### 12. 设置压力偏移 Psh

```python
eos.setPsh(0.0)
```

设置压力偏移 Psh：
- 参数：参数值（浮点数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| EOSPolynomial | - | 创建Polynomial状态方程对象 |
| EOSPolynomial.GetEOSPolynomial | string | 根据名称获取已存在的Polynomial状态方程对象 |
| createEquationOfState | string | 创建指定名称的Polynomial状态方程 |
| setName | string | 修改状态方程名称 |
| addMaterial | string | 添加材料 |
| removeMaterial | string | 移除材料 |
| setP0 | double | 设置参考密度 P0 |
| setC0 | double | 设置系数 C0 |
| setC1 | double | 设置系数 C1 |
| setC2 | double | 设置系数 C2 |
| setC3 | double | 设置系数 C3 |
| setC4 | double | 设置系数 C4 |
| setC5 | double | 设置系数 C5 |
| setE0 | double | 设置初始单位参考体积内能 E0 |
| setPsh | double | 设置压力偏移 Psh |

## 注意事项

1. 创建前需保证当前存在有效的状态方程管理环境。
2. 同名Polynomial状态方程已存在时，`createEquationOfState` 会失败。
3. 编辑前建议先通过 `EOS.EOSPolynomial.GetEOSPolynomial` 获取对象。
4. 编辑已有状态方程时，建议先通过 `AI.AI_Properties().getEquationOfStateType('状态方程名')` 确认对象类型。
5. `addMaterial` 和 `removeMaterial` 使用的是材料名称，不是材料 ID；对应材料必须已存在。
6. 当前界面 `writePythonScript` 不会输出 `addMaterial` 和 `removeMaterial`；如需维护关联材料，需要额外调用这两个接口。
7. 目前该skill对应的状态方程类型为 `Polynomial`。
