# OperatorsInterface - 操作器接口定义模块

## 简介

OperatorsInterface 定义图形界面交互相关操作器的抽象基类和接口规范，不实现具体业务逻辑。

## 模块定位

- 定义图形拾取操作的标准接口
- 定义图形数据操作的标准接口
- 定义树形菜单事件的标准接口
- 定义 ComboBox 控件事件的标准接口

## 架构与目录

```text
OperatorsInterface
├── GraphEventOperator.h / .cpp        三维可视化数据操作器接口
├── GraphInteractionOperator.h / .cpp  三维拾取操作器接口
├── TreeEventOperator.h / .cpp          树形菜单事件操作器
└── ComboboxEventOperator.h / .cpp     ComboBox事件处理器
```

## 依赖关系

- Qt5::Core、FITKCore
- FITKRenderWindowVTK、GUIWidget

## 主要类与功能说明

### GraphEventOperator

三维可视化数据操作器接口：

- `updateAll()` - 更新全部可视化对象
- `updateGraph(dataObjId)` - 按 ID 更新可视化对象
- `getModelGraphObjectByDataId()` - 获取模型可视化对象
- `setModelVisible()` - 设置可见性
- `render()` - 刷新渲染

### GraphInteractionOperator

三维拾取操作器接口：

```cpp
virtual void picked(vtkActor*, index, worldPos);   // 拾取单个
virtual void picked(actors, cutPlane);              // 框选拾取
virtual void clear();                                // 清除高亮
virtual void setPickedColor(color);                 // 设置拾取颜色
```

### TreeEventOperator

树形菜单事件操作器：

- `updateTree()` - 更新树形菜单
- `on_itemStateChanged()` - 节点状态改变事件
- `on_itemSelectionChanged()` - 节点选择变化事件

## 注意事项

- 本模块是接口定义模块，不包含具体业务实现
- 所有虚函数都有默认空实现，可选择性重写
- 图形拾取依赖 VTK，确保正确引入 VTK 头文件

最后更新时间：2026-04-28