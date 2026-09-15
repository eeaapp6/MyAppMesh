# 编辑几何属性

## 功能描述

本技能用于在几何环境中编辑已有几何属性。
执行编辑前，先通过 AI.AI_tool().getGeometryType(几何名称) 判断几何是否存在，并获取几何类型。
确认几何类型后，再进入对应几何类型的 skill 中执行具体编辑操作。

当前支持识别的几何类型包括：

- Box
- Cylinder
- Sphere
- Bool

Bool类型不支持编辑，只支持修改名称。

## 使用方法

通过 Python 语言接口调用，使用 submitPythonCommand 先查询几何类型，再根据返回类型选择对应 skill 执行编辑。

执行规则：submitPythonCommand 接口可以执行多条命令，各命令之间使用 ; 分割开。

## 编辑流程

### 1. 查询几何是否存在及几何类型

```python
AI.AI_tool().getGeometryType('Geometry-1')
```

说明：

- 参数：几何名称（字符串）
- 返回：几何是否存在及几何类型信息
- 作用：判断目标几何是否存在，并识别其几何类型

如需单独判断几何是否存在，也可调用：

```python
AI.AI_tool().isGeometryExist('Geometry-1')
```

### 2. 根据几何类型进入对应 skill

当 AI.AI_tool().getGeometryType('几何名称') 返回类型后，按下列规则选择对应 skill：

- 返回 Box：进入 box_skill，执行 Box 几何属性编辑。
- 返回 Cylinder：进入 cylinder_skill，执行 Cylinder 几何属性编辑。
- 返回 Sphere：进入 sphere_skill，执行 Sphere 几何属性编辑。
- 返回失败信息、空值或 Unknown：停止编辑，并提示几何不存在或当前暂不支持该几何类型。

## 调用示例

### 示例1：先查询几何类型

```python
AI.AI_tool().getGeometryType('Geometry-1')
```

### 示例2：几何类型为 Box 时

先确认类型：

```python
AI.AI_tool().getGeometryType('Box-Geometry-1')
```

若返回 Box，则在 box_skill 中执行：

```python
geo=Geometry.Box.GetGeometry('Box-Geometry-1')
geo.setName('Box-Geometry-2')
```

### 示例3：几何类型为 Cylinder 时

先确认类型：

```python
AI.AI_tool().getGeometryType('Cylinder-Geometry-1')
```

若返回 Cylinder，则在 cylinder_skill 中执行：

```python
geo=Geometry.Cylinder.GetGeometry('Cylinder-Geometry-1')
geo.setName('Cylinder-Geometry-2')
```

### 示例4：几何类型为 Sphere 时

先确认类型：

```python
AI.AI_tool().getGeometryType('Sphere-Geometry-1')
```

若返回 Sphere，则在 sphere_skill 中执行：

```python
geo=Geometry.Sphere.GetGeometry('Sphere-Geometry-1')
geo.setName('Sphere-Geometry-2')
```

## 使用约束

1. 本 skill 只负责识别几何是否存在以及几何类型，不直接定义某一种几何的全部编辑接口。
2. 识别出几何类型后，应切换到对应几何 skill 执行具体编辑操作。
3. 若几何不存在，不应继续执行编辑命令。
4. 若几何类型当前没有对应 skill，不应继续编辑，应提示当前暂不支持。
5. 编辑已有几何前，优先执行一次 AI.AI_tool().getGeometryType('几何名')。

## 建议执行策略

1. 从用户输入中提取目标几何名称和待修改属性。
2. 调用 AI.AI_tool().getGeometryType('几何名称')。
3. 判断返回结果是否有效。
4. 根据几何类型选择对应 skill。
5. 在对应 skill 中生成并执行具体的几何编辑脚本。
6. 将具体编辑接口的返回结果反馈给用户。