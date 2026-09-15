# 边界条件示例代码

## 示例1：创建边界条件并设置部分参数（指定名称）
p=BCS.BCS()
p.createBCS('BCS-1')
p.setNodeGroup('NodeGroup-1')
p.setFreedomFixed(1, True)
p.setFreedomFixed(2, True)
p.setFreedomFixed(3, True)
p.setFreedomFixed(4, False)
p.setFreedomFixed(5, False)
p.setFreedomFixed(6, False)

## 根据边界条件名称获取类型
AI.AI_Solution().getBCSType('BCS-1')

## 根据名称获取边界条件操作对象
p=BCS.BCS.GetBCS('BCS-1')

## 修改边界条件名称
p.setName('BCS-2')

## 设置节点组
p.setNodeGroup('NodeGroup-2')

## 设置自由度约束状态
p.setFreedomFixed(1, False)
p.setFreedomFixed(2, True)
p.setFreedomFixed(3, False)
p.setFreedomFixed(4, True)
p.setFreedomFixed(5, False)
p.setFreedomFixed(6, True)