# Box 几何示例代码

## 示例1：创建Box几何
geo=Geometry.Box()
geo.createGeometry(0, 0, 0, 1, 2, 3)

## 示例2：根据几何名称获取类型
AI.AI_tool().getGeometryType('Box-1')

## 示例3：根据几何名称获取Box对象
geo=Geometry.Box.GetGeometry('Box-1')

## 示例4：修改几何名称
geo.setName('Box-Geometry-1')