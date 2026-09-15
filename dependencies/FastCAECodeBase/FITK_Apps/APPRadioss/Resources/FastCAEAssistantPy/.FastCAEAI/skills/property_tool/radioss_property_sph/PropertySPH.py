# SPH属性示例代码

## 示例1：创建SPH属性并设置部分参数（指定名称）
property=Property.SPH()
property.createProperty('Property-1')  # 传递属性名称，创建一个SPH属性对象
property.setMP(1.0)
property.setQA(0.0)
property.setQB(0.0)
property.setACS(0.0)
property.setHID(0)
property.setOrderID(0)
property.setH(0.0)
property.setStab(0.0)

## 根据属性名称获取类型
AI.AI_Properties().getPropertyType('Property-1')

## 根据属性名称获取SPH属性操作对象
property=Property.SPH.GetSPH('Property-1')

## 修改属性名称
property.setName('Property-2')

## 设置粒子质量
property.setMP(1.0)

## 设置二次体积粘性
property.setQA(0.0)

## 设置线性体积粘性
property.setQB(0.0)

## 设置保守平滑系数
property.setACS(0.0)

## 设置平滑长度变化选项
property.setHID(0)

## 设置SPH修正阶次
property.setOrderID(0)

## 设置平滑长度
property.setH(0.0)

## 设置抗拉不稳定系数
property.setStab(0.0)
