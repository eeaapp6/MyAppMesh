# Radioss 边界条件

## 功能描述

本技能用于在Radioss中创建和修改标准边界条件 BCS，支持设置边界条件名称、节点组以及 1 到 6 自由度的约束状态。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行边界条件配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建边界条件

### 1. 创建边界条件

创建边界条件操作对象后，可以创建边界条件。

```python
p=BCS.BCS()
p.createBCS('BCS-1')
```

- 参数：边界条件名称（字符串）
- 使用指定名称创建边界条件
- 注意：如果已存在同名边界条件会创建失败

**完整示例：创建边界条件并设置部分参数**

```python
p=BCS.BCS()
p.createBCS('BCS-1')
p.setNodeGroup('NodeGroup-1')
p.setFreedomFixed(1, True)
p.setFreedomFixed(2, True)
p.setFreedomFixed(3, True)
p.setFreedomFixed(4, False)
p.setFreedomFixed(5, False)
p.setFreedomFixed(6, False)
```

## 修改边界条件

### 1. 获取边界条件对象

根据边界条件名称获取已存在的边界条件对象：
- 参数：边界条件名称（字符串）
- 编辑前可先通过 `AI.AI_Solution().getBCSType('BCS-1')` 判断对象是否为 `BCS` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
p=BCS.BCS.GetBCS('BCS-1')
```

## 修改边界条件参数

获取对象后，可修改以下属性：

### 1. 修改边界条件名称

```python
p.setName('BCS-2')
```

设置边界条件的新名称：
- 参数：边界条件名称（字符串）

### 2. 设置节点组

```python
p.setNodeGroup('NodeGroup-1')
```

设置边界条件作用的节点组：
- 参数：节点组名称（字符串）

### 3. 设置自由度约束状态

```python
p.setFreedomFixed(1, True)
p.setFreedomFixed(2, True)
p.setFreedomFixed(3, True)
p.setFreedomFixed(4, False)
p.setFreedomFixed(5, False)
p.setFreedomFixed(6, False)
```

设置 1 到 6 号自由度的约束状态：
- 参数1：自由度索引（整数，范围 1-6）
- 参数2：是否固定（布尔值）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| BCS | - | 创建BCS对象 |
| BCS.GetBCS | string | 根据名称获取已存在的边界条件对象 |
| createBCS | string | 创建指定名称的边界条件 |
| setName | string | 修改边界条件名称 |
| setNodeGroup | string | 设置节点组 |
| setFreedomFixed | int, bool | 设置指定自由度是否固定 |

## 注意事项

1. 默认使用当前有效的Solution，无需额外判断或设置。
2. 同名边界条件已存在时，`createBCS` 会失败。
3. 编辑前建议先通过 `BCS.BCS.GetBCS` 获取对象。
4. 编辑已有边界条件前，建议先通过 `AI.AI_Solution().getBCSType('边界条件名')` 确认对象类型。
5. `setNodeGroup` 需要传入当前模型中已存在的节点组名称。
6. `setFreedomFixed` 的自由度索引范围是 1 到 6，分别对应界面中的 Tx、Ty、Tz、Wx、Wy、Wz。
7. 示例中的参数顺序与界面 `writePythonScript` 输出顺序保持一致。
