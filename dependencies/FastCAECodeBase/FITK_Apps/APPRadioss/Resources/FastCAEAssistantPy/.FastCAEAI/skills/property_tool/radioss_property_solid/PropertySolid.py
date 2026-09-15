# Solid属性示例代码

## 示例1：创建Solid属性并设置部分参数（指定名称）
property=Property.Solid()
property.createProperty('Property-1')  # 传递属性名称，创建一个Solid属性对象
property.setIsolid(0)
property.setIsmstr(0)
property.setIcpre(0)
property.setItetra10(0)
property.setItetra4(0)
property.setIFrame(0)
property.setDn(0)
property.setMuV(0)
property.setMinTimeStep(0)
property.setNdir(1)

## 根据属性名称获取类型
AI.AI_Properties().getPropertyType('Property-1')

## 根据属性名称获取Solid属性操作对象
property=Property.Solid.GetSolid('Property-1')

## 修改属性名称
property.setName('Property-2')

## 设置稳定化数值阻尼
property.setDn(0)

## 设置二次体积粘性
property.setQa(0)

## 设置线性体积粘性
property.setQb(0)

## 设置沙漏粘性系数
property.setH(0)

## 设置数值粘性 LambdaV
property.setLambdaV(0)

## 设置数值粘性 MuV
property.setMuV(0)

## 设置最小时间步长
property.setMinTimeStep(0)

## 设置每个实体单元的方向数
property.setNdir(1)

## 设置实体单元公式标志
property.setIsolid(0)

## 设置小应变公式标志
property.setIsmstr(0)

## 设置接触压力公式标志
property.setIcpre(0)

## 设置四节点四面体公式标志
property.setItetra4(0)

## 设置十节点四面体公式标志
property.setItetra10(0)

## 设置节点质量分配标志
property.setImas(0)

## 设置单元坐标系公式标志
property.setIFrame(0)
