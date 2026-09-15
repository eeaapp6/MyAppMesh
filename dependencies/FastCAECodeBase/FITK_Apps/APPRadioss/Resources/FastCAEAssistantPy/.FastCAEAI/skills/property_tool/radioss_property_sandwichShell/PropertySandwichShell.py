# SandwichShell属性示例代码

## 示例1：创建SandwichShell属性并设置部分参数（指定名称）
property=Property.SandwichShell()
property.createProperty('Property-1')  # 传递属性名称，创建一个SandwichShell属性对象
property.setThick(1.8)
property.setIshell(24)
property.setIsmstr(0)
property.setIsh3n(0)
property.setIdril(0)
property.setPthickFail(0)
property.setHm(0)
property.setHf(0)
property.setHr(0)
property.setDm(0)
property.setDn(0)
property.setNumOfLayers(2)
property.setIstrain(0)
property.setAshear(0)
property.setIthick(0)
property.setIplas(0)
property.setVx(1.0)
property.setVy(0.0)
property.setVz(0.0)
property.setIorth(0)
property.setIpos(0)
property.setLayerAngle(1, 0.0)
property.setLayerThickness(1, 0.9)
property.setLayerPositionZ(1, -0.45)
property.setLayerMaterial(1, 'Material-1')
property.setLayerFailureWeight(1, 1.0)
property.setLayerAngle(2, 0.0)
property.setLayerThickness(2, 0.9)
property.setLayerPositionZ(2, 0.45)
property.setLayerMaterial(2, 'Material-1')
property.setLayerFailureWeight(2, 1.0)

## 根据属性名称获取类型
AI.AI_Properties().getPropertyType('Property-1')

## 根据属性名称获取SandwichShell属性操作对象
property=Property.SandwichShell.GetSandwichShell('Property-1')

## 修改属性名称
property.setName('Property-2')

## 设置壳单元公式标志
property.setIshell(24)

## 设置壳小应变公式标志
property.setIsmstr(0)

## 设置三节点壳单元公式标志
property.setIsh3n(0)

## 设置钻转自由度刚度标志
property.setIdril(0)

## 设置厚度失效比例
property.setPthickFail(0)

## 设置膜向沙漏系数
property.setHm(0)

## 设置离面沙漏系数
property.setHf(0)

## 设置转动沙漏系数
property.setHr(0)

## 设置膜向阻尼
property.setDm(0)

## 设置数值阻尼
property.setDn(0)

## 设置层数
property.setNumOfLayers(2)

## 设置应变选项
property.setIstrain(0)

## 设置总厚度
property.setThick(1.8)

## 设置剪切因子
property.setAshear(0)

## 设置厚度应力计算标志
property.setIthick(0)

## 设置平面应力塑性标志
property.setIplas(0)

## 设置局部方向 Vx
property.setVx(1.0)

## 设置局部方向 Vy
property.setVy(0.0)

## 设置局部方向 Vz
property.setVz(0.0)

## 设置正交选项
property.setIorth(0)

## 设置位置选项
property.setIpos(0)

## 设置第1层角度
property.setLayerAngle(1, 0.0)

## 设置第1层厚度
property.setLayerThickness(1, 0.9)

## 设置第1层位置 Z
property.setLayerPositionZ(1, -0.45)

## 设置第1层材料
property.setLayerMaterial(1, 'Material-1')

## 设置第1层失效权重
property.setLayerFailureWeight(1, 1.0)
