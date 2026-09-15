# Radioss 圆柱刚性墙

## 功能描述

本技能用于在Radioss中创建和修改圆柱刚性墙，支持设置刚性墙名称、滑移类型、搜索距离、摩擦系数、特征直径、滤波参数、节点组以及圆柱几何参数。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行圆柱刚性墙配置脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 创建圆柱刚性墙

### 1. 创建圆柱刚性墙

创建圆柱刚性墙操作对象后，可以创建圆柱刚性墙。

```python
wall=RWall.RWallCylinder()
wall.createRWall('RigidWall-1')
```

- 参数：刚性墙名称（字符串）
- 使用指定名称创建圆柱刚性墙
- 注意：如果已存在同名刚性墙会创建失败

**完整示例：创建圆柱刚性墙并设置部分参数**

```python
wall=RWall.RWallCylinder()
wall.createRWall('RigidWall-1')
wall.setSlidingType(2)
wall.setDSearch(200)
wall.setFrictionCoef(0.2)
wall.setDiameter(254)
wall.setBasicPoint(-170, 0, 0)
wall.setAxisVector(0, 0, 1)
```

## 修改圆柱刚性墙

### 1. 获取圆柱刚性墙对象

根据刚性墙名称获取已存在的圆柱刚性墙对象：
- 参数：刚性墙名称（字符串）
- 编辑前可先通过 `AI.AI_Solution().getRWallType('RigidWall-1')` 判断对象是否为 `RWallCylinder` 类型，如果不是则不在这个skill中编辑,查找别的skill。

```python
wall=RWall.RWallCylinder.GetRWallCylinder('RigidWall-1')
```

## 修改圆柱刚性墙参数

获取对象后，可修改以下属性：

### 1. 修改刚性墙名称

```python
wall.setName('RigidWall-2')
```

设置圆柱刚性墙的新名称：
- 参数：刚性墙名称（字符串）

### 2. 设置滑移类型

```python
wall.setSlidingType(2)
```

设置刚性墙滑移类型：
- 参数：滑移类型编号（整数）
- 具体编号以系统枚举定义为准

### 3. 设置搜索距离

```python
wall.setDSearch(200)
```

设置刚性墙搜索距离：
- 参数：搜索距离（浮点数）

### 4. 设置摩擦系数

```python
wall.setFrictionCoef(0.2)
```

设置刚性墙摩擦系数：
- 参数：摩擦系数（浮点数）

### 5. 设置特征直径

```python
wall.setDiameter(254)
```

设置圆柱刚性墙特征直径：
- 参数：特征直径（浮点数）

### 6. 设置滤波因子

```python
wall.setFilteringFactor(0)
```

设置滤波因子：
- 参数：滤波因子（浮点数）

### 7. 设置滤波标志

```python
wall.setFilteringFlag(0)
```

设置滤波标志：
- 参数：滤波标志（整数）

### 8. 设置添加到刚性墙的节点组

```python
wall.setNodeGroup1('NodeGroup-1')
```

设置需要添加到刚性墙的节点组：
- 参数：节点组名称（字符串）
- 特殊值：`'None'` 表示清除该节点组设置

### 9. 设置从刚性墙移除的节点组

```python
wall.setNodeGroup2('NodeGroup-2')
```

设置需要从刚性墙移除的节点组：
- 参数：节点组名称（字符串）
- 特殊值：`'None'` 表示清除该节点组设置

### 10. 设置圆柱基点坐标

```python
wall.setBasicPoint(-170, 0, 0)
```

设置圆柱刚性墙基点坐标：
- 参数：`x, y, z` 三个坐标分量（浮点数）

### 11. 设置圆柱轴向量

```python
wall.setAxisVector(0, 0, 1)
```

设置圆柱刚性墙轴向量：
- 参数：`nx, ny, nz` 三个方向分量（浮点数）

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| RWallCylinder | - | 创建RWallCylinder对象 |
| RWallCylinder.GetRWallCylinder | string | 根据名称获取已存在的圆柱刚性墙对象 |
| createRWall | string | 创建指定名称的圆柱刚性墙 |
| setName | string | 修改刚性墙名称 |
| setSlidingType | int | 设置滑移类型 |
| setDSearch | double | 设置搜索距离 |
| setFrictionCoef | double | 设置摩擦系数 |
| setDiameter | double | 设置圆柱特征直径 |
| setFilteringFactor | double | 设置滤波因子 |
| setFilteringFlag | int | 设置滤波标志 |
| setNodeGroup1 | string | 设置添加到刚性墙的节点组 |
| setNodeGroup2 | string | 设置从刚性墙移除的节点组 |
| setBasicPoint | double, double, double | 设置圆柱基点坐标 |
| setAxisVector | double, double, double | 设置圆柱轴向量 |

## 注意事项

1. 默认使用当前有效的Solution，无需额外判断或设置。
2. 同名圆柱刚性墙已存在时，`createRWall` 会失败。
3. 编辑前建议先通过 `RWall.RWallCylinder.GetRWallCylinder` 获取对象。
4. 编辑已有刚性墙时，建议先通过 `AI.AI_Solution().getRWallType('刚性墙名')` 确认对象类型。
5. `setNodeGroup1('None')` 和 `setNodeGroup2('None')` 表示清除对应节点组设置。
6. 仅在使用摩擦相关滑移类型时设置摩擦系数更合理。
7. 圆柱基点和轴向量共同决定圆柱刚性墙的空间位置与方向。