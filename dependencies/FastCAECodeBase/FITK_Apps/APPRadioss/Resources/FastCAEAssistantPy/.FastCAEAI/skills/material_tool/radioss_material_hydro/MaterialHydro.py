# Hydro材料示例代码

## 示例1：创建Hydro材料并设置部分参数（指定名称）
material=Material.Hydro()
material.createMaterial('Material-1')  # 传递材料名称，创建一个Hydro材料对象
material.setDensity(1.0e-06)
material.setReferenceDensity(1.0e-06)
material.setKinematicViscosity(0.0)
material.setPressureCut(0.0)
material.setEOS('EOS-1')

## 根据材料名称获取类型
AI.AI_Properties().getMaterialType('Material-1')

## 根据材料名称获取Hydro材料操作对象
material=Material.Hydro.GetHydro('Material-1')

## 修改材料名称
material.setName('Material-2')

## 设置密度
material.setDensity(1.0e-06)

## 设置参考密度
material.setReferenceDensity(1.0e-06)

## 设置运动粘度
material.setKinematicViscosity(0.0)

## 设置最小压力截断值
material.setPressureCut(0.0)

## 设置状态方程
material.setEOS('EOS-1')
