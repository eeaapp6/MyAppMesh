# Radioss 初始平移速度场

## 功能描述

本技能用于在Radioss中创建和修改初始平移速度场，支持设置名称、三方向初始速度以及节点组。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行初始平移速度场配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建初始平移速度场

### 1. 创建初始平移速度场

创建初始平移速度场操作对象后，可以创建初始平移速度场。

```python
p=InitialValue.InitialValueTra()
p.createInitialValue('InitialField-1')
```

- 参数：初速度场名称（字符串）
- 使用指定名称创建初始平移速度场
- 注意：如果已存在同名初速度场会创建失败

**完整示例：创建初始平移速度场并设置部分参数**

```python
p=InitialValue.InitialValueTra()
p.createInitialValue('InitialField-1')
p.setValue(1.0, 0.0, 0.0)
p.setNodeGroup('NodeGroup-1')
```

## 修改初始平移速度场

### 1. 获取初始平移速度场对象

根据初速度场名称获取已存在的初始平移速度场对象：
- 参数：初速度场名称（字符串）
- 编辑前可先通过 `AI.AI_Solution().getInitialValueType('InitialField-1')` 判断对象是否为 `InitialValueTra` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
p=InitialValue.InitialValueTra.GetInitialValueTra('InitialField-1')
```

## 修改初始平移速度场参数

获取对象后，可修改以下属性：

### 1. 修改初速度场名称

```python
p.setName('InitialField-2')
```

设置初速度场的新名称：
- 参数：初速度场名称（字符串）

### 2. 设置初始速度

```python
p.setValue(1.0, 0.0, 0.0)
```

设置三个方向的初始速度：
- 参数：`x, y, z` 三个方向速度值（浮点数）

### 3. 设置节点组

```python
p.setNodeGroup('NodeGroup-1')
```

设置初速度场作用的节点组：
- 参数：节点组名称（字符串）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| InitialValueTra | - | 创建InitialValueTra对象 |
| InitialValueTra.GetInitialValueTra | string | 根据名称获取已存在的初始平移速度场对象 |
| createInitialValue | string | 创建指定名称的初始平移速度场 |
| setName | string | 修改初速度场名称 |
| setValue | double, double, double | 设置三个方向的初始速度 |
| setNodeGroup | string | 设置作用节点组 |

## 注意事项

1. 默认使用当前有效的Solution，无需额外判断或设置。
2. 同名初速度场已存在时，`createInitialValue` 会失败。
3. 编辑前建议先通过 `InitialValue.InitialValueTra.GetInitialValueTra` 获取对象。
4. 编辑已有初速度场前，建议先通过 `AI.AI_Solution().getInitialValueType('初速度场名')` 确认对象类型。
5. `setNodeGroup` 需要传入有效的节点组名称，界面创建时要求必须选择节点组。
6. 示例中的参数顺序与界面 `writePythonScript` 输出顺序保持一致。
