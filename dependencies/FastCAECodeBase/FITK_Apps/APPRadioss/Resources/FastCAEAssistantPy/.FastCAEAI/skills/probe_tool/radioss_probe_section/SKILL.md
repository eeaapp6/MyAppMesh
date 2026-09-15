# Radioss 截面探针

## 功能描述

本技能用于在Radioss中创建和修改截面探针，支持设置探针名称、三个单点节点组、节点组、I_SAVE标志、文件名、时间步长、指数平均滤波系数、壳单元组以及局部坐标系中心选项。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行截面探针配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建截面探针

### 1. 创建截面探针

创建截面探针操作对象后，可以创建截面探针。

```python
p=Probe.ProbeSection()
p.createProbe('Probe-1')
```

- 参数：探针名称（字符串）
- 使用指定名称创建截面探针
- 注意：如果已存在同名探针会创建失败

**完整示例：创建截面探针并设置部分参数**

```python
p=Probe.ProbeSection()
p.createProbe('Probe-1')
p.setSingleNodeGroup1('NodeGroup-1')
p.setSingleNodeGroup2('NodeGroup-2')
p.setSingleNodeGroup3('NodeGroup-3')
p.setNodeGroup('NodeGroup-4')
p.setIsave(0)
p.setFileName('section.out')
p.setDt(1.0)
p.setAlpha(0.0)
p.setShellElementGroup('ShellSet-1')
p.setIframe(0)
```

## 修改截面探针

### 1. 获取截面探针对象

根据探针名称获取已存在的截面探针对象：
- 参数：探针名称（字符串）
- 编辑前可先通过 `AI.AI_Solution().getProbeType('Probe-1')` 判断对象是否为 `ProbeSection` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
p=Probe.ProbeSection.GetProbeSection('Probe-1')
```

## 修改截面探针参数

获取对象后，可修改以下属性：

### 1. 修改探针名称

```python
p.setName('Probe-2')
```

设置截面探针的新名称：
- 参数：探针名称（字符串）

### 2. 设置单点节点组1

```python
p.setSingleNodeGroup1('NodeGroup-1')
```

设置截面探针的单点节点组1：
- 参数：节点组名称（字符串）
- 特殊值：`'None'` 表示清除该节点组设置

### 3. 设置单点节点组2

```python
p.setSingleNodeGroup2('NodeGroup-2')
```

设置截面探针的单点节点组2：
- 参数：节点组名称（字符串）
- 特殊值：`'None'` 表示清除该节点组设置

### 4. 设置单点节点组3

```python
p.setSingleNodeGroup3('NodeGroup-3')
```

设置截面探针的单点节点组3：
- 参数：节点组名称（字符串）
- 特殊值：`'None'` 表示清除该节点组设置

### 5. 设置节点组

```python
p.setNodeGroup('NodeGroup-4')
```

设置截面探针的节点组：
- 参数：节点组名称（字符串）
- 特殊值：`'None'` 表示清除节点组设置

### 6. 设置 I_SAVE 标志

```python
p.setIsave(0)
```

设置截面探针的 I_SAVE 标志：
- 参数：标志值（整数）

### 7. 设置文件名

```python
p.setFileName('section.out')
```

设置截面探针的输出文件名：
- 参数：文件名（字符串）

### 8. 设置时间步长

```python
p.setDt(1.0)
```

设置截面探针的时间步长：
- 参数：时间步长（浮点数）

### 9. 设置指数平均滤波系数

```python
p.setAlpha(0.0)
```

设置截面探针的指数平均滤波系数：
- 参数：滤波系数（浮点数）

### 10. 设置壳单元组

```python
p.setShellElementGroup('ShellSet-1')
```

设置截面探针的壳单元组：
- 参数：壳单元组名称（字符串）
- 特殊值：`'None'` 表示清除壳单元组设置

### 11. 设置局部坐标系中心选项

```python
p.setIframe(0)
```

设置截面探针的局部坐标系中心选项：
- 参数：选项值（整数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| ProbeSection | - | 创建ProbeSection对象 |
| ProbeSection.GetProbeSection | string | 根据名称获取已存在的截面探针对象 |
| createProbe | string | 创建指定名称的截面探针 |
| setName | string | 修改探针名称 |
| setSingleNodeGroup1 | string | 设置单点节点组1 |
| setSingleNodeGroup2 | string | 设置单点节点组2 |
| setSingleNodeGroup3 | string | 设置单点节点组3 |
| setNodeGroup | string | 设置节点组 |
| setIsave | int | 设置 I_SAVE 标志 |
| setFileName | string | 设置输出文件名 |
| setDt | double | 设置时间步长 |
| setAlpha | double | 设置指数平均滤波系数 |
| setShellElementGroup | string | 设置壳单元组 |
| setIframe | int | 设置局部坐标系中心选项 |

## 注意事项

1. 默认使用当前有效的Solution，无需额外判断或设置。
2. 同名截面探针已存在时，`createProbe` 会失败。
3. 编辑前建议先通过 `Probe.ProbeSection.GetProbeSection` 获取对象。
4. 编辑已有探针前，建议先通过 `AI.AI_Solution().getProbeType('探针名')` 确认对象类型。
5. `setSingleNodeGroup1('None')`、`setSingleNodeGroup2('None')`、`setSingleNodeGroup3('None')`、`setNodeGroup('None')` 和 `setShellElementGroup('None')` 表示清除对应设置。
6. `setFrameID` 当前在 wrapper 中未开放，本skill不包含参考坐标系设置。
7. 示例中的参数顺序与界面 `writePythonScript` 输出顺序保持一致。