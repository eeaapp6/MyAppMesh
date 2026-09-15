# OperatorsGUI - 图形界面交互操作器模块

## 简介

OperatorsGUI 处理用户在界面上的各类交互操作，包括树形菜单动作、图形窗口交互、拾取操作和后处理场输出控制。

## 模块定位

- 实现树形菜单的各类动作操作（创建、删除、重命名、编辑）
- 实现图形窗口的拾取、框选和预览交互
- 实现后处理场输出的变量控制
- 处理树形菜单和图形窗口的事件响应

## 架构与目录

```text
OperatorsGUI
├── OperTreeAction.h / .cpp            树形菜单动作基类
├── OperTreeAssemblyAction.h / .cpp    装配树操作器
├── OperTreePropertyAction.h / .cpp    属性树操作器
├── OperTreeSolutionAction.h / .cpp    求解器树操作器
├── OperGraphPick.h / .cpp             图形拾取操作器
├── OperGraphPickPreview.h / .cpp      拾取预览操作器
├── OperGraphEvent3D.h / .cpp          三维可视化事件基类
└── OperFieldOutput.h / cpp             后处理场输出控制
```

## 依赖关系

- Qt5::Core、FITKCore
- GraphDataProvider、PickDataProvider、OperatorsInterface
- FITKRenderWindowVTK、FITKRadiossData、GUIWidget

## 主要类与功能说明

### OperTreeAction

树形菜单动作操作器基类，提供通用模板方法：

```cpp
callOperator("actionCreatePart");           // 调用操作器
changeActivationStatus(manager, id);       // 改变激活状态
renameData(oper, manager, id);             // 重命名数据
deleteData(manager, id);                   // 删除数据
```

### OperGraphPick

图形拾取操作器，继承自 GraphInteractionOperator：

```cpp
void picked(Comp::FITKGraph3DWindowVTK* gw, vtkActor* actor, int idx, double* pos) {
    auto provider = GraphData::PickedDataProvider::getInstance();
    provider->generatePickedData(actor, idx, pos, false);
}
```

### OperGraphEvent3D

三维可视化事件操作器基类：

```cpp
void render() override;     // 刷新渲染
void fitView() override;     // 自适应视图
GUI::GraphArea* getGraphWidget(GUI::GraphAreaType type);
```

## 注意事项

- 所有操作器通过 `Register2FITKOperatorRepo` 宏注册到仓库
- 操作器名称应保持唯一，建议使用 `actionXXXTreeYYY` 命名规范
- 业务逻辑应委托给 OperatorsModel，本模块只负责 UI 逻辑

最后更新时间：2026-04-28