# Radioss JOHNSON失效模型

## 功能描述

本技能用于在Radioss中创建和修改JOHNSON失效模型，支持设置失效模型名称、关联材料、Johnson-Cook失效参数、参考应变率、壳/实体失效标志、裂纹扩展判据以及 XFEM 标志。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行JOHNSON失效模型配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建JOHNSON失效模型

### 1. 创建JOHNSON失效模型

创建JOHNSON失效模型操作对象后，可以创建JOHNSON失效模型。

```python
failure=Failure.JOHNSON()
failure.createFailureModel('Failure-1')
```

- 参数：失效模型名称（字符串）
- 使用指定名称创建JOHNSON失效模型
- 注意：如果已存在同名失效模型会创建失败

**完整示例：创建JOHNSON失效模型并设置部分参数**

```python
failure=Failure.JOHNSON()
failure.createFailureModel('Failure-1')
failure.addMaterial('Material-1')
failure.setD1(0.1)
failure.setD2(0.2)
failure.setD3(0.3)
failure.setD4(0.4)
failure.setD5(0.5)
failure.setEps0(0.01)
failure.setIfailsh(1)
failure.setIfailso(1)
failure.setDadv(1.0)
failure.setIxfem(0)
```

## 修改JOHNSON失效模型

### 1. 获取JOHNSON失效模型对象

根据名称获取已存在的JOHNSON失效模型对象：
- 参数：失效模型名称（字符串）
- 编辑前可先通过 `AI.AI_Properties().getFailureModelType('Failure-1')` 判断对象是否为 `JOHNSON` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
failure=Failure.JOHNSON.GetJOHNSON('Failure-1')
```

## 修改JOHNSON失效模型参数

获取对象后，可修改以下属性：

### 1. 修改失效模型名称

```python
failure.setName('Failure-2')
```

设置JOHNSON失效模型的新名称：
- 参数：名称（字符串）

### 2. 添加材料

```python
failure.addMaterial('Material-1')
```

向JOHNSON失效模型添加材料：
- 参数：材料名称（字符串）

### 3. 移除材料

```python
failure.removeMaterial('Material-1')
```

从JOHNSON失效模型移除材料：
- 参数：材料名称（字符串）

### 4. 设置参数 D1

```python
failure.setD1(0.1)
```

设置参数 D1：
- 参数：参数值（浮点数）

### 5. 设置参数 D2

```python
failure.setD2(0.2)
```

设置参数 D2：
- 参数：参数值（浮点数）

### 6. 设置参数 D3

```python
failure.setD3(0.3)
```

设置参数 D3：
- 参数：参数值（浮点数）

### 7. 设置参数 D4

```python
failure.setD4(0.4)
```

设置参数 D4：
- 参数：参数值（浮点数）

### 8. 设置参数 D5

```python
failure.setD5(0.5)
```

设置参数 D5：
- 参数：参数值（浮点数）

### 9. 设置参考应变率 Eps0

```python
failure.setEps0(0.01)
```

设置参考应变率 Eps0：
- 参数：参数值（浮点数）

### 10. 设置壳失效标志 Ifailsh

```python
failure.setIfailsh(1)
```

设置壳失效标志 Ifailsh：
- 参数：标志值（整数）

### 11. 设置实体失效标志 Ifailso

```python
failure.setIfailso(1)
```

设置实体失效标志 Ifailso：
- 参数：标志值（整数）

### 12. 设置裂纹扩展判据 Dadv

```python
failure.setDadv(1.0)
```

设置裂纹扩展判据 Dadv：
- 参数：参数值（浮点数）

### 13. 设置 XFEM 标志 Ixfem

```python
failure.setIxfem(0)
```

设置 XFEM 标志 Ixfem：
- 参数：标志值（整数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| JOHNSON | - | 创建JOHNSON对象 |
| JOHNSON.GetJOHNSON | string | 根据名称获取已存在的JOHNSON失效模型对象 |
| createFailureModel | string | 创建指定名称的JOHNSON失效模型 |
| setName | string | 修改失效模型名称 |
| addMaterial | string | 添加材料 |
| removeMaterial | string | 移除材料 |
| setD1 | double | 设置参数 D1 |
| setD2 | double | 设置参数 D2 |
| setD3 | double | 设置参数 D3 |
| setD4 | double | 设置参数 D4 |
| setD5 | double | 设置参数 D5 |
| setEps0 | double | 设置参考应变率 Eps0 |
| setIfailsh | int | 设置壳失效标志 Ifailsh |
| setIfailso | int | 设置实体失效标志 Ifailso |
| setDadv | double | 设置裂纹扩展判据 Dadv |
| setIxfem | int | 设置 XFEM 标志 Ixfem |

## 注意事项

1. 创建前需保证当前存在有效的失效模型管理环境。
2. 同名JOHNSON失效模型已存在时，`createFailureModel` 会失败。
3. 编辑前建议先通过 `Failure.JOHNSON.GetJOHNSON` 获取对象。
4. 编辑已有失效模型时，建议先通过 `AI.AI_Properties().getFailureModelType('失效模型名')` 确认对象类型。
5. `addMaterial` 和 `removeMaterial` 使用的是材料名称，不是材料 ID；对应材料必须已存在。
6. `writePythonScript` 生成的脚本只会输出界面上可编辑的参数项；如需维护关联材料，需要额外调用 `addMaterial` 或 `removeMaterial`。
7. `Ifailsh`、`Ifailso`、`Ixfem` 应使用接口支持的整数标志值。