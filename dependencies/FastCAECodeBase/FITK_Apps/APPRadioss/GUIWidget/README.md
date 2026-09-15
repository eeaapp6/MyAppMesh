# GUIWidget - 界面组件与图形窗口模块

## 简介

GUIWidget 负责构建主窗口中的各类 UI 组件，包括图形显示区域（前后处理）、树形菜单面板、控制面板和对话框基类。

## 模块定位

- 实现前后处理图形显示区域组件
- 构建装配、属性、求解器、结果等树形菜单
- 提供参数编辑和显示面板
- 定义对话框基类供 GUIDialog 继承
- 集成 FITKRenderWindowVTK 实现三维渲染能力

## 架构与目录

```text
GUIWidget
├── GraphArea.h / .cpp / .ui           图形显示区域基类
├── GraphMdiArea.h / .cpp               MDI容器组件
├── PreGraphWidget.h / .cpp            前处理图形窗口
├── PostGraphWidget.h / .cpp           后处理图形窗口
├── ControlPanel.h / .cpp              控制面板
├── TreeWidgetAssembly.h / .cpp        装配树形菜单
├── TreeWidgetProperty.h / .cpp        属性树形菜单
├── TreeWidgetSolution.h / .cpp        求解器树形菜单
├── ResultTreeWidget.h / .cpp           结果树形菜单
└── GUIDialogBase.h / .cpp             对话框基类
```

## 依赖关系

- Qt5::Core、Qt5::Gui、Qt5::Widgets
- FITKCore、FITKWidget、FITKRenderWindowVTK、FITKRadiossData

## 主要类与功能说明

### GraphArea / GraphMdiArea

图形显示区域组件：

```cpp
graphArea->addObject(layerIndex, obj, true);  // 添加渲染对象
graphArea->fitView();                          // 自适应对象大小
graphArea->clear();                            // 清空所有显示对象
Comp::FITKGraph3DWindowVTK* vtkWin = graphArea->getRenderWindow();
```

### TreeWidgetAssembly / TreeWidgetProperty / TreeWidgetSolution

树形菜单组件，管理和显示模型、属性、求解器等数据。

### ControlPanel

控制面板，继承自 QDockWidget：

```cpp
controlPanel->getAssemblyTree();    // 获取装配树
controlPanel->getPropertyTree();     // 获取属性树
controlPanel->getSolutionTree();    // 获取求解器树
controlPanel->setPage("Assembly");  // 设置显示页面
```

## 注意事项

- GraphArea 是抽象基类，需派生具体类型
- 图形窗口依赖 FITKRenderWindowVTK
- GUIDialogBase 中定义了 TreeWidgetItem 的数据存储 Role 宏

最后更新时间：2026-04-28