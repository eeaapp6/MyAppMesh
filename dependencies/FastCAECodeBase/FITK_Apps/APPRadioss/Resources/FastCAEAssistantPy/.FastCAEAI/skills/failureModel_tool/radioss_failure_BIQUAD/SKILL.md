# Radioss BIQUAD失效模型

## 功能描述

本技能用于在Radioss中创建和修改BIQUAD失效模型，支持设置失效模型名称、关联材料、失效塑性应变参数、材料选择标志、特定行为标志、局部缩颈参数、单元尺寸因子曲线以及 M-Flag=99 时的比例参数。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行BIQUAD失效模型配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建BIQUAD失效模型

### 1. 创建BIQUAD失效模型

创建BIQUAD失效模型操作对象后，可以创建BIQUAD失效模型。

```python
failure=Failure.BIQUAD()
failure.createFailureModel('Failure-1')
```

- 参数：失效模型名称（字符串）
- 使用指定名称创建BIQUAD失效模型
- 注意：如果已存在同名失效模型会创建失败

**完整示例：创建BIQUAD失效模型并设置部分参数**

```python
failure=Failure.BIQUAD()
failure.createFailureModel('Failure-1')
failure.addMaterial('Material-1')
failure.setC1(0.1)
failure.setC2(0.2)
failure.setC3(0.3)
failure.setC4(0.4)
failure.setC5(0.5)
failure.setPthickfail(0.0)
failure.setMFlag(0)
failure.setSFlag(1)
failure.setInststart(0.0)
failure.setFctIDel('Curve-1')
failure.setElref(1.0)
```

## 修改BIQUAD失效模型

### 1. 获取BIQUAD失效模型对象

根据名称获取已存在的BIQUAD失效模型对象：
- 参数：失效模型名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getFailureModelType('Failure-1')` 判断对象是否为 `BIQUAD` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
failure=Failure.BIQUAD.GetBIQUAD('Failure-1')
```

## 修改BIQUAD失效模型参数

获取对象后，可修改以下属性：

### 1. 修改失效模型名称

```python
failure.setName('Failure-2')
```

设置BIQUAD失效模型的新名称：
- 参数：名称（字符串）

### 2. 添加材料

```python
failure.addMaterial('Material-1')
```

向BIQUAD失效模型添加材料：
- 参数：材料名称（字符串）

### 3. 移除材料

```python
failure.removeMaterial('Material-1')
```

从BIQUAD失效模型移除材料：
- 参数：材料名称（字符串）

### 4. 设置单轴压缩失效塑性应变

```python
failure.setC1(0.1)
```

设置单轴压缩失效塑性应变：
- 参数：参数值（浮点数）

### 5. 设置剪切失效塑性应变

```python
failure.setC2(0.2)
```

设置剪切失效塑性应变：
- 参数：参数值（浮点数）

### 6. 设置单轴拉伸失效塑性应变

```python
failure.setC3(0.3)
```

设置单轴拉伸失效塑性应变：
- 参数：参数值（浮点数）

### 7. 设置平面应变拉伸失效塑性应变

```python
failure.setC4(0.4)
```

设置平面应变拉伸失效塑性应变：
- 参数：参数值（浮点数）

### 8. 设置双轴拉伸失效应变

```python
failure.setC5(0.5)
```

设置双轴拉伸失效应变：
- 参数：参数值（浮点数）

### 9. 设置壳单元厚度方向失效积分点比例

```python
failure.setPthickfail(0.0)
```

设置壳单元厚度方向失效积分点比例：
- 参数：参数值（浮点数）

### 10. 设置材料选择标志

```python
failure.setMFlag(0)
```

设置材料选择标志：
- 参数：标志值（整数）

### 11. 设置特定行为标志

```python
failure.setSFlag(1)
```

设置特定行为标志：
- 参数：标志值（整数）

### 12. 设置局部缩颈失稳起始值

```python
failure.setInststart(0.0)
```

设置局部缩颈失稳起始值：
- 参数：参数值（浮点数）

### 13. 设置单元尺寸因子函数曲线

```python
failure.setFctIDel('Curve-1')
```

设置单元尺寸因子函数曲线：
- 参数：曲线名称（字符串）
- 传空字符串可清空当前曲线引用

### 14. 设置参考单元尺寸

```python
failure.setElref(1.0)
```

设置参考单元尺寸：
- 参数：参数值（浮点数）

### 15. 设置 M-Flag=99 时的失效塑性应变比 R1

```python
failure.setR1(1.0)
```

设置 R1：
- 参数：参数值（浮点数）

### 16. 设置 M-Flag=99 时的失效塑性应变比 R2

```python
failure.setR2(1.0)
```

设置 R2：
- 参数：参数值（浮点数）

### 17. 设置 M-Flag=99 时的失效塑性应变比 R4

```python
failure.setR4(1.0)
```

设置 R4：
- 参数：参数值（浮点数）

### 18. 设置 M-Flag=99 时的失效塑性应变比 R5

```python
failure.setR5(1.0)
```

设置 R5：
- 参数：参数值（浮点数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| BIQUAD | - | 创建BIQUAD对象 |
| BIQUAD.GetBIQUAD | string | 根据名称获取已存在的BIQUAD失效模型对象 |
| createFailureModel | string | 创建指定名称的BIQUAD失效模型 |
| setName | string | 修改失效模型名称 |
| addMaterial | string | 添加材料 |
| removeMaterial | string | 移除材料 |
| setC1 | double | 设置单轴压缩失效塑性应变 |
| setC2 | double | 设置剪切失效塑性应变 |
| setC3 | double | 设置单轴拉伸失效塑性应变 |
| setC4 | double | 设置平面应变拉伸失效塑性应变 |
| setC5 | double | 设置双轴拉伸失效应变 |
| setPthickfail | double | 设置壳单元厚度方向失效积分点比例 |
| setMFlag | int | 设置材料选择标志 |
| setSFlag | int | 设置特定行为标志 |
| setInststart | double | 设置局部缩颈失稳起始值 |
| setFctIDel | string | 设置单元尺寸因子函数曲线名称 |
| setElref | double | 设置参考单元尺寸 |
| setR1 | double | 设置 R1 |
| setR2 | double | 设置 R2 |
| setR4 | double | 设置 R4 |
| setR5 | double | 设置 R5 |

## 注意事项

1. 创建前需保证当前存在有效的失效模型管理环境。
2. 同名BIQUAD失效模型已存在时，`createFailureModel` 会失败。
3. 编辑前建议先通过 `Failure.BIQUAD.GetBIQUAD` 获取对象。
4. 编辑已有失效模型时，建议先通过 `AI.AI_Properties().getFailureModelType('失效模型名')` 确认对象类型。
5. `addMaterial` 和 `removeMaterial` 使用的是材料名称，不是材料 ID；对应材料必须已存在。
6. `setFctIDel` 使用的是曲线名称，不是曲线 ID；对应曲线必须已存在于当前算例中。
7. 当前界面 `writePythonScript` 在 `MFlag != 99` 时不会输出 `R1`、`R2`、`R4`、`R5`。
8. 若 `MFlag = 99`，应同时设置 `R1`、`R2`、`R4`、`R5`，否则参数含义不完整。
