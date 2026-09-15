# Radioss 编辑探针

## 功能描述

本技能用于在Radioss中编辑已有探针。
执行编辑前，先通过 `AI.AI_Solution().getProbeType(探针名称)` 判断探针是否存在，并获取探针类型。
确认探针类型后，再进入对应探针类型的skill中执行具体编辑操作。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand先查询探针类型，再根据返回类型选择对应skill执行编辑。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 编辑流程

### 1. 查询探针是否存在及探针类型

```python
AI.AI_Solution().getProbeType('Probe-1')
```

说明：
- 参数：探针名称（字符串）
- 返回：探针是否存在及探针类型信息
- 作用：判断目标探针是否存在，并识别其探针类型

### 2. 根据探针类型进入对应skill

当 `AI.AI_Solution().getProbeType('探针名称')` 返回类型后，按下列规则选择对应skill：

- 返回 `ProbeAcceleration`：进入 `radioss_probe_accelerometer` skill，执行加速度探针编辑。
- 返回 `ProbeSection`：进入 `radioss_probe_section` skill，执行截面探针编辑。
- 返回空值、失败信息或未知类型：停止编辑，并提示探针不存在或当前暂不支持该探针类型。

## 调用示例

### 示例1：先查询探针类型

```python
AI.AI_Solution().getProbeType('Probe-1')
```

### 示例2：探针类型为ProbeAcceleration时

先确认类型：

```python
AI.AI_Solution().getProbeType('Probe-1')
```

若返回 `ProbeAcceleration`，则在 `radioss_probe_accelerometer` skill 中执行：

```python
p=Probe.ProbeAccelerometer.GetProbeAccelerometer('Probe-1')
p.setSingleNodeGroup('NodeGroup-1')
p.setCutOffFreq(1000.0)
```

### 示例3：探针类型为ProbeSection时

先确认类型：

```python
AI.AI_Solution().getProbeType('Probe-1')
```

若返回 `ProbeSection`，则在 `radioss_probe_section` skill 中执行：

```python
p=Probe.ProbeSection.GetProbeSection('Probe-1')
p.setSingleNodeGroup1('NodeGroup-1')
p.setShellElementGroup('ShellSet-1')
```

## 使用约束

1. 本skill只负责识别探针是否存在以及探针类型，不直接定义某一种探针的全部编辑接口。
2. 识别出探针类型后，应切换到对应探针skill执行具体编辑操作。
3. 若探针不存在，不应继续执行编辑命令。
4. 若探针类型当前没有对应skill，不应继续编辑，应提示当前暂不支持。
5. 编辑已有探针前，优先执行一次 `AI.AI_Solution().getProbeType('探针名')`。
6. `getProbeType` 返回的加速度探针类型字符串是 `ProbeAcceleration`，对应的 Python 包装类名称为 `Probe.ProbeAccelerometer`。

## 建议执行策略

1. 从用户输入中提取目标探针名称和待修改参数。
2. 调用 `AI.AI_Solution().getProbeType('探针名称')`。
3. 判断返回结果是否有效。
4. 根据探针类型选择对应skill。
5. 在对应skill中生成并执行具体的探针编辑脚本。
6. 将具体编辑接口的返回结果反馈给用户。