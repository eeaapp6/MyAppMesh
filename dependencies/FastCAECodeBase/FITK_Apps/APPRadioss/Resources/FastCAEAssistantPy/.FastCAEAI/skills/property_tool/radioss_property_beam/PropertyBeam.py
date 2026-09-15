# Beam属性示例代码

## 示例1：创建Beam属性并设置部分参数（指定名称）
property=Property.Beam()
property.createProperty('Property-1')  # 传递属性名称，创建一个Beam属性对象
property.setCrossSection(1.0)
property.setSmallStrainOption(0)
property.setBeamMembraneDamping(0.0)
property.setBeamFlexuralDamping(0.0)
property.setAreaMomentInertiaIyy(0.0)
property.setAreaMomentInertiaIzz(0.0)
property.setAreaMomentInertiaIxx(0.0)
property.setRotationDOF1('x', 1)
property.setRotationDOF1('y', 0)
property.setRotationDOF1('z', 0)
property.setRotationDOF2('x', 1)
property.setRotationDOF2('y', 0)
property.setRotationDOF2('z', 0)
property.setNeglectsShear(0)

## 根据属性名称获取类型
AI.AI_Properties().getPropertyType('Property-1')

## 根据属性名称获取Beam属性操作对象
property=Property.Beam.GetBeam('Property-1')

## 修改属性名称
property.setName('Property-2')

## 设置小应变选项
property.setSmallStrainOption(0)

## 设置梁膜阻尼
property.setBeamMembraneDamping(0.0)

## 设置梁弯曲阻尼
property.setBeamFlexuralDamping(0.0)

## 设置横截面面积
property.setCrossSection(1.0)

## 设置截面惯性矩 Iyy
property.setAreaMomentInertiaIyy(0.0)

## 设置截面惯性矩 Izz
property.setAreaMomentInertiaIzz(0.0)

## 设置截面惯性矩 Ixx
property.setAreaMomentInertiaIxx(0.0)

## 设置节点1 x 方向旋转自由度
property.setRotationDOF1('x', 1)

## 设置节点1 y 方向旋转自由度
property.setRotationDOF1('y', 0)

## 设置节点1 z 方向旋转自由度
property.setRotationDOF1('z', 0)

## 设置节点2 x 方向旋转自由度
property.setRotationDOF2('x', 1)

## 设置节点2 y 方向旋转自由度
property.setRotationDOF2('y', 0)

## 设置节点2 z 方向旋转自由度
property.setRotationDOF2('z', 0)

## 设置是否忽略剪切
property.setNeglectsShear(0)
