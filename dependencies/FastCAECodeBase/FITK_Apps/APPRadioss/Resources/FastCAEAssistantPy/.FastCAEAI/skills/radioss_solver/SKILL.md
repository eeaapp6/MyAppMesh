# Radioss 求解器

## 功能描述

本技能用于在Radioss中执行仿真求解，支持多种求解方式和参数配置。

## 使用方法

通过Python语言接口调用，使用submitPythonCommand执行求解脚本。

**执行规则：submitPythonCommand接口可以执行多条命令，各命令之间使用;分割开。**

## 求解流程

### 1. 创建求解器

```python
solver=Global.Solver()
```

创建Solver对象，用于管理整个求解过程。

### 2. 配置求解器（可选）

#### 设置线程数

```python
solver.setThreadNum(1)
```

设置求解计算使用的线程数：
- 参数：线程数量（整数）

#### 设置工作路径

```python
solver.setSolverWorkDir('E:/FastCAE_IntTK/APPRadioss/output/workDir')
```

设置求解器的工作目录：
- 参数：工作路径（字符串）

### 3. 执行求解

#### 直接求解（当前Solution）

```python
solver.startRun()
```

求解当前激活的Solution。

#### 指定Solution求解

```python
solver.startRun('solution-1')
```

求解指定的Solution：
- 参数：Solution名称（字符串）

#### 使用配置文件求解

```python
solver.startRunConfigFile('E:/FastCAE_IntTK/APPRadioss/output/workDir/Solution-2/Solution-2_0000.rad')
```

使用已存在的.rad配置文件进行求解：
- 参数：.rad文件完整路径（字符串）

## 参数说明

| 方法 | 参数 | 说明 |
|------|------|------|
| Solver | 无 | 创建求解器对象 |
| setThreadNum | int | 设置求解线程数 |
| setSolverWorkDir | string | 设置求解器工作目录路径 |
| startRun | string（可选） | 执行求解当前Solution或指定Solution |
| startRunConfigFile | string | 使用已存在的.rad配置文件求解 |

## 逐条执行示例

| 序号 | 命令 | 说明 |
|------|------|------|
| 1 | solver=Global.Solver() | 创建求解器 |
| 2 | solver.setThreadNum(1) | 设置线程数（可选） |
| 3 | solver.setSolverWorkDir('E:/FastCAE_IntTK/APPRadioss/output/workDir') | 设置工作路径（可选） |
| 4 | solver.startRun() | 执行求解 |

## 注意事项

1. startRun()可指定Solution名称，不指定时求解当前Solution
2. startRunConfigFile()用于使用已生成的.rad配置文件求解
3. 工作路径建议使用绝对路径
4. 线程数根据计算机配置合理设置