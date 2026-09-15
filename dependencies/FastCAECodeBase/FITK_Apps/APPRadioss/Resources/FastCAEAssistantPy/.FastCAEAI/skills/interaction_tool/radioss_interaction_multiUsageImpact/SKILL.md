# Radioss 多用途冲击相互作用

## 功能描述

本技能用于在Radioss中创建和修改多用途冲击相互作用，支持设置名称、主面、从节点组、摩擦、间隙、刚度、热交换以及相关可选参数。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行多用途冲击相互作用配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建多用途冲击相互作用

### 1. 创建多用途冲击相互作用

创建多用途冲击相互作用操作对象后，可以创建多用途冲击相互作用。

```python
p=InterMultiUsageImpact.InterMultiUsageImpact()
p.createInteraction('Interaction-1')
```

- 参数：相互作用名称（字符串）
- 使用指定名称创建多用途冲击相互作用
- 注意：如果已存在同名相互作用会创建失败

**完整示例：创建多用途冲击相互作用并设置部分参数**

```python
p=InterMultiUsageImpact.InterMultiUsageImpact()
p.createInteraction('Interaction-1')
p.setMasterSurface('Surface-1')
p.setNodeGroup('NodeGroup-1')
p.setIthe(1)
p.setIcurv(0)
p.setIadm(0)
p.setFscaleGap(1.0)
p.setGAPmax(0.0)
p.setFpenmax(0.0)
p.setItied(0)
p.setSTmin(0.0)
p.setSTmax(1e30)
p.setMeshSize(0.4)
p.setDtmin(0.0)
p.setStfac(1.0)
p.setFric(0.0)
p.setGapmin(1.0)
p.setTstart(0.0)
p.setTstop(0.0)
p.setIbc(0)
p.setVisS(0.05)
p.setVisF(1.0)
p.setBumult(0.2)
p.setIfric(0)
p.setIfiltr(0)
p.setXfreq(0.0)
p.setSensID(0)
p.setAscaleF(1.0)
p.setFricID(0)
p.setIstf(0)
p.setIgap(0)
p.setIbag(0)
p.setIdel(0)
p.setIremGap(0)
p.setIremI2(0)
p.setInacti(0)
p.setIform(0)
p.setC1(0.0)
p.setC2(0.0)
p.setC3(0.0)
p.setC4(0.0)
p.setC5(0.0)
p.setC6(0.0)
p.setNRadm(0)
p.setPadm(0.0)
p.setAngladm(0.0)
p.setKthe(0.0)
p.setTint(0.0)
p.setItheForm(0)
p.setAscaleK(1.0)
p.setFrad(0.0)
p.setDrad(0.0)
p.setFheats(0.0)
p.setFheatm(0.0)
```

## 修改多用途冲击相互作用

### 1. 获取多用途冲击相互作用对象

根据相互作用名称获取已存在的多用途冲击相互作用对象：
- 参数：相互作用名称（字符串）
- 编辑前可先通过 `AI.AI_Solution().getInteractionType('Interaction-1')` 判断对象是否为 `MultiUsageImpact` 类型，如果不是则不在这个skill中编辑，查找别的skill。

```python
p=InterMultiUsageImpact.InterMultiUsageImpact.GetInterMultiUsageImpact('Interaction-1')
```

## 修改多用途冲击相互作用参数

获取对象后，可修改以下属性：

### 1. 基础参数

```python
p.setName('Interaction-2')
p.setMasterSurface('Surface-1')
p.setNodeGroup('NodeGroup-1')
```

### 2. 刚度、间隙与摩擦相关参数

```python
p.setIcurv(0)
p.setIadm(0)
p.setFscaleGap(1.0)
p.setGAPmax(0.0)
p.setFpenmax(0.0)
p.setItied(0)
p.setSTmin(0.0)
p.setSTmax(1e30)
p.setMeshSize(0.4)
p.setDtmin(0.0)
p.setStfac(1.0)
p.setFric(0.0)
p.setGapmin(1.0)
p.setTstart(0.0)
p.setTstop(0.0)
p.setIbc(0)
p.setVisS(0.05)
p.setVisF(1.0)
p.setBumult(0.2)
p.setIfric(0)
p.setIfiltr(0)
p.setXfreq(0.0)
p.setSensID(0)
p.setAscaleF(1.0)
p.setFricID(0)
```

### 3. 默认参数与附加标志

```python
p.setIstf(0)
p.setIgap(0)
p.setIbag(0)
p.setIdel(0)
p.setIremGap(0)
p.setIremI2(0)
p.setInacti(0)
p.setIform(0)
```

### 4. 摩擦定律与热交换相关参数

