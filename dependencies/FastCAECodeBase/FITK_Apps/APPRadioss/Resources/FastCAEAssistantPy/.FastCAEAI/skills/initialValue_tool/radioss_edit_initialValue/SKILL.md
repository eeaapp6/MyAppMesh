# Radioss 编辑初速度场

## 功能描述

本技能用于在Radioss中编辑已有初速度场。
执行编辑前，先通过 `AI.AI_Solution().getInitialValueType(初速度场名称)` 判断初速度场是否存在，并获取初速度场类型。
确认初速度场类型后，再进入对应初速度场类型的skill中执行具体编辑操作。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand先查询初速度场类型，再根据返回类型选择对应skill执行编辑。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 编辑流程

### 1. 查询初速度场是否存在及初速度场类型

```python
AI.AI_Solution().getInitialValueType('InitialField-1')
```

说明：
- 参数：初速度场名称（字符串）
- 返回：初速度场是否存在及初速度场类型信息
- 作用：判断目标初速度场是否存在，并识别其初速度场类型

### 2. 根据初速度场类型进入对应skill

当 `AI.AI_Solution().getInitialValueType('初速度场名称')` 返回类型后，按下列规则选择对应skill：

- 返回 `InitialValueTra`：进入 `radioss_initialValue_TRA` skill，执行初始平移速度场编辑。
- 返回 `InitialValueAxis`：进入 `radioss_initialValue_AXIS` skill，执行角速度初速度场编辑。
- 返回空值、失败信息或未知类型：停止编辑，并提示初速度场不存在或当前暂不支持该初速度场类型。

## 调用示例

### 示例1：先查询初速度场类型

```python
AI.AI_Solution().getInitialValueType('InitialField-1')
```

### 示例2：初速度场类型为InitialValueTra时

先确认类型：

```python
AI.AI_Solution().getInitialValueType('InitialField-1')
```

若返回 `InitialValueTra`，则在 `radioss_initialValue_TRA` skill 中执行：

```python
p=InitialValue.InitialValueTra.GetInitialValueTra('InitialField-1')
p.setValue(1.0, 0.0, 0.0)
p.setNodeGroup('NodeGroup-1')
```

### 示例3：初速度场类型为InitialValueAxis时

先确认类型：

```python
AI.AI_Solution().getInitialValueType('InitialField-1')
```

若返回 `InitialValueAxis`，则在 `radioss_initialValue_AXIS` skill 中执行：

```python
p=InitialValue.InitialValueAxis.GetInitialValueAxis('InitialField-1')
p.setDir('X')
p.setVr(10.0)
```

## 使用约束

1. 本skill只负责识别初速度场是否存在以及初速度场类型，不直接定义某一种初速度场的全部编辑接口。
2. 识别出初速度场类型后，应切换到对应初速度场skill执行具体编辑操作。
3. 若初速度场不存在，不应继续执行编辑命令。
4. 若初速度场类型当前没有对应skill，不应继续编辑，应提示当前暂不支持。
5. 编辑已有初速度场前，优先执行一次 `AI.AI_Solution().getInitialValueType('初速度场名')`。

## 建议执行策略

1. 从用户输入中提取目标初速度场名称和待修改参数。
2. 调用 `AI.AI_Solution().getInitialValueType('初速度场名称')`。
3. 判断返回结果是否有效。
4. 根据初速度场类型选择对应skill。
5. 在对应skill中生成并执行具体的初速度场编辑脚本。
6. 将具体编辑接口的返回结果反馈给用户。
