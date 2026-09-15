# OperatorsModel - 模型业务操作器模块

## 简介

OperatorsModel 实现 Radioss 仿真软件的核心业务逻辑操作，包括材料管理、边界条件、载荷施加、求解器设置、网格生成、文件导入导出等。

## 模块定位

- 实现材料属性的创建、编辑、删除等业务操作
- 实现边界条件和载荷的业务逻辑处理
- 实现分析步骤和求解器设置的业务操作
- 实现 Part 和几何模型的管理操作
- 实现文件的导入和导出操作

## 架构与目录

```text
OperatorsModel
├── 文件操作                              OperatorsNewProject、OperatorsSaveAs
├── 模型与几何                            OperatorImportGeometry、OperatorPart
├── 属性与材料                            OperatorMaterialProp、OperatorProperty
├── 边界条件与载荷                        OperatorBCS、OperatorRigidWallProp
├── 接触与相互作用                        OperatorContact、OperatorContactTie
├── 求解与分析                            OperatorCreateSolution、OperSolverSettings
├── 后处理                                OperPost3D、OperatorProbe、OperatorColorBar
└── 网格生成                              OperGmshGenerator、OperGmshSettings
```

## 依赖关系

- Qt5::Core、FITKCore
- FITKRenderWindowVTK、GraphDataProvider、FITKRadiossData
- FITKInterfaceMesh、FITKInterfaceModel、FITKMeshIO

## 主要类与功能说明

### 通用操作器模式

所有操作器继承自 FITKActionOperator，通过 `Register2FITKOperatorRepo` 宏注册：

```cpp
class OperatorMaterialProp : public Core::FITKActionOperator {
    bool execGUI() override;      // 执行界面交互
    bool execProfession() override;  // 执行业务逻辑
};
Register2FITKOperatorRepo(actionMaterialPropCreate, OperatorMaterialProp);
```

### 典型业务操作

```cpp
// 边界条件操作
OperatorBCS::bcsCreateOper();   // 创建边界条件
OperatorBCS::bcsEditOper();      // 编辑边界条件
OperatorBCS::bcsDeleteOper();    // 删除边界条件

// 材料操作
materialPropRenameOper();       // 重命名材料
deleteMaterial();                // 删除材料
```

## 注意事项

- 操作器的 `execGUI()` 处理界面交互，`execProfession()` 处理业务逻辑
- 数据变更后应及时调用 GraphDataProvider 更新图形显示
- 文件 IO 操作建议使用后台线程，避免阻塞 UI

最后更新时间：2026-04-28