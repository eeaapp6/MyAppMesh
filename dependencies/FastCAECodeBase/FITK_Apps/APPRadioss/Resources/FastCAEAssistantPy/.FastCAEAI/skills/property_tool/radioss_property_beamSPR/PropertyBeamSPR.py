# BeamSPR属性示例代码

## 示例1：创建BeamSPR属性并设置部分参数（指定名称）
property=Property.BeamSPR()
property.createProperty('Property-1')  # 传递属性名称，创建一个BeamSPR属性对象
property.setMass(0.0)
property.setInertia(0.0)
property.setIsflag(0)
property.setIfail(0)
property.setIleng(0)
property.setIfail2(0)
property.setV0(0.0)
property.setW0(0.0)
property.setFcut(0.0)
property.setFsmooth(0)
property.setK(1, 0.0)
property.setC(1, 0.0)
property.setA(1, 0.0)
property.setB(1, 0.0)
property.setD(1, 0.0)
property.setH(1, 0)
property.setMin(1, 0.0)
property.setMax(1, 0.0)
property.setF(1, 0.0)
property.setE(1, 0.0)
property.setAscale(1, 0.0)
property.setHscale(1, 0.0)
property.setVc(1, 0.0)
property.setN(1, 0.0)
property.setAlpha(1, 0.0)
property.setExponent(1, 0.0)

## 根据属性名称获取类型
AI.AI_Properties().getPropertyType('Property-1')

## 根据属性名称获取BeamSPR属性操作对象
property=Property.BeamSPR.GetBeamSPR('Property-1')

## 修改属性名称
property.setName('Property-2')

## 设置质量
property.setMass(0.0)

## 设置弹簧惯量
property.setInertia(0.0)

## 设置传感器标志
property.setIsflag(0)

## 设置失效标准
property.setIfail(0)

## 设置每单位长度输入标志
property.setIleng(0)

## 设置故障模型标志
property.setIfail2(0)

## 设置参考速度
property.setV0(0.0)

## 设置参考角速度
property.setW0(0.0)

## 设置应变率切割频率
property.setFcut(0.0)

## 设置平滑应变率标志
property.setFsmooth(0)

## 设置第1个自由度刚度
property.setK(1, 0.0)

## 设置第1个自由度阻尼
property.setC(1, 0.0)

## 设置第1个自由度非线性刚度函数比例系数
property.setA(1, 0.0)

## 设置第1个自由度对数速率效应比例系数 B
property.setB(1, 0.0)

## 设置第1个自由度对数速率效应比例系数 D
property.setD(1, 0.0)

## 设置第1个自由度非线性刚度函数 ID1
property.setFunction1(1, 'Curve-1')

## 设置第1个自由度弹簧硬化标志 H
property.setH(1, 0)

## 设置第1个自由度函数 ID2
property.setFunction2(1, 'Curve-2')

## 设置第1个自由度函数 ID3
property.setFunction3(1, 'Curve-3')

## 设置第1个自由度函数 ID4
property.setFunction4(1, 'Curve-4')

## 设置第1个自由度最小失效阈值
property.setMin(1, 0.0)

## 设置第1个自由度最大失效阈值
property.setMax(1, 0.0)

## 设置第1个自由度阻尼函数横坐标比例系数 F
property.setF(1, 0.0)

## 设置第1个自由度阻尼函数纵坐标比例系数 E
property.setE(1, 0.0)

## 设置第1个自由度刚度函数横坐标比例系数 Ascale
property.setAscale(1, 0.0)

## 设置第1个自由度阻尼函数 h() 纵坐标比例系数 Hscale
property.setHscale(1, 0.0)

## 设置第1个自由度相对速度系数 vc
property.setVc(1, 0.0)

## 设置第1个自由度相对速度指数 n
property.setN(1, 0.0)

## 设置第1个自由度故障比例系数 alpha
property.setAlpha(1, 0.0)

## 设置第1个自由度指数 beta
property.setExponent(1, 0.0)
