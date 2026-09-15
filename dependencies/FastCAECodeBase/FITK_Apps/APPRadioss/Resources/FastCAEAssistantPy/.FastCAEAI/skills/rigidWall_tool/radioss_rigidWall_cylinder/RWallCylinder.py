# 圆柱刚性墙示例代码

## 示例1：创建圆柱刚性墙并设置部分参数（指定名称）
wall=RWall.RWallCylinder()
wall.createRWall('RigidWall-1')  # 传递刚性墙名称，创建一个圆柱刚性墙对象
wall.setSlidingType(2)
wall.setDSearch(200)
wall.setFrictionCoef(0.2)
wall.setDiameter(254)
wall.setFilteringFactor(0)
wall.setFilteringFlag(0)
wall.setNodeGroup1('None')
wall.setNodeGroup2('None')
wall.setBasicPoint(-170, 0, 0)
wall.setAxisVector(0, 0, 1)

## 根据刚性墙名称获取类型
AI.AI_Solution().getRWallType('RigidWall-1')

## 根据名称获取圆柱刚性墙操作对象
wall=RWall.RWallCylinder.GetRWallCylinder('RigidWall-1')

## 修改刚性墙名称
wall.setName('RigidWall-2')

## 设置滑移类型
wall.setSlidingType(2)

## 设置搜索距离
wall.setDSearch(200)

## 设置摩擦系数
wall.setFrictionCoef(0.2)

## 设置特征直径
wall.setDiameter(254)

## 设置滤波因子
wall.setFilteringFactor(0)

## 设置滤波标志
wall.setFilteringFlag(0)

## 设置添加到刚性墙的节点组
wall.setNodeGroup1('None')  # None表示不添加节点组

## 设置从刚性墙移除的节点组
wall.setNodeGroup2('None')  # None表示不移除节点组

## 设置圆柱基点坐标
wall.setBasicPoint(-170, 0, 0)

## 设置圆柱轴向量
wall.setAxisVector(0, 0, 1)