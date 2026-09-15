# Polynomial状态方程示例代码

## 示例1：创建Polynomial状态方程并设置部分参数（指定名称）
eos=EOS.EOSPolynomial()
eos.createEquationOfState('EOS-1')  # 传递状态方程名称，创建一个Polynomial状态方程对象
eos.addMaterial('Material-1')
eos.setC0(0.0)
eos.setC1(0.0)
eos.setC2(0.0)
eos.setC3(0.0)
eos.setC4(0.0)
eos.setC5(0.0)
eos.setE0(0.0)
eos.setPsh(0.0)
eos.setP0(1.0)

## 根据状态方程名称获取类型
AI.AI_Properties().getEquationOfStateType('EOS-1')

## 根据名称获取Polynomial状态方程操作对象
eos=EOS.EOSPolynomial.GetEOSPolynomial('EOS-1')

## 修改状态方程名称
eos.setName('EOS-2')

## 添加材料
eos.addMaterial('Material-1')

## 移除材料
eos.removeMaterial('Material-1')

## 设置系数 C0
eos.setC0(0.0)

## 设置系数 C1
eos.setC1(0.0)

## 设置系数 C2
eos.setC2(0.0)

## 设置系数 C3
eos.setC3(0.0)

## 设置系数 C4
eos.setC4(0.0)

## 设置系数 C5
eos.setC5(0.0)

## 设置初始单位参考体积内能 E0
eos.setE0(0.0)

## 设置压力偏移 Psh
eos.setPsh(0.0)

## 设置参考密度 P0
eos.setP0(1.0)
