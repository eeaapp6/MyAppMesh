# 曲线示例代码

## 示例1：创建曲线并设置部分参数（指定名称）
curve=Curve.Curve()
curve.createCurve('Curve-1')  # 传递曲线名称，创建一个曲线对象
curve.addPoint(0.0, 0.0)
curve.addPoint(1.0, 2.0)
curve.addPoint(2.0, 3.5)

## 根据曲线名称获取曲线操作对象
curve=Curve.Curve.GetCurve('Curve-1')

## 修改曲线名称
curve.setName('Curve-2')

## 添加曲线点
curve.addPoint(3.0, 4.5)

## 修改索引为1的曲线点
curve.updatePoint(1, 1.5, 2.5)

## 删除索引为0的曲线点
curve.removePoint(0)

## 获取曲线点数量
curve.getPointCount()

## 删除全部曲线点
curve.removeAllPoints()

## 重新添加曲线点
curve.addPoint(0.0, 0.0)
curve.addPoint(2.0, 5.0)
