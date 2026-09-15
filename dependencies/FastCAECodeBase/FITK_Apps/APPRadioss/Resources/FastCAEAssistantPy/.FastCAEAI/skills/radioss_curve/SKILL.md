# Radioss 曲线

## 功能描述

本技能用于在Radioss中创建和修改曲线，支持设置曲线名称、添加曲线点、修改曲线点、删除曲线点、清空全部曲线点以及获取曲线点数量。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行曲线配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建曲线

### 1. 创建曲线

创建曲线操作对象后，可以创建曲线。

```python
curve=Curve.Curve()
curve.createCurve('Curve-1')
```

- 参数：曲线名称（字符串）
- 使用指定名称创建曲线
- 注意：如果已存在同名曲线会创建失败

**完整示例：创建曲线并设置部分参数**

```python
curve=Curve.Curve()
curve.createCurve('Curve-1')
curve.addPoint(0.0, 0.0)
curve.addPoint(1.0, 2.0)
curve.addPoint(2.0, 3.5)
```

## 修改曲线

### 1. 获取曲线对象

根据曲线名称获取已存在的曲线对象：
- 参数：曲线名称（字符串）

```python
curve=Curve.Curve.GetCurve('Curve-1')
```

## 修改曲线参数

获取对象后，可修改以下属性：

### 1. 修改曲线名称

```python
curve.setName('Curve-2')
```

设置曲线的新名称：
- 参数：曲线名称（字符串）
- 返回：执行结果信息（字符串）

### 2. 添加曲线点

```python
curve.addPoint(0.0, 0.0)
```

添加曲线点：
- 参数1：点横坐标（浮点数）
- 参数2：点纵坐标（浮点数）
- 返回：执行结果信息（字符串）

### 3. 删除指定索引的曲线点

```python
curve.removePoint(0)
```

删除指定索引的曲线点：
- 参数：点索引（整数，从 0 开始）
- 返回：执行结果信息（字符串）

### 4. 修改指定索引的曲线点

```python
curve.updatePoint(1, 1.5, 2.5)
```

修改指定索引的曲线点：
- 参数1：点索引（整数，从 0 开始）
- 参数2：点横坐标（浮点数）
- 参数3：点纵坐标（浮点数）
- 返回：执行结果信息（字符串）

### 5. 删除全部曲线点

```python
curve.removeAllPoints()
```

删除全部曲线点：
- 返回：执行结果信息（字符串）

### 6. 获取曲线点数量

```python
curve.getPointCount()
```

获取曲线点数量：
- 返回：点数量（整数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| Curve | - | 创建Curve对象 |
| Curve.GetCurve | string | 根据名称获取已存在的曲线对象 |
| createCurve | string | 创建指定名称的曲线 |
| setName | string | 修改曲线名称 |
| addPoint | double, double | 添加曲线点 |
| removePoint | int | 删除指定索引的曲线点 |
| updatePoint | int, double, double | 修改指定索引的曲线点 |
| removeAllPoints | - | 删除全部曲线点 |
| getPointCount | - | 获取曲线点数量 |

## 注意事项

1. 创建前需保证当前存在有效的曲线管理环境。
2. 同名曲线已存在时，`createCurve` 会失败。
3. 编辑前建议先通过 `Curve.Curve.GetCurve` 获取对象。
4. `removePoint` 和 `updatePoint` 的点索引从 0 开始，不是从 1 开始。
5. 当前界面 `writePythonScript` 在编辑曲线时采用 `removeAllPoints()` 后重新 `addPoint(...)` 的方式重建点集。
6. `createCurve`、`setName`、`addPoint`、`removePoint`、`updatePoint`、`removeAllPoints` 会返回执行详细信息，便于脚本中直接判断执行结果。
