# Bool 几何示例代码

## 示例1：创建布尔求和
geo=Geometry.BoolOper()
geo.createGBTAdd('Box-1', 'Box-2')

## 示例2：创建布尔求差
geo=Geometry.BoolOper()
geo.createGBTCut('Box-1', 'Cylinder-1')

## 示例3：创建布尔求交
geo=Geometry.BoolOper()
geo.createGBTCommon('Box-1', 'Sphere-1')

## 示例4：根据几何名称获取BoolOper对象
geo=Geometry.BoolOper.GetGeometry('GBTAdd-1')

## 示例5：修改几何名称
geo.setName('Bool-Geometry-1')