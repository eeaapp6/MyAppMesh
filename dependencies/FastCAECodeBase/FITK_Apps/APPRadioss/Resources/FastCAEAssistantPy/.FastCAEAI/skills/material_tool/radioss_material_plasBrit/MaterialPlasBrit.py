# PlasBrit材料示例代码

## 示例1：创建PlasBrit材料并设置部分参数（指定名称）
material=Material.PlasBrit()
material.createMaterial('Material-1')  # 传递材料名称，创建一个PlasBrit材料对象
material.setDensity(7.85e-06)
material.setYoungsModulus(210)
material.setPoissonsRatio(0.3)
material.setPlasticityYieldStress(0.5)
material.setPlasticityHardeningParameter(0.5)
material.setPlasticityHardeningExponent(0.5)
material.setPlasticityMaximumStress(0.8)
material.setStrainRateCoefficient(0)
material.setReferenceStrainRate(0)
material.setStrainRateComputation(0)
material.setStrainRateSmoothing(0)
material.setCutoffFreq(0)
material.setTensileFilureStrain1(0)
material.setMaximumTensileFilureStrain1(0)
material.setMaximumDamageFactor1(0)
material.setMaximumTensileStrain1(0)
material.setTensileFilureStrain2(0)
material.setMaximumTensileFilureStrain2(0)
material.setMaximumDamageFactor2(0)
material.setMaximumTensileStrain2(0)

## 根据材料名称获取类型
AI.AI_Properties().getMaterialType('Material-1')

## 根据材料名称获取PlasBrit材料操作对象
material=Material.PlasBrit.GetPlasBrit('Material-1')

## 修改材料名称
material.setName('Material-2')

## 设置密度
material.setDensity(7.85e-06)

## 设置弹性模量
material.setYoungsModulus(210)

## 设置泊松比
material.setPoissonsRatio(0.3)

## 设置塑性屈服应力
material.setPlasticityYieldStress(0.5)

## 设置塑性硬化参数
material.setPlasticityHardeningParameter(0.5)

## 设置塑性硬化指数
material.setPlasticityHardeningExponent(0.5)

## 设置塑性最大应力
material.setPlasticityMaximumStress(0.8)

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

## 设置方向1拉伸破坏应变
material.setTensileFilureStrain1(0)

## 设置方向1最大拉伸破坏应变
material.setMaximumTensileFilureStrain1(0)

## 设置方向1最大损伤因子
material.setMaximumDamageFactor1(0)

## 设置方向1最大拉伸应变
material.setMaximumTensileStrain1(0)

## 设置方向2拉伸破坏应变
material.setTensileFilureStrain2(0)

## 设置方向2最大拉伸破坏应变
material.setMaximumTensileFilureStrain2(0)

## 设置方向2最大损伤因子
material.setMaximumDamageFactor2(0)

## 设置方向2最大拉伸应变
material.setMaximumTensileStrain2(0)
