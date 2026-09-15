# Sphere 几何示例代码

## 示例1：创建Sphere几何
geo=Geometry.Sphere()
geo.createGeometry(0, 0, 0, 1)

## 示例2：根据几何名称获取类型
AI.AI_tool().getGeometryType('Sphere-1')

## 示例3：根据几何名称获取Sphere对象
geo=Geometry.Sphere.GetGeometry('Sphere-1')

## 示例4：修改几何名称
geo.setName('Sphere-Geometry-1')