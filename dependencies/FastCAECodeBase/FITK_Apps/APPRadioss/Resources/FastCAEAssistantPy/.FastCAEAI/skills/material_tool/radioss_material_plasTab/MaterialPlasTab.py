# PlasTab材料示例代码

## 示例1：创建PlasTab材料并设置部分参数（指定名称）
material=Material.PlasTab()
material.createMaterial('Material-1')  # 传递材料名称，创建一个PlasTab材料对象
material.setDensity(7.85e-06)
material.setYoungsModulus(210)
material.setPoissonsRatio(0.3)
material.setFailureStrain(0)
material.setTensileFailureStrain(0)
material.setMaxTensileFailureStrain(0)
material.setNumberOfFunctions(2)
material.setFunction(1, 'Curve-1')
material.setFctScale(1, 1.0)
material.setEpsilon(1, 0.0)
material.setFunction(2, 'Curve-2')
material.setFctScale(2, 1.0)
material.setEpsilon(2, 1.0)
material.setSmoothStrainRateOptFlag(0)
material.setChard(0)
material.setFcut(0)
material.setVpFlag(0)
material.setTensileStrainForElementDeletion(0)
material.setFctIDp(0)
material.setFscaleFactor(1.0)
material.setFctIODE(0)
material.setEint(0)
material.setCe(0)

## 根据材料名称获取类型
AI.AI_Properties().getMaterialType('Material-1')

## 根据材料名称获取PlasTab材料操作对象
material=Material.PlasTab.GetPlasTab('Material-1')

## 修改材料名称
material.setName('Material-2')

## 设置密度
material.setDensity(7.85e-06)

## 设置弹性模量
material.setYoungsModulus(210)

## 设置泊松比
material.setPoissonsRatio(0.3)

## 设置失效应变
material.setFailureStrain(0)

## 设置拉伸失效应变
material.setTensileFailureStrain(0)

## 设置最大拉伸失效应变
material.setMaxTensileFailureStrain(0)

## 设置函数数量
material.setNumberOfFunctions(2)

## 设置第1个屈服应力函数曲线
material.setFunction(1, 'Curve-1')

## 设置第1个函数缩放因子
material.setFctScale(1, 1.0)

## 设置第1个应变率参数
material.setEpsilon(1, 0.0)

## 设置第2个屈服应力函数曲线
material.setFunction(2, 'Curve-2')

## 设置第2个函数缩放因子
material.setFctScale(2, 1.0)

## 设置第2个应变率参数
material.setEpsilon(2, 1.0)

## 设置平滑应变率选项标志
material.setSmoothStrainRateOptFlag(0)

## 设置硬化系数
material.setChard(0)

## 设置应变率平滑截止频率
material.setFcut(0)

## 设置应变率选择标志
material.setVpFlag(0)

## 设置元素删除拉伸应变
material.setTensileStrainForElementDeletion(0)

## 设置压力屈服因子函数 ID
material.setFctIDp(0)

## 设置压力屈服因子缩放因子
material.setFscaleFactor(1.0)

## 设置杨氏模量函数标识
material.setFctIODE(0)

## 设置饱和杨氏模量
material.setEint(0)

## 设置杨氏模量演化参数
material.setCe(0)
