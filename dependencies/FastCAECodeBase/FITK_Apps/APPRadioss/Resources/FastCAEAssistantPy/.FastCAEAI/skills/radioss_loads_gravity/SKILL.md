# Radioss 重力载荷

## 功能描述

本技能用于在Radioss中创建和修改重力载荷，支持设置载荷名称、重力方向、重力强度、作用节点组，以及可选的时间函数和 X 方向缩放因子。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行重力载荷配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建重力载荷

### 1. 创建重力载荷

创建重力载荷操作对象后，可以创建重力载荷。

```python
p=Gravity.Gravity()
p.createGravity('Gravity-1')
```
- 参数：重力载荷名称（字符串）
- 使用指定名称创建重力载荷
- 注意：如果已存在同名载荷会创建失败

**完整示例：创建重力载荷并设置部分参数**

```python
p=Gravity.Gravity()
p.createGravity('Gravity-1')
p.setGravityDirection('Z')
p.setFScaleY(9.8)
p.setNodeGroup('Global')
```

## 修改重力载荷

### 1. 获取重力载荷对象

```python
p=Gravity.Gravity.GetGravity('Gravity-1')
```

根据重力载荷名称获取已存在的重力载荷对象：
- 参数：重力载荷名称（字符串）

## 修改重力载荷参数

获取对象后，可修改以下属性：

### 1. 修改重力载荷名称

```python
p.setName('Gravity-2')
```

设置重力载荷的新名称：
- 参数：重力载荷名称（字符串）

### 2. 设置重力方向

```python
p.setGravityDirection('Z')
```

设置重力加速度的方向：
- 参数：方向标识（字符串）
- `X`：X 轴方向
- `Y`：Y 轴方向
- `Z`：Z 轴方向

### 3. 设置重力强度

```python
p.setFScaleY(9.8)
```

设置重力加速度的强度值：
- 参数：重力强度（浮点数，可为正负）
- 正值表示沿设置方向的加速度
- 负值表示沿设置方向的反方向加速度

### 4. 设置作用节点组

```python
p.setNodeGroup('Global')
```

设置需要施加重力的节点组：
- 参数：节点组名称（字符串）
- 特殊值：`Global` 表示全局施加重力

### 5. 设置时间函数（可选）

```python
p.setTimeFunction('Curve-1')
```

设置重力载荷的时间函数：
- 参数：时间函数名称（字符串）
- 用于定义随时间变化的重力载荷

### 6. 设置 X 方向缩放因子（可选）

```python
p.setAScaleX(1.0)
```

设置X方向加速度缩放因子：
- 参数：缩放因子（浮点数）
- 用于调整X方向重力分量

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| Gravity | - | 创建Gravity对象 |
| Gravity.GetGravity | string | 根据名称获取已存在的重力载荷对象 |
| createGravity | string | 创建指定名称的重力载荷 |
| setName | string | 修改重力载荷名称 |
| setGravityDirection | string | 设置重力方向（'X'/'Y'/'Z'） |
| setFScaleY | double | 设置重力强度值 |
| setNodeGroup | string | 设置作用节点组（'Global'为全局） |
| setTimeFunction | string | 设置时间函数名称 |
| setAScaleX | double | 设置X方向加速度缩放因子 |

## 注意事项

1. 默认使用当前有效的Solution，无需额外判断或设置。
2. 同名重力载荷已存在时，`createGravity` 会失败。
3. 编辑前建议先通过 `Gravity.Gravity.GetGravity` 获取对象。
4. `setGravityDirection` 仅接受 `X`、`Y`、`Z` 三个方向标识。
5. `setFScaleY` 的正负号与方向组合决定最终重力方向，例如方向 `Z` 配合 `-9.8` 表示沿 `-Z` 方向施加重力。
6. `setNodeGroup('Global')` 表示对全局节点施加重力。
7. 界面 `writePythonScript` 当前只直接输出 `setGravityDirection`、`setFScaleY`、`setNodeGroup` 这组参数；`setTimeFunction` 和 `setAScaleX` 可通过 wrapper 接口补充设置。
8. 示例中的参数顺序与界面 `writePythonScript` 输出顺序保持一致。