# Radioss 编辑刚性墙

## 功能描述

本技能用于在Radioss中编辑已有刚性墙。
执行编辑前，先通过 `AI.AI_Solution().getRWallType(刚性墙名称)` 判断刚性墙是否存在，并获取刚性墙类型。
确认刚性墙类型后，再进入对应刚性墙类型的skill中执行具体编辑操作。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand先查询刚性墙类型，再根据返回类型选择对应skill执行编辑。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 编辑流程

### 1. 查询刚性墙是否存在及刚性墙类型

```python
AI.AI_Solution().getRWallType('RigidWall-1')
```

说明：
- 参数：刚性墙名称（字符串）
- 返回：刚性墙是否存在及刚性墙类型信息
- 作用：判断目标刚性墙是否存在，并识别其刚性墙类型

### 2. 根据刚性墙类型进入对应skill

当 `AI.AI_Solution().getRWallType('刚性墙名称')` 返回类型后，按下列规则选择对应skill：

- 返回 `RWallPlane`：进入 `radioss_rigidWall_plane` skill，执行平面刚性墙编辑。
- 返回 `RWallCylinder`：进入 `radioss_rigidWall_cylinder` skill，执行圆柱刚性墙编辑。
- 返回空值、失败信息或未知类型：停止编辑，并提示刚性墙不存在或当前暂不支持该刚性墙类型。

## 调用示例

### 示例1：先查询刚性墙类型

```python
AI.AI_Solution().getRWallType('RigidWall-1')
```

### 示例2：刚性墙类型为RWallPlane时

先确认类型：

```python
AI.AI_Solution().getRWallType('RigidWall-1')
```

若返回 `RWallPlane`，则在 `radioss_rigidWall_plane` skill 中执行：

```python
wall=RWall.RWallPlane.GetRWallPlane('RigidWall-1')
wall.setDSearch(200)
wall.setNormalVector(0, 0, 1)
```

### 示例3：刚性墙类型为RWallCylinder时

先确认类型：

```python
AI.AI_Solution().getRWallType('RigidWall-1')
```

若返回 `RWallCylinder`，则在 `radioss_rigidWall_cylinder` skill 中执行：

```python
wall=RWall.RWallCylinder.GetRWallCylinder('RigidWall-1')
wall.setDiameter(254)
wall.setAxisVector(0, 0, 1)
```

## 使用约束

1. 本skill只负责识别刚性墙是否存在以及刚性墙类型，不直接定义某一种刚性墙的全部编辑接口。
2. 识别出刚性墙类型后，应切换到对应刚性墙skill执行具体编辑操作。
3. 若刚性墙不存在，不应继续执行编辑命令。
4. 若刚性墙类型当前没有对应skill，不应继续编辑，应提示当前暂不支持。
5. 编辑已有刚性墙前，优先执行一次 `AI.AI_Solution().getRWallType('刚性墙名')`。

## 建议执行策略

1. 从用户输入中提取目标刚性墙名称和待修改参数。
2. 调用 `AI.AI_Solution().getRWallType('刚性墙名称')`。
3. 判断返回结果是否有效。
4. 根据刚性墙类型选择对应skill。
5. 在对应skill中生成并执行具体的刚性墙编辑脚本。
6. 将具体编辑接口的返回结果反馈给用户。