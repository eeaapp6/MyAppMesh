# 重力载荷示例代码

## 示例1：创建重力载荷并设置部分参数（指定名称）
p=Gravity.Gravity()
p.createGravity('Gravity-1')
p.setGravityDirection('Z')
p.setFScaleY(9.8)
p.setNodeGroup('Global')

## 根据名称获取重力载荷操作对象
p=Gravity.Gravity.GetGravity('Gravity-1')

## 修改重力载荷名称
p.setName('Gravity-2')

## 设置重力方向
p.setGravityDirection('Y')

## 设置重力强度
p.setFScaleY(-9.8)

## 设置作用节点组
p.setNodeGroup('NodeGroup-1')

## 设置时间函数（可选）
p.setTimeFunction('Curve-1')

## 设置 X 方向缩放因子（可选）
p.setAScaleX(1.0)