# 动力学绑定相互作用示例代码

## 示例1：创建动力学绑定相互作用并设置部分参数（指定名称）
p=InterTied.InterTied()
p.createInteraction('Interaction-1')
p.setMasterSurface('Surface-1')
p.setNodeGroup('NodeGroup-1')
p.setLevel(0)
p.setSearchDistance(0.0)
p.setIgnoreFlag(0)
p.setSpotWeldFormulationFlag(0)
p.setSearchFormulationFlag(2)
p.setNodeDeletionFlag(1000)

## 根据相互作用名称获取类型
AI.AI_Solution().getInteractionType('Interaction-1')

## 根据名称获取动力学绑定相互作用操作对象
p=InterTied.InterTied.GetInterTied('Interaction-1')

## 修改相互作用名称
p.setName('Interaction-2')

## 设置主面
p.setMasterSurface('Surface-2')

## 设置从节点组
p.setNodeGroup('NodeGroup-2')

## 设置层级
p.setLevel(1)

## 设置搜索距离
p.setSearchDistance(2.5)

## 设置界面脚本直接输出的默认参数
p.setIgnoreFlag(0)
p.setSpotWeldFormulationFlag(20)
p.setSearchFormulationFlag(2)
p.setNodeDeletionFlag(1000)

## 设置部分可选参数
p.setRupt(0)
p.setIfilter(0)
p.setFctIDsr('Curve-1')
p.setFctIDsn('Curve-2')
p.setFctIDst('Curve-3')
p.setIsym(0)
p.setMaxNDist(1.0)
p.setMaxTDist(1.0)
p.setFscaleStress(1.0)
p.setFscaleStrRate(1.0)
p.setFscaleDist(1.0)
p.setAlpha(1.0)
p.setArea(0.0)
p.setStfac(1.0)
p.setVisc(0.05)
p.setIstf(0)
