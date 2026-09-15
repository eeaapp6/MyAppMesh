# Radioss 编辑边界条件

## 功能描述

本技能用于在Radioss中编辑已有边界条件。
执行编辑前，先通过 `AI.AI_Solution().getBCSType(边界条件名称)` 判断边界条件是否存在，并获取边界条件类型。
确认边界条件类型后，再进入对应边界条件类型的skill中执行具体编辑操作。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand先查询边界条件类型，再根据返回类型选择对应skill执行编辑。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 编辑流程

### 1. 查询边界条件是否存在及边界条件类型

```python
AI.AI_Solution().getBCSType('BCS-1')
```

说明：
- 参数：边界条件名称（字符串）
- 返回：边界条件是否存在及边界条件类型信息
- 作用：判断目标边界条件是否存在，并识别其边界条件类型

### 2. 根据边界条件类型进入对应skill

当 `AI.AI_Solution().getBCSType('边界条件名称')` 返回类型后，按下列规则选择对应skill：

- 返回 `BCS`：进入 `radioss_bcs` skill，执行标准边界条件编辑。
- 返回 `BSSCyclic`：当前暂不支持对应skill，停止编辑并提示该类型尚未实现。
- 返回 `BCSLAGMUL`：当前暂不支持对应skill，停止编辑并提示该类型尚未实现。
- 返回空值、失败信息或未知类型：停止编辑，并提示边界条件不存在或当前暂不支持该边界条件类型。

## 调用示例

### 示例1：先查询边界条件类型

```python
AI.AI_Solution().getBCSType('BCS-1')
```

### 示例2：边界条件类型为BCS时

先确认类型：

```python
AI.AI_Solution().getBCSType('BCS-1')
```

若返回 `BCS`，则在 `radioss_bcs` skill 中执行：

```python
p=BCS.BCS.GetBCS('BCS-1')
p.setNodeGroup('NodeGroup-1')
p.setFreedomFixed(1, True)
p.setFreedomFixed(2, False)
```

## 使用约束

1. 本skill只负责识别边界条件是否存在以及边界条件类型，不直接定义某一种边界条件的全部编辑接口。
2. 识别出边界条件类型后，应切换到对应边界条件skill执行具体编辑操作。
3. 若边界条件不存在，不应继续执行编辑命令。
4. 若边界条件类型当前没有对应skill，不应继续编辑，应提示当前暂不支持。
5. 编辑已有边界条件前，优先执行一次 `AI.AI_Solution().getBCSType('边界条件名')`。
6. 当前仅实现 `BCS` 类型skill；`BSSCyclic` 和 `BCSLAGMUL` 暂未实现。

## 建议执行策略

1. 从用户输入中提取目标边界条件名称和待修改参数。
2. 调用 `AI.AI_Solution().getBCSType('边界条件名称')`。
3. 判断返回结果是否有效。
4. 根据边界条件类型选择对应skill。
5. 在对应skill中生成并执行具体的边界条件编辑脚本。
6. 将具体编辑接口的返回结果反馈给用户。