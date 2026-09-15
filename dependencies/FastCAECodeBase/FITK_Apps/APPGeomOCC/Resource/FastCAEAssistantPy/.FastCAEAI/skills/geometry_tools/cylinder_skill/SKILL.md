# Cylinder 几何

## 功能描述

本技能用于在几何环境中创建 Cylinder 几何，并支持获取几何类型、根据名称获取 Cylinder 操作对象以及修改几何名称。

当前仅说明已具备的接口：

- 创建 Cylinder
- 根据名称判断几何类型
- 根据名称获取 Cylinder 对象
- 修改 Cylinder 名称

基准点、方向、半径和长度的单独编辑接口后续补充，本技能暂不包含这部分内容。

## 使用方法

通过 Python 语言接口调用，使用 submitPythonCommand 执行 Cylinder 几何配置脚本。

执行规则：submitPythonCommand 接口可以执行多条命令，各命令之间使用 ; 分割开。

## 创建 Cylinder 几何

创建 Cylinder 操作对象后，可直接通过 createGeometry 创建一个 Cylinder 几何。

```python
geo=Geometry.Cylinder()
geo.createGeometry(0, 0, 0, 0, 0, 1, 0.5, 5)
```

- 参数依次为：基准点 X、Y、Z 坐标，圆柱轴向方向 X、Y、Z 分量，圆柱体半径，圆柱体长度
- 返回：执行结果信息（字符串）
- 几何会以系统默认名称创建，如 Cylinder-1、Cylinder-2

完整示例：创建 Cylinder

```python
geo=Geometry.Cylinder()
geo.createGeometry(0, 0, 0, 0, 0, 1, 0.5, 5)
```

## 查询几何类型

根据几何名称获取类型，便于在编辑前确认对象是否为 Cylinder：

```python
AI.AI_tool().getGeometryType('Cylinder-Geometry-1')
```

- 参数：几何名称（字符串）
- 返回：几何类型描述，Cylinder 几何返回 `The geometry type is Cylinder`

## 修改Cylinder名称、基准点、方向、半径、长度

### 1. 获取已有 Cylinder 对象

根据几何名称获取已存在的 Cylinder 对象：

```python
geo=Geometry.Cylinder.GetGeometry('Cylinder-Geometry-1')
```

- 参数：几何名称（字符串）
- 当名称存在且对象类型正确时，可继续执行后续操作

### 2. 修改 Cylinder 名称

获取对象后，可以修改几何名称：

```python
geo.setName('Cylinder-Geometry-2')
```

- 参数：新的几何名称（字符串）
- 返回：执行结果信息（字符串）

### 3. 修改 Cylinder 基准点、方向、半径和长度

获取对象后，可以修改 Cylinder 的基准点、方向、半径和长度，目前暂不支持单独修改这些属性的接口，后续版本将补充相关功能。

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| Cylinder | - | 创建 Cylinder 操作对象 |
| createGeometry | double, double, double, double, double, double, double, double | 创建 Cylinder，参数依次为基准点、方向、半径和长度 |
| Cylinder.GetGeometry | string | 根据名称获取已存在的 Cylinder 对象 |
| setName | string | 修改几何名称 |
| AI.AI_tool().getGeometryType | string | 根据名称获取几何类型 |

## 注意事项

1. createGeometry 当前支持创建 Cylinder，但不包含基准点、方向、半径和长度的单独编辑接口。
2. 方向分量应能表达有效的圆柱轴向。
3. 编辑前建议先通过 AI.AI_tool().getGeometryType('几何名') 确认对象类型是否为 Cylinder。
4. 若根据名称获取对象失败，说明目标几何不存在或名称不正确。