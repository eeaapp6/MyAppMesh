# pyright: reportUndefinedVariable=false

# JOHNSON失效模型示例代码

## 示例1：创建JOHNSON失效模型并设置部分参数（指定名称）
failure=Failure.JOHNSON()
failure.createFailureModel('Failure-1')  # 传递失效模型名称，创建一个JOHNSON失效模型对象
failure.addMaterial('Material-1')
failure.setD1(0.1)
failure.setD2(0.2)
failure.setD3(0.3)
failure.setD4(0.4)
failure.setD5(0.5)
failure.setEps0(0.01)
failure.setIfailsh(1)
failure.setIfailso(1)
failure.setDadv(1.0)
failure.setIxfem(0)

## 根据失效模型名称获取类型
AI.AI_Properties().getFailureModelType('Failure-1')

## 根据名称获取JOHNSON失效模型操作对象
failure=Failure.JOHNSON.GetJOHNSON('Failure-1')

## 修改失效模型名称
failure.setName('Failure-2')

## 添加材料
failure.addMaterial('Material-1')

## 移除材料
failure.removeMaterial('Material-1')

## 设置参数 D1
failure.setD1(0.1)

## 设置参数 D2
failure.setD2(0.2)

## 设置参数 D3
failure.setD3(0.3)

## 设置参数 D4
failure.setD4(0.4)

## 设置参数 D5
failure.setD5(0.5)

## 设置参考应变率 Eps0
failure.setEps0(0.01)

## 设置壳失效标志 Ifailsh
failure.setIfailsh(1)

## 设置实体失效标志 Ifailso
failure.setIfailso(1)

## 设置裂纹扩展判据 Dadv
failure.setDadv(1.0)

## 设置 XFEM 标志 Ixfem
failure.setIxfem(0)