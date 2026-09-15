# Radioss 编辑相互作用

## 功能描述

本技能用于在Radioss中编辑已有相互作用。
执行编辑前，先通过 `AI.AI_Solution().getInteractionType(相互作用名称)` 判断相互作用是否存在，并获取相互作用类型。
确认相互作用类型后，再进入对应相互作用类型的skill中执行具体编辑操作。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand先查询相互作用类型，再根据返回类型选择对应skill执行编辑。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 编辑流程

### 1. 查询相互作用是否存在及相互作用类型

```python
AI.AI_Solution().getInteractionType('Interaction-1')
```

说明：
- 参数：相互作用名称（字符串）
- 返回：相互作用是否存在及相互作用类型信息
- 作用：判断目标相互作用是否存在，并识别其相互作用类型

### 2. 根据相互作用类型进入对应skill

当 `AI.AI_Solution().getInteractionType('相互作用名称')` 返回类型后，按下列规则选择对应skill：

- 返回 `KinematicTied`：进入 `radioss_interaction_tied` skill，执行动力学绑定相互作用编辑。
- 返回 `MultiUsageImpact`：进入 `radioss_interaction_multiUsageImpact` skill，执行多用途冲击相互作用编辑。
- 返回空值、失败信息或未知类型：停止编辑，并提示相互作用不存在或当前暂不支持该相互作用类型。

## 调用示例

### 示例1：先查询相互作用类型

```python
AI.AI_Solution().getInteractionType('Interaction-1')
```

### 示例2：相互作用类型为KinematicTied时

先确认类型：

```python
AI.AI_Solution().getInteractionType('Interaction-1')
```

若返回 `KinematicTied`，则在 `radioss_interaction_tied` skill 中执行：

```python
p=InterTied.InterTied.GetInterTied('Interaction-1')
p.setLevel(1)
p.setSearchDistance(2.5)
```

### 示例3：相互作用类型为MultiUsageImpact时

先确认类型：

```python
AI.AI_Solution().getInteractionType('Interaction-1')
```

若返回 `MultiUsageImpact`，则在 `radioss_interaction_multiUsageImpact` skill 中执行：

```python
p=InterMultiUsageImpact.InterMultiUsageImpact.GetInterMultiUsageImpact('Interaction-1')
p.setFric(0.2)
p.setGapmin(0.5)
```

## 使用约束

1. 本skill只负责识别相互作用是否存在以及相互作用类型，不直接定义某一种相互作用的全部编辑接口。
2. 识别出相互作用类型后，应切换到对应相互作用skill执行具体编辑操作。
3. 若相互作用不存在，不应继续执行编辑命令。
4. 若相互作用类型当前没有对应skill，不应继续编辑，应提示当前暂不支持。
5. 编辑已有相互作用前，优先执行一次 `AI.AI_Solution().getInteractionType('相互作用名')`。

## 建议执行策略

1. 从用户输入中提取目标相互作用名称和待修改参数。
2. 调用 `AI.AI_Solution().getInteractionType('相互作用名称')`。
3. 判断返回结果是否有效。
4. 根据相互作用类型选择对应skill。
5. 在对应skill中生成并执行具体的相互作用编辑脚本。
6. 将具体编辑接口的返回结果反馈给用户。
