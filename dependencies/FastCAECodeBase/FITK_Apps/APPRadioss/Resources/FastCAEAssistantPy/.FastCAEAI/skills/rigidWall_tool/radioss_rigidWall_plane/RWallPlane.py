# 平面刚性墙示例代码

## 示例1：创建平面刚性墙并设置部分参数（指定名称）
wall=RWall.RWallPlane()
wall.createRWall('RigidWall-1')  # 传递刚性墙名称，创建一个平面刚性墙对象
wall.setSlidingType(2)
wall.setDSearch(200)
wall.setFrictionCoef(0.2)
wall.setFilteringFactor(0)
wall.setFilteringFlag(0)
wall.setNodeGroup1('None')
wall.setNodeGroup2('None')
wall.setBasicPoint(0, 0, 0)
wall.setNormalVector(0, 0, 1)

## 根据刚性墙名称获取类型
AI.AI_Solution().getRWallType('RigidWall-1')

## 根据名称获取平面刚性墙操作对象
wall=RWall.RWallPlane.GetRWallPlane('RigidWall-1')

## 修改刚性墙名称
wall.setName('RigidWall-2')

## 设置滑移类型
wall.setSlidingType(2)

## 设置搜索距离
wall.setDSearch(200)

## 设置摩擦系数
wall.setFrictionCoef(0.2)

## 设置滤波因子
wall.setFilteringFactor(0)

## 设置滤波标志
wall.setFilteringFlag(0)

## 设置添加到刚性墙的节点组
wall.setNodeGroup1('None')  # None表示不添加节点组

## 设置从刚性墙移除的节点组
wall.setNodeGroup2('None')  # None表示不移除节点组

## 设置平面基点坐标
wall.setBasicPoint(0, 0, 0)

## 设置平面法向量
wall.setNormalVector(0, 0, 1)
