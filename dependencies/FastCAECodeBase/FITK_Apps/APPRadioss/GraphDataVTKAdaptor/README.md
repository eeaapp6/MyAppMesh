# GraphDataVTKAdaptor - 三维可视化VTK适配器模块

## 简介

GraphDataVTKAdaptor 负责将业务数据对象转换为 VTK 可视化对象，包含两类核心组件：GraphVTKObject3D 系列作为可视化对象基类，以及 GraphVTKViewAdaptor 系列作为数据到对象的适配转换器。

## 模块定位

- 定义 VTK 可视化对象基类及各类专用渲染对象
- 实现从数据对象到 VTK 可视化对象的适配转换器
- 管理可视化对象的颜色、透明度、点大小等外观属性
- 提供拾取、高亮、选择等交互能力
- 通过 FITKViewAdaptorFactory 注册适配器

## 架构与目录

```text
GraphDataVTKAdaptor
├── GraphVTKObject3D.h / .cpp           可视化对象基类
├── GraphVTKObjectBCBase.h / .cpp        边界条件基类
├── GraphVTKObjectModelBase.h / .cpp     模型基类
├── GraphVTKObjectFieldBase.h / .cpp     场数据基类
├── GraphVTKObjectPick.h / .cpp          拾取对象
├── GraphVTKViewAdaptorBase.h / .cpp    适配器基类
├── GraphVTKViewAdaptorModel.h / .cpp   模型适配器
├── GraphVTKViewAdaptorPartMesh.h / .cpp Part网格适配器
└── GraphVTKCommons.h / .cpp             公共工具
```

## 依赖关系

- Qt5::Core、VTK
- FITKCore、FITKAdaptor、FITKRenderWindowVTK、FITKRadiossData

## 主要类与功能说明

### GraphVTKObject3D

可视化对象基类，继承自 FITKGraphObjectVTK：

```cpp
obj->setColor(QColor(255, 0, 0));      // 设置颜色
obj->setTransparent(true);            // 设置透明度
obj->setPointSize(3);                  // 设置点大小
obj->setVisible(true);                 // 设置可见性
obj->setPickable(true);                // 设置可拾取性
```

### GraphVTKViewAdaptorBase

适配器基类，继承自 FITKAbstractViewAdaptor：

```cpp
auto adaptor = FITKVIEWADAPTORFACTORY->createT<GraphVTKViewAdaptorBase>(keyName, dataObj);
adaptor->setCaseData(caseData);
Exchange::GraphVTKObject3D* obj = adaptor->getOutputData();
```

### 专用可视化对象

- `GraphVTKObjectBCS` - 边界条件箭头渲染（T-蓝色，W-黄色）
- `GraphVTKObjectRBODY` - 刚体
- `GraphVTKObjectRigidWall` - 刚体墙
- `GraphVTKObjectGravity` - 重力载荷
- `GraphVTKObjectPartMesh` - Part 网格

### 专用适配器

通过 `Register2FITKViewAdaptorFactory` 宏注册各类适配器。

## 注意事项

- 所有适配器通过注册宏注册，注册 key 必须唯一
- 可视化对象需要关联正确的数据对象类型
- 依赖 VTK，确保项目正确链接 VTK 库

最后更新时间：2026-04-28