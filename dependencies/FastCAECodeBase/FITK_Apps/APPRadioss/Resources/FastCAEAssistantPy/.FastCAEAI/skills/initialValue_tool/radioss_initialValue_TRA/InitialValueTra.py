# 初始平移速度场示例代码

## 示例1：创建初始平移速度场并设置部分参数（指定名称）
p=InitialValue.InitialValueTra()
p.createInitialValue('InitialField-1')
p.setValue(1.0, 0.0, 0.0)
p.setNodeGroup('NodeGroup-1')

## 根据初速度场名称获取类型
AI.AI_Solution().getInitialValueType('InitialField-1')

## 根据名称获取初始平移速度场操作对象
p=InitialValue.InitialValueTra.GetInitialValueTra('InitialField-1')

## 修改初速度场名称
p.setName('InitialField-2')

## 设置初始速度
p.setValue(2.0, 0.0, 0.0)

## 设置节点组
p.setNodeGroup('NodeGroup-2')
