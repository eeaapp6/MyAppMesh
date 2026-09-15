# BIQUAD失效模型示例代码

## 示例1：创建BIQUAD失效模型并设置部分参数（指定名称）
failure=Failure.BIQUAD()
failure.createFailureModel('Failure-1')  # 传递失效模型名称，创建一个BIQUAD失效模型对象
failure.addMaterial('Material-1')
failure.setC1(0.1)
failure.setC2(0.2)
failure.setC3(0.3)
failure.setC4(0.4)
failure.setC5(0.5)
failure.setPthickfail(0.0)
failure.setMFlag(0)
failure.setSFlag(1)
failure.setInststart(0.0)
failure.setFctIDel('Curve-1')
failure.setElref(1.0)

## 根据失效模型名称获取类型
AI.AI_Properties().getFailureModelType('Failure-1')

## 根据名称获取BIQUAD失效模型操作对象
failure=Failure.BIQUAD.GetBIQUAD('Failure-1')

## 修改失效模型名称
failure.setName('Failure-2')

## 添加材料
failure.addMaterial('Material-1')

## 移除材料
failure.removeMaterial('Material-1')

## 设置单轴压缩失效塑性应变
failure.setC1(0.1)

## 设置剪切失效塑性应变
failure.setC2(0.2)

## 设置单轴拉伸失效塑性应变
failure.setC3(0.3)

## 设置平面应变拉伸失效塑性应变
failure.setC4(0.4)

## 设置双轴拉伸失效应变
failure.setC5(0.5)

## 设置壳单元厚度方向失效积分点比例
failure.setPthickfail(0.0)

## 设置材料选择标志
failure.setMFlag(0)

## 设置特定行为标志
failure.setSFlag(1)

## 设置局部缩颈失稳起始值
failure.setInststart(0.0)

## 设置单元尺寸因子函数曲线
failure.setFctIDel('Curve-1')

## 设置参考单元尺寸
failure.setElref(1.0)

## 当 MFlag=99 时设置 R1、R2、R4、R5
failure.setMFlag(99)
failure.setR1(1.0)
failure.setR2(1.0)
failure.setR4(1.0)
failure.setR5(1.0)
