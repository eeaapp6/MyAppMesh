# Radioss 加速度探针

## 功能描述

本技能用于在Radioss中创建和修改加速度探针，支持设置探针名称、坐标系ID、单点节点组以及截止频率。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行加速度探针配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建加速度探针

### 1. 创建加速度探针

创建加速度探针操作对象后，可以创建加速度探针。

```python
p=Probe.ProbeAccelerometer()
p.createProbe('Probe-1')
```

- 参数：探针名称（字符串）
- 使用指定名称创建加速度探针
- 注意：如果已存在同名探针会创建失败

**完整示例：创建加速度探针并设置部分参数**

```python
p=Probe.ProbeAccelerometer()
p.createProbe('Probe-1')
p.setSystemID(0)
p.setSingleNodeGroup('NodeGroup-1')
p.setCutOffFreq(1000.0)
```

## 修改加速度探针

### 1. 获取加速度探针对象

根据探针名称获取已存在的加速度探针对象：
- 参数：探针名称（字符串）
- 编辑前可先通过 `AI.AI_Solution().getProbeType('Probe-1')` 判断对象是否为 `ProbeAcceleration` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
p=Probe.ProbeAccelerometer.GetProbeAccelerometer('Probe-1')
```

## 修改加速度探针参数

获取对象后，可修改以下属性：

### 1. 修改探针名称

```python
p.setName('Probe-2')
```

设置加速度探针的新名称：
- 参数：探针名称（字符串）

### 2. 设置坐标系ID

```python
p.setSystemID(0)
```

设置加速度探针的坐标系ID：
- 参数：坐标系ID（整数）

### 3. 设置单点节点组

```python
p.setSingleNodeGroup('NodeGroup-1')
```

设置加速度探针关联的单点节点组：
- 参数：节点组名称（字符串）
- 特殊值：`'None'` 表示清除节点组设置

### 4. 设置截止频率

```python
p.setCutOffFreq(1000.0)
```

设置加速度探针的截止频率：
- 参数：截止频率（浮点数）

### 5. 获取首节点ID

```python
p.getNodeID()
```

获取当前加速度探针关联的首节点ID：
- 返回：节点ID（整数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| ProbeAccelerometer | - | 创建ProbeAccelerometer对象 |
| ProbeAccelerometer.GetProbeAccelerometer | string | 根据名称获取已存在的加速度探针对象 |
| createProbe | string | 创建指定名称的加速度探针 |
| setName | string | 修改探针名称 |
| setSystemID | int | 设置坐标系ID |
| setSingleNodeGroup | string | 设置单点节点组 |
| setCutOffFreq | double | 设置截止频率 |
| getNodeID | - | 获取首节点ID |

## 注意事项

1. 默认使用当前有效的Solution，无需额外判断或设置。
2. 同名加速度探针已存在时，`createProbe` 会失败。
3. 编辑前建议先通过 `Probe.ProbeAccelerometer.GetProbeAccelerometer` 获取对象。
4. 编辑已有探针前，建议先通过 `AI.AI_Solution().getProbeType('探针名')` 确认对象类型。
5. `AI.AI_Solution().getProbeType` 对加速度探针返回的是 `ProbeAcceleration`，对应的 Python 包装类名称为 `Probe.ProbeAccelerometer`。
6. `setSingleNodeGroup('None')` 表示清除单点节点组设置。
7. 示例中的参数顺序与界面 `writePythonScript` 输出顺序保持一致。
