# Radioss 编辑状态方程

## 功能描述

本技能用于在Radioss中编辑已有状态方程。
执行编辑前，先通过 `AI.AI_Properties().getEquationOfStateType(状态方程名称)` 判断状态方程是否存在，并获取状态方程类型。
确认状态方程类型后，再进入对应状态方程类型的skill中执行具体编辑操作。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand先查询状态方程类型，再根据返回类型选择对应skill执行编辑。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 编辑流程

### 1. 查询状态方程是否存在及状态方程类型

```python
AI.AI_Properties().getEquationOfStateType('EOS-1')
```

说明：
- 参数：状态方程名称（字符串）
- 返回：状态方程是否存在及状态方程类型信息
- 作用：判断目标状态方程是否存在，并识别其状态方程类型

### 2. 根据状态方程类型进入对应skill

当 `AI.AI_Properties().getEquationOfStateType('状态方程名称')` 返回类型后，按下列规则选择对应skill：

- 返回 `Polynomial`：进入 `radioss_eos_polynomial` skill，执行Polynomial状态方程编辑。
- 返回空值、失败信息或未知类型：停止编辑，并提示状态方程不存在或当前暂不支持该状态方程类型。

## 调用示例

### 示例1：先查询状态方程类型

```python
AI.AI_Properties().getEquationOfStateType('EOS-1')
```

### 示例2：状态方程类型为Polynomial时

先确认类型：

```python
AI.AI_Properties().getEquationOfStateType('EOS-1')
```

若返回 `Polynomial`，则在 `radioss_eos_polynomial` skill 中执行：

```python
eos=EOS.EOSPolynomial.GetEOSPolynomial('EOS-1')
eos.setC0(0.0)
eos.setP0(1.0)
```

## 使用约束

1. 本skill只负责识别状态方程是否存在以及状态方程类型，不直接定义某一种状态方程的全部编辑接口。
2. 识别出状态方程类型后，应切换到对应状态方程skill执行具体编辑操作。
3. 若状态方程不存在，不应继续执行编辑命令。
4. 若状态方程类型当前没有对应skill，不应继续编辑，应提示当前暂不支持。
5. 编辑已有状态方程前，优先执行一次 `AI.AI_Properties().getEquationOfStateType('状态方程名')`。

## 建议执行策略

1. 从用户输入中提取目标状态方程名称和待修改参数。
2. 调用 `AI.AI_Properties().getEquationOfStateType('状态方程名称')`。
3. 判断返回结果是否有效。
4. 根据状态方程类型选择对应skill。
5. 在对应skill中生成并执行具体的状态方程编辑脚本。
6. 将具体编辑接口的返回结果反馈给用户。
