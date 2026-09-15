# Bool 几何

## 功能描述

本技能用于在几何环境中执行布尔操作，并支持创建布尔求和、求差、求交对象，以及根据名称获取 BoolOper 操作对象并修改名称。

当前仅说明已具备的接口：

- 创建布尔求和（GBTAdd）
- 创建布尔求差（GBTCut）
- 创建布尔求交（GBTCommon）
- 根据名称获取 BoolOper 对象
- 修改 BoolOper 名称

更多布尔结果编辑接口后续补充，本技能暂不包含这部分内容。

## 使用方法

通过 Python 语言接口调用，使用 submitPythonCommand 执行 Bool 几何配置脚本。

执行规则：submitPythonCommand 接口可以执行多条命令，各命令之间使用 ; 分割开。

## 创建 Bool 几何

创建 BoolOper 操作对象后，可调用不同接口执行布尔操作。

### 1. 创建布尔求和

```python
geo=Geometry.BoolOper()
geo.createGBTAdd('Box-1', 'Box-2')
```

- 参数依次为：目标几何名称、工具几何名称
- 返回：执行结果信息（字符串）
- 几何会以系统默认名称创建，如 GBTAdd-1

### 2. 创建布尔求差

```python
geo=Geometry.BoolOper()
geo.createGBTCut('Box-1', 'Cylinder-1')
```

- 参数依次为：目标几何名称、工具几何名称
- 返回：执行结果信息（字符串）

### 3. 创建布尔求交

```python
geo=Geometry.BoolOper()
geo.createGBTCommon('Box-1', 'Sphere-1')
```

- 参数依次为：目标几何名称、工具几何名称
- 返回：执行结果信息（字符串）

完整示例：创建布尔求和后重命名

```python
geo=Geometry.BoolOper()
geo.createGBTAdd('Box-1', 'Box-2')
geo.setName('Bool-Geometry-1')
```

## 获取已有 BoolOper 对象

根据几何名称获取已存在的 BoolOper 对象：

```python
geo=Geometry.BoolOper.GetGeometry('GBTAdd-1')
```

- 参数：几何名称（字符串）
- 当名称存在时，可继续执行后续操作

## 修改 BoolOper 名称

获取对象后，可以修改几何名称：

```python
geo.setName('Bool-Geometry-2')
```

- 参数：新的几何名称（字符串）
- 返回：执行结果信息（字符串）

## 获取 BoolOper 名称

```python
geo.getName()
```

- 返回：当前几何名称

## 参数说明

| 方法 | 参数类型 | 说明 |
|------|----------|------|
| BoolOper | - | 创建 BoolOper 操作对象 |
| createGBTAdd | string, string | 创建布尔求和，参数依次为目标几何名称和工具几何名称 |
| createGBTCut | string, string | 创建布尔求差，参数依次为目标几何名称和工具几何名称 |
| createGBTCommon | string, string | 创建布尔求交，参数依次为目标几何名称和工具几何名称 |
| BoolOper.GetGeometry | string | 根据名称获取已存在的 BoolOper 对象 |
| setName | string | 修改几何名称 |
| getName | - | 获取当前几何名称 |

## 注意事项

1. 布尔操作前需保证目标几何和工具几何都已存在。
2. 目标几何和工具几何不能是同一个对象，否则布尔操作会失败。
3. 当前 skill 只覆盖布尔创建和名称修改，不包含更细粒度的结果编辑接口。
4. 若根据名称获取对象失败，说明目标几何不存在或名称不正确。