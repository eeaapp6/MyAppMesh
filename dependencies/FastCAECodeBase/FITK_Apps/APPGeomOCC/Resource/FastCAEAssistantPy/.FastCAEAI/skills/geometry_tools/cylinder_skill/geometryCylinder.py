# Cylinder 几何示例代码

## 示例1：创建Cylinder几何
geo=Geometry.Cylinder()
geo.createGeometry(0, 0, 0, 0, 0, 1, 0.5, 5)

## 示例2：根据几何名称获取类型
AI.AI_tool().getGeometryType('Cylinder-1')

## 示例3：根据几何名称获取Cylinder对象
geo=Geometry.Cylinder.GetGeometry('Cylinder-1')

## 示例4：修改几何名称
geo.setName('Cylinder-Geometry-1')