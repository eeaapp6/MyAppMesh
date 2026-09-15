# Radioss 编辑失效模型

## 功能描述

本技能用于在Radioss中编辑已有失效模型。
执行编辑前，先通过 `AI.AI_Properties().getFailureModelType(失效模型名称)` 判断失效模型是否存在，并获取失效模型类型。
确认失效模型类型后，再进入对应失效模型类型的skill中执行具体编辑操作。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand先查询失效模型类型，再根据返回类型选择对应skill执行编辑。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 编辑流程

### 1. 查询失效模型是否存在及失效模型类型

```python
AI.AI_Properties().getFailureModelType('Failure-1')
```

说明：
- 参数：失效模型名称（字符串）
- 返回：失效模型是否存在及失效模型类型信息
- 作用：判断目标失效模型是否存在，并识别其失效模型类型

### 2. 根据失效模型类型进入对应skill

当 `AI.AI_Properties().getFailureModelType('失效模型名称')` 返回类型后，按下列规则选择对应skill：

- 返回 `BIQUAD`：进入 `radioss_failure_BIQUAD` skill，执行BIQUAD失效模型编辑。
- 返回 `JOHNSON`：进入 `radioss_failure_JOHNSON` skill，执行JOHNSON失效模型编辑。
- 返回空值、失败信息或未知类型：停止编辑，并提示失效模型不存在或当前暂不支持该失效模型类型。

## 调用示例

### 示例1：先查询失效模型类型

```python
AI.AI_Properties().getFailureModelType('Failure-1')
```

### 示例2：失效模型类型为BIQUAD时

先确认类型：

```python
AI.AI_Properties().getFailureModelType('Failure-1')
```

若返回 `BIQUAD`，则在 `radioss_failure_BIQUAD` skill 中执行：

```python
failure=Failure.BIQUAD.GetBIQUAD('Failure-1')
failure.setC1(0.1)
failure.setMFlag(0)
```

### 示例3：失效模型类型为JOHNSON时

先确认类型：

```python
AI.AI_Properties().getFailureModelType('Failure-1')
```

若返回 `JOHNSON`，则在 `radioss_failure_JOHNSON` skill 中执行：

```python
failure=Failure.JOHNSON.GetJOHNSON('Failure-1')
failure.setD1(0.1)
failure.setIfailsh(1)
```

## 使用约束

1. 本skill只负责识别失效模型是否存在以及失效模型类型，不直接定义某一种失效模型的全部编辑接口。
2. 识别出失效模型类型后，应切换到对应失效模型skill执行具体编辑操作。
3. 若失效模型不存在，不应继续执行编辑命令。
4. 若失效模型类型当前没有对应skill，不应继续编辑，应提示当前暂不支持。
5. 编辑已有失效模型前，优先执行一次 `AI.AI_Properties().getFailureModelType('失效模型名')`。

## 建议执行策略

1. 从用户输入中提取目标失效模型名称和待修改参数。
2. 调用 `AI.AI_Properties().getFailureModelType('失效模型名称')`。
3. 判断返回结果是否有效。
4. 根据失效模型类型选择对应skill。
5. 在对应skill中生成并执行具体的失效模型编辑脚本。
6. 将具体编辑接口的返回结果反馈给用户。
