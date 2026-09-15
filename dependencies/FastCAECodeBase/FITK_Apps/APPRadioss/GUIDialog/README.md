# GUIDialog - 图形界面对话框模块

## 简介

GUIDialog 实现 Radioss 仿真软件的各种对话框界面，按功能分为分析对话框、后处理对话框、几何对话框、网格对话框和工具对话框五个子模块。

## 模块定位

- 实现材料属性编辑对话框
- 实现截面属性和边界条件设置对话框
- 实现接触和相互作用配置对话框
- 实现求解器设置和后处理配置对话框
- 实现几何变换和网格设置对话框

## 架构与目录

```text
GUIDialog
├── GUIAnalysisDialog      GUIPropertyDialog、GUIMaterialPropDialog
├── GUIPostDialog          GUISolverRunDialog、GUIRadiossResultRequestDialog
├── GUIGeometryDialog      GUIGeometryBoxDialog、GUIGeometryBoolDialog
├── GUIMeshDialog          GUIPartPickDialog
└── GUIToolDialog          GUIRenameDialog、GUIGraphSettingDialog
```

## 依赖关系

- Qt5::Core、Qt5::Gui、Qt5::Widgets
- FITKCore、FITKRadiossData、FITKInterfaceModel、GUIWidget

## 主要类与功能说明

### GUIAnalysisDialog 子模块

- `GUIPropertyDialog` - 截面属性对话框基类
- `GUIMaterialPropDialog` - 材料属性对话框基类
- `GUIBCSDialog` - 边界条件对话框
- `GUIContactCreateDialog` - 接触创建对话框

### GUIPostDialog 子模块

- `GUISolverRunDialog` - 求解器运行对话框
- `GUISolverSettingDialog` - 求解器参数设置
- `GUIRadiossResultRequestDialog` - 结果请求配置
- `GUIColorBarDialog` - 颜色条设置

### 使用示例

```cpp
// 创建材料对话框
GUI::GUIMaterialPropDialog dialog(
    getOperator(),
    manager->getDataByID(selectedID),
    MainWindow::GetMainWindowFromFramework()
);
dialog.exec();

// 对话框数据同步
void GUIMaterialPropDialog::on_pushButton_OK_clicked() {
    _material->setName(ui->lineEdit_Name->text());
    callOperator("actionMaterialPropCreate");
    accept();
}
```

## 注意事项

- 对话框继承自 FITKDialog（FITKCore 提供）
- 对话框的 UI 文件使用 Qt Designer 设计
- 数据检查逻辑在 `checkData()` 中实现

最后更新时间：2026-04-28