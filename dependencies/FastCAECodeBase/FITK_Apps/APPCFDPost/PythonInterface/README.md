# PythonInterface

## 简介

PythonInterface 是 APPCFDPost 应用中面向 PythonQt 的文件操作包装层模块。

它不负责嵌入 Python 解释器本身，解释器和脚本环境由 FITKPython 模块提供。

PythonInterface 的职责是把当前应用的文件操作（主要是 VTK 文件打开）包装成 PythonQt 可见的类和 decorator，供脚本层调用。

## 模块定位

- 所在目录：PythonInterface
- 构建产物：共享库 PythonInterface.dll
- 主要使用者：APPCFDPost 中的 PythonQt 脚本注册流程
- 主要上游依赖：FITKCore

## 架构与目录

```
PythonInterface
├── PythonInterfaceAPI.h      # 导出宏定义
├── GlobalFiles.h            # 文件操作包装类声明
├── GlobalFiles.cpp          # 文件操作包装类实现
└── CMakeLists.txt           # 构建配置
```

## 依赖关系

当前模块依赖：

- Qt5::Core
- Qt5::Gui
- Qt5::Widgets
- FITKCore
- Python37::Python37
- PythonQt::PythonQt

## 主要类与功能说明

### FilesWapper

FilesWapper 是文件操作接口的 PythonQt 装饰器类，继承自 QObject。

主要功能：

- 创建 Files 对象实例
- 删除 Files 对象实例
- 通过 FITKActionOperator 打开 VTK 文件

主要接口：

```cpp
Files* new_Files();                                          // 创建 Files 对象
void delete_Files(Files* F);                                 // 删除 Files 对象
void static_Files_OpenVTKFiles(const QString& fileName);     // 打开 VTK 文件
```

## 使用方法

### 场景 1：在脚本环境中注册 FilesWapper

```cpp
// 通过 PythonQt 注册装饰器
FilesWapper* filesWrapper = new FilesWapper();
_pyInterface->addDecorator(filesWrapper);
_pyInterface->registerCPPClass("Files", "", "Global");
```

### 场景 2：在脚本中打开 VTK 文件

```python
# 通过装饰器调用
Files.static_Files_OpenVTKFiles("path/to/your/file.vtk")
```

### 场景 3：创建和管理 Files 对象

```python
# 创建 Files 对象
f = Files()

# 打开 VTK 文件
Files.static_Files_OpenVTKFiles("path/to/your/file.vtk")

# 删除对象
Files.delete_Files(f)
```

## 文件结构说明

| 文件 | 说明 |
|------|------|
| PythonInterfaceAPI.h | 导出宏定义，定义 PythonInterfaceAPI 装饰器 |
| GlobalFiles.h | FilesWapper 和 Files 类的声明 |
| GlobalFiles.cpp | FilesWapper 类的实现，包含 VTK 文件打开逻辑 |
| CMakeLists.txt | CMake 构建配置 |

## 注意事项

- PythonInterface 模块目前仅提供文件操作相关的 PythonQt 包装，不包含几何建模等功能
- 打开 VTK 文件功能依赖 FITKCore 中的 FITKActionOperator（actionOpen 操作器）
- 模块使用 FITKThreadPool 等待文件异步加载完成
- 注册到 PythonQt 时，FilesWapper 作为装饰器使用

## 最后更新时间

最后更新时间：2026-06-02
