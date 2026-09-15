# 角速度初速度场示例代码

## 示例1：创建角速度初速度场并设置部分参数（指定名称）
p=InitialValue.InitialValueAxis()
p.createInitialValue('InitialField-1')
p.setValue(0.0, 0.0, 0.0)
p.setNodeGroup('NodeGroup-1')
p.setDir('X')
p.setVr(10.0)

## 根据初速度场名称获取类型
AI.AI_Solution().getInitialValueType('InitialField-1')

## 根据名称获取角速度初速度场操作对象
p=InitialValue.InitialValueAxis.GetInitialValueAxis('InitialField-1')

## 修改初速度场名称
p.setName('InitialField-2')

## 设置初始速度
p.setValue(0.0, 0.0, 0.0)

## 设置节点组
p.setNodeGroup('NodeGroup-2')

## 设置旋转轴方向
p.setDir('Y')

## 设置绕轴角速度
p.setVr(20.0)
