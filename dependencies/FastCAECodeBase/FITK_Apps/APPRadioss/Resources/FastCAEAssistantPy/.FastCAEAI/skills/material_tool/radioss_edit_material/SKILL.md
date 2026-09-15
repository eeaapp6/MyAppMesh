# Radioss 编辑材料属性

## 功能描述

本技能用于在Radioss中编辑已有材料属性。
执行编辑前，先通过 `AI.AI_Properties().getMaterialType(材料名称)` 判断材料是否存在，并获取材料类型。
确认材料类型后，再进入对应材料类型的skill中执行具体编辑操作。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand先查询材料类型，再根据返回类型选择对应skill执行编辑。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 编辑流程

### 1. 查询材料是否存在及材料类型

```python
AI.AI_Properties().getMaterialType('Material-1')
```

说明：
- 参数：材料名称（字符串）
- 返回：材料是否存在及材料类型信息
- 作用：判断目标材料是否存在，并识别其材料类型

### 2. 根据材料类型进入对应skill

当 `AI.AI_Properties().getMaterialType('材料名称')` 返回类型后，按下列规则选择对应skill：

- 返回 `Elastic`：进入 `radioss_material_elastic` skill，执行Elastic材料属性编辑。
- 返回 `Hydro`：进入 `radioss_material_hydro` skill，执行Hydro材料属性编辑。
- 返回 `PlasBrit`：进入 `radioss_material_plasBrit` skill，执行PlasBrit材料属性编辑。
- 返回 `PlasJohns`：进入 `radioss_material_plasJohns` skill，执行PlasJohns材料属性编辑。
- 返回 `PlasTab`：进入 `radioss_material_plasTab` skill，执行PlasTab材料属性编辑。
- 返回空值、失败信息或未知类型：停止编辑，并提示材料不存在或当前暂不支持该材料类型。

## 调用示例

### 示例1：先查询材料类型

```python
AI.AI_Properties().getMaterialType('Material-1')
```

### 示例2：材料类型为Elastic时

先确认类型：

```python
AI.AI_Properties().getMaterialType('Material-1')
```

若返回 `Elastic`，则在 `radioss_material_elastic` skill 中执行：

```python
material=Material.Elastic.GetElastic('Material-1')
material.setDensity(7.85e-06)
```

### 示例3：材料类型为PlasJohns时

先确认类型：

```python
AI.AI_Properties().getMaterialType('Material-1')
```

若返回 `PlasJohns`，则在 `radioss_material_plasJohns` skill 中执行：

```python
material=Material.PlasJohns.GetPlasJohns('Material-1')
material.setType(0)
material.setPoissonsRatio(0.3)
```

### 示例4：材料类型为PlasBrit时

先确认类型：

```python
AI.AI_Properties().getMaterialType('Material-1')
```

若返回 `PlasBrit`，则在 `radioss_material_plasBrit` skill 中执行：

```python
material=Material.PlasBrit.GetPlasBrit('Material-1')
material.setPlasticityYieldStress(0.5)
material.setPoissonsRatio(0.3)
```

### 示例5：材料类型为PlasTab时

先确认类型：

```python
AI.AI_Properties().getMaterialType('Material-1')
```

若返回 `PlasTab`，则在 `radioss_material_plasTab` skill 中执行：

```python
material=Material.PlasTab.GetPlasTab('Material-1')
material.setFailureStrain(0)
material.setNumberOfFunctions(2)
```

### 示例6：材料类型为Hydro时

先确认类型：

```python
AI.AI_Properties().getMaterialType('Material-1')
```

若返回 `Hydro`，则在 `radioss_material_hydro` skill 中执行：

```python
material=Material.Hydro.GetHydro('Material-1')
material.setReferenceDensity(1.0e-06)
material.setEOS('EOS-1')
```

## 使用约束

1. 本skill只负责识别材料是否存在以及材料类型，不直接定义某一种材料的全部编辑接口。
2. 识别出材料类型后，应切换到对应材料skill执行具体编辑操作。
3. 若材料不存在，不应继续执行编辑命令。
4. 若材料类型当前没有对应skill，不应继续编辑，应提示当前暂不支持。
5. 编辑已有材料前，优先执行一次 `AI.AI_Properties().getMaterialType('材料名')`。

## 建议执行策略

1. 从用户输入中提取目标材料名称和待修改属性。
2. 调用 `AI.AI_Properties().getMaterialType('材料名称')`。
3. 判断返回结果是否有效。
4. 根据材料类型选择对应skill。
5. 在对应skill中生成并执行具体的材料编辑脚本。
6. 将具体编辑接口的返回结果反馈给用户。
