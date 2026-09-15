# Radioss 编辑属性

## 功能描述

本技能用于在Radioss中编辑已有属性。
执行编辑前，先通过 `AI.AI_Properties().getPropertyType(属性名称)` 判断属性是否存在，并获取属性类型。
确认属性类型后，再进入对应属性类型的skill中执行具体编辑操作。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand先查询属性类型，再根据返回类型选择对应skill执行编辑。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 编辑流程

### 1. 查询属性是否存在及属性类型

```python
AI.AI_Properties().getPropertyType('Property-1')
```

说明：
- 参数：属性名称（字符串）
- 返回：属性是否存在及属性类型信息
- 作用：判断目标属性是否存在，并识别其属性类型

### 2. 根据属性类型进入对应skill

当 `AI.AI_Properties().getPropertyType('属性名称')` 返回类型后，按下列规则选择对应skill：

- 返回 `Beam`：进入 `radioss_property_beam` skill，执行Beam属性编辑。
- 返回 `BeamSPR`：进入 `radioss_property_beamSPR` skill，执行BeamSPR属性编辑。
- 返回 `Sandwich`：进入 `radioss_property_sandwichShell` skill，执行SandwichShell属性编辑。
- 返回 `Shell`：进入 `radioss_property_shell` skill，执行Shell属性编辑。
- 返回 `SPH`：进入 `radioss_property_sph` skill，执行SPH属性编辑。
- 返回 `Solid`：进入 `radioss_property_solid` skill，执行Solid属性编辑。
- 返回空值、失败信息或未知类型：停止编辑，并提示属性不存在或当前暂不支持该属性类型。

## 调用示例

### 示例1：先查询属性类型

```python
AI.AI_Properties().getPropertyType('Property-1')
```

### 示例2：属性类型为Shell时

先确认类型：

```python
AI.AI_Properties().getPropertyType('Property-1')
```

若返回 `Shell`，则在 `radioss_property_shell` skill 中执行：

```python
property=Property.Shell.GetShell('Property-1')
property.setThick(1.8)
```

### 示例3：属性类型为Beam时

先确认类型：

```python
AI.AI_Properties().getPropertyType('Property-1')
```

若返回 `Beam`，则在 `radioss_property_beam` skill 中执行：

```python
property=Property.Beam.GetBeam('Property-1')
property.setCrossSection(1.0)
property.setBeamMembraneDamping(0.0)
```

### 示例4：属性类型为BeamSPR时

先确认类型：

```python
AI.AI_Properties().getPropertyType('Property-1')
```

若返回 `BeamSPR`，则在 `radioss_property_beamSPR` skill 中执行：

```python
property=Property.BeamSPR.GetBeamSPR('Property-1')
property.setMass(0.0)
property.setK(1, 0.0)
```

### 示例5：属性类型为Sandwich时

先确认类型：

```python
AI.AI_Properties().getPropertyType('Property-1')
```

若返回 `Sandwich`，则在 `radioss_property_sandwichShell` skill 中执行：

```python
property=Property.SandwichShell.GetSandwichShell('Property-1')
property.setNumOfLayers(2)
property.setLayerMaterial(1, 'Material-1')
```

### 示例6：属性类型为SPH时

先确认类型：

```python
AI.AI_Properties().getPropertyType('Property-1')
```

若返回 `SPH`，则在 `radioss_property_sph` skill 中执行：

```python
property=Property.SPH.GetSPH('Property-1')
property.setMP(1.0)
property.setH(0.0)
```

### 示例7：属性类型为Solid时

先确认类型：

```python
AI.AI_Properties().getPropertyType('Property-1')
```

若返回 `Solid`，则在 `radioss_property_solid` skill 中执行：

```python
property=Property.Solid.GetSolid('Property-1')
property.setIsolid(0)
property.setDn(0)
```

## 使用约束

1. 本skill只负责识别属性是否存在以及属性类型，不直接定义某一种属性的全部编辑接口。
2. 识别出属性类型后，应切换到对应属性skill执行具体编辑操作。
3. 若属性不存在，不应继续执行编辑命令。
4. 若属性类型当前没有对应skill，不应继续编辑，应提示当前暂不支持。
5. 编辑已有属性前，优先执行一次 `AI.AI_Properties().getPropertyType('属性名')`。

## 建议执行策略

1. 从用户输入中提取目标属性名称和待修改参数。
2. 调用 `AI.AI_Properties().getPropertyType('属性名称')`。
3. 判断返回结果是否有效。
4. 根据属性类型选择对应skill。
5. 在对应skill中生成并执行具体的属性编辑脚本。
6. 将具体编辑接口的返回结果反馈给用户。
