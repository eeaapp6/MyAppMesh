# Elastic材料示例代码

## 示例1：创建Elastic材料并设置部分参数（指定名称）
material=Material.Elastic()
material.createMaterial('Material-1')  # 传递材料名称，创建一个Elastic材料对象
material.setDensity(7.85e-06)
material.setYoungsModulus(210)
material.setPoissonsRatio(0.3)

## 根据材料名称获取类型
AI.AI_Properties().getMaterialType('Material-1')

## 根据材料名称获取Elastic材料操作对象
material=Material.Elastic.GetElastic('Material-1')

## 修改材料名称
material.setName('Material-2')

## 设置密度
material.setDensity(7.85e-06)

## 设置弹性模量
material.setYoungsModulus(210)

## 设置泊松比
material.setPoissonsRatio(0.3)
