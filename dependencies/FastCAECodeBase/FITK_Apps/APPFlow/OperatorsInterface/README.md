# OperatorsInterface - 操作器接口模块

## 简介

OperatorsInterface 是 FastCAE 集成工具包（FITK）的操作器接口模块，定义操作器的抽象接口和基础类，为上层应用提供统一的操作器调用方式。

## 模块定位

OperatorsInterface 位于项目根目录，是操作器模块的接口层，主要承担以下职责：

- 定义操作器的抽象接口
- 提供操作器的基础类和模板
- 规范操作器的行为和调用方式
- 作为 GUI 和 Model 之间的桥梁

## 架构与目录

```text
OperatorsInterface/
└── CMakeLists.txt              # 构建配置
```

## 依赖关系

### 编译依赖

- Qt5::Core
- FITKCore（核心对象）

## 注意事项

- OperatorsInterface 是一个接口层模块
- 具体接口定义在 OperatorsModel 和 OperatorsGUI 中
- 该模块主要提供编译和依赖管理
- 接口定义通常通过头文件实现

最后更新时间：2026-04-30