```python
p.setFctIDf('Curve-1')
p.setC1(0.0)
p.setC2(0.0)
p.setC3(0.0)
p.setC4(0.0)
p.setC5(0.0)
p.setC6(0.0)
p.setNRadm(0)
p.setPadm(0.0)
p.setAngladm(0.0)
p.setIthe(1)
p.setKthe(0.0)
p.setFctIDK('Curve-2')
p.setTint(0.0)
p.setItheForm(0)
p.setAscaleK(1.0)
p.setFrad(0.0)
p.setDrad(0.0)
p.setFheats(0.0)
p.setFheatm(0.0)
```

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| InterMultiUsageImpact | - | 创建InterMultiUsageImpact对象 |
| InterMultiUsageImpact.GetInterMultiUsageImpact | string | 根据名称获取已存在的多用途冲击相互作用对象 |
| createInteraction | string | 创建指定名称的多用途冲击相互作用 |
| setName | string | 修改相互作用名称 |
| setMasterSurface | string | 设置主面名称 |
| setNodeGroup | string | 设置从节点组名称 |
| setIthe | int | 设置是否启用热传导或热摩擦 |
| setIcurv | int | 设置带曲率间隙包络线标志 |
| setIadm | int | 设置局部曲率标志 |
| setFscaleGap | double | 设置间隙比例因子 |
| setGAPmax | double | 设置最大间隙 |
| setFpenmax | double | 设置初始渗透最大分数 |
| setItied | int | 设置绑定关联点标志 |
| setSTmin | double | 设置最小刚度 |
| setSTmax | double | 设置最大刚度 |
| setMeshSize | double | 设置网格大小百分比 |
| setDtmin | double | 设置最小接口时间步长 |
| setStfac | double | 设置主侧刚度比例因子 |
| setFric | double | 设置摩擦系数 |
| setGapmin | double | 设置最小间隙 |
| setTstart | double | 设置开始时间 |
| setTstop | double | 设置停用时间 |
| setIbc | int | 设置边界条件停用标志 |
| setVisS | double | 设置界面刚度临界阻尼系数 |
| setVisF | double | 设置界面摩擦临界阻尼系数 |
| setBumult | double | 设置排序因子 |
| setIfric | int | 设置摩擦配方标志 |
| setIfiltr | int | 设置摩擦过滤标志 |
| setXfreq | double | 设置滤波系数 |
| setSensID | int | 设置传感器标识符 |
| setFctIDf | string | 设置摩擦系数曲线 |
| setAscaleF | double | 设置摩擦系数横坐标比例因子 |
| setFricID | int | 设置摩擦标识符 |
| setIstf | int | 设置刚度标志 |
| setIgap | int | 设置间隙单元选项标志 |
| setIbag | int | 设置气囊通风孔关闭标志 |
| setIdel | int | 设置节点退化表面标志 |
| setIremGap | int | 设置次节点停用标志 |
| setIremI2 | int | 设置辅助节点停用标志 |
| setInacti | int | 设置初始穿透刚度停用标志 |
| setIform | int | 设置摩擦惩罚公式类型 |
| setC1 | double | 设置摩擦定律系数 C1 |
| setC2 | double | 设置摩擦定律系数 C2 |
| setC3 | double | 设置摩擦定律系数 C3 |
| setC4 | double | 设置摩擦定律系数 C4 |
| setC5 | double | 设置摩擦定律系数 C5 |
| setC6 | double | 设置摩擦定律系数 C6 |
| setNRadm | int | 设置 90 度半径元素数 |
| setPadm | double | 设置渗透百分比标准 |
| setAngladm | double | 设置角度标准 |
| setKthe | double | 设置热交换系数 |
| setFctIDK | string | 设置热交换曲线 |
| setTint | double | 设置接口温度 |
| setItheForm | int | 设置热接触配方标志 |
| setAscaleK | double | 设置热交换横坐标比例因子 |
| setFrad | double | 设置辐射因子 |
| setDrad | double | 设置辐射最大距离 |
| setFheats | double | 设置二次摩擦发热系数 |
| setFheatm | double | 设置主摩擦发热系数 |

## 注意事项

1. 默认使用当前有效的Solution，无需额外判断或设置。
2. 同名相互作用已存在时，`createInteraction` 会失败。
3. 编辑前建议先通过 `InterMultiUsageImpact.InterMultiUsageImpact.GetInterMultiUsageImpact` 获取对象。
4. 编辑已有相互作用前，建议先通过 `AI.AI_Solution().getInteractionType('相互作用名')` 确认对象类型。
5. `setMasterSurface`、`setNodeGroup`、`setFctIDf`、`setFctIDK` 需要传入当前模型中已存在的对象名称。
6. 界面 `writePythonScript` 会按当前界面状态输出大量参数；若界面未选择曲线，则不会输出 `setFctIDf` 或 `setFctIDK`。
7. `setNodeID1` 和 `setNodeID2` 当前在wrapper中未开放，本skill不包含这两个接口。
