# Shell属性示例代码

## 示例1：创建Shell属性并设置部分参数（指定名称）
property=Property.Shell()
property.createProperty('Property-1')  # 传递属性名称，创建一个Shell属性对象
property.setThick(1.8)
property.setIshell(24)
property.setIsmstr(0)
property.setIsh3n(0)
property.setIdril(0)
property.setIpinch(0)
property.setPthickFail(0)
property.setHm(0)
property.setHf(0)
property.setHr(0)
property.setDm(0)
property.setDn(0)
property.setN(0)
property.setAshear(0)
property.setIthick(0)
property.setIplas(1)

## 根据属性名称获取类型
AI.AI_Properties().getPropertyType('Property-1')

## 根据属性名称获取Shell属性操作对象
property=Property.Shell.GetShell('Property-1')

## 修改属性名称
property.setName('Property-2')

## 设置壳厚度
property.setThick(1.8)

## 设置壳单元公式标志
property.setIshell(24)

## 设置壳小应变公式标志
property.setIsmstr(0)

## 设置三节点壳单元公式标志
property.setIsh3n(0)

## 设置钻转自由度刚度标志
property.setIdril(0)

## 设置夹挤自由度标志
property.setIpinch(0)

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

## 设置厚度积分点数
property.setN(0)

## 设置剪切因子
property.setAshear(0)

## 设置厚度应力计算标志
property.setIthick(0)

## 设置平面应力塑性标志
property.setIplas(1)