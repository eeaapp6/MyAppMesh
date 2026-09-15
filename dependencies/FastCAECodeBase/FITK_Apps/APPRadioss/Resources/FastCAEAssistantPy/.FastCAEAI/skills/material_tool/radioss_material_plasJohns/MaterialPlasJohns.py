# PlasJohns材料示例代码

## 示例1：创建PlasJohns材料并设置部分参数（指定名称）
material=Material.PlasJohns()
material.createMaterial('Material-1')  # 传递材料名称，创建一个PlasJohns材料对象
material.setType(0)
material.setDensity(7.85e-06)
material.setYoungsModulus(210)
material.setPoissonsRatio(0.3)
material.setYieldStress(0.5)
material.setPlasticHardening(0.5)
material.setPlasticHardeningExponent(0.5)
material.setFailureStrain(0)
material.setMaxStress(0)
material.setUTS(0)
material.setEpsilonUTS(1)
material.setStrainRateCoefficient(0)
material.setReferenceStrainRate(0)
material.setStrainRateComputation(0)
material.setStrainRateSmoothing(0)
material.setCutoffFreq(0)
material.setChard(0)
material.setTempExponent(0)
material.setMeltingTemperature(0)
material.setRhoCp(0)
material.setTref(0)

## 根据材料名称获取类型
AI.AI_Properties().getMaterialType('Material-1')

## 根据材料名称获取PlasJohns材料操作对象
material=Material.PlasJohns.GetPlasJohns('Material-1')

## 修改材料名称
material.setName('Material-2')

## 设置材料子类型
material.setType(0)

## 设置密度
material.setDensity(7.85e-06)

## 设置弹性模量
material.setYoungsModulus(210)

## 设置泊松比
material.setPoissonsRatio(0.3)

## 设置屈服强度
material.setYieldStress(0.5)

## 设置塑性硬化参数
material.setPlasticHardening(0.5)

## 设置塑性硬化指数
material.setPlasticHardeningExponent(0.5)

## 设置断裂应变
material.setFailureStrain(0)

## 设置最大应力
material.setMaxStress(0)

## 设置极限拉伸应力
material.setUTS(0)

## 设置极限拉伸应力对应应变
material.setEpsilonUTS(1)

## 设置应变率系数
material.setStrainRateCoefficient(0)

## 设置参考应变率
material.setReferenceStrainRate(0)

## 设置应变率计算标记
material.setStrainRateComputation(0)

## 设置应变率平滑标记
material.setStrainRateSmoothing(0)

## 设置应变率平滑截止频率
material.setCutoffFreq(0)

## 设置硬化系数
material.setChard(0)

## 设置温度指数
material.setTempExponent(0)

## 设置熔化温度
material.setMeltingTemperature(0)

## 设置比热容
material.setRhoCp(0)

## 设置参考温度
material.setTref(0)