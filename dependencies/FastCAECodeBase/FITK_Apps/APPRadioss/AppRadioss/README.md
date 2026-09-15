# AppRadioss - Radioss应用程序入口模块

## 简介

AppRadioss 将 FITKAppFramework 与 Radioss 仿真软件的具体实现相结合，定义应用程序的主程序入口、全局数据工厂、主窗口生成器、组件工厂等核心装配逻辑。

## 模块定位

- 定义应用程序的 main() 入口函数
- 提供主窗口生成器，创建 Radioss 主界面
- 提供全局数据工厂，定义 Radioss 数据的组织结构
- 提供组件工厂，注册和创建 Radioss 功能组件
- 提供信号处理器，响应程序驱动和进度信号

## 架构与目录

```text
AppRadioss
├── main.cpp                       应用程序入口
├── AppInitializer.h / .cpp         应用程序初始化器
├── MainWindowGenerator.h / .cpp   主窗口生成器
├── GlobalDataFactory.h / .cpp      全局数据工厂
├── ComponentFactory.h / .cpp       组件工厂
├── SignalProcessor.h / .cpp        信号处理器
├── CommandLineHandler.h / .cpp      命令行处理器
└── RadiossAppSettings.h / .cpp    应用设置
```

## 依赖关系

- Qt5::Core、Qt5::Gui、Qt5::Widgets
- FITKAppFramework、FITKCore
- GUIFrame、GUIWidget、OperatorsGUI、OperatorsModel
- GraphDataProvider、FITKRadiossData

## 主要类与功能说明

### main.cpp

应用程序入口点：

```cpp
int main(int argc, char* argv[]) {
    AppFrame::FITKApplication app(argc, argv);
    app.regMainWindowGenerator(new MainWindowGenerator);
    app.regGlobalDataFactory(new GlobalDataFactory);
    app.regComponentsFactory(new ComponentFactory);
    app.regAppSettings(new RadiossAppSettings);
    return app.exec();
}
```

### GlobalDataFactory

全局数据工厂，定义 Radioss 数据组织结构：

```cpp
createMeshData();     // 创建网格数据
createGeoData();      // 创建几何数据
createPhysicsData();  // 创建物理场数据
createPostData();      // 创建后处理数据
```

### MainWindowGenerator

主窗口生成器，返回 GUI::MainWindow 实例。

## 注意事项

- AppRadioss 是应用程序入口模块，构建为可执行文件
- 所有依赖模块需要在 CMakeLists.txt 中正确声明
- 应用程序关闭时应正确清理全局单例

最后更新时间：2026-04-28