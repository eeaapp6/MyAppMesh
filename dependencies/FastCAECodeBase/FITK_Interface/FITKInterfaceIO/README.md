# FITKInterfaceIO

## 简介

FITKInterfaceIO 是 `FITK_Interface` 层中的 IO 抽象接口模块，负责提供文本、XML、JSON、HDF5 读写的统一抽象基类与工具能力。

该模块定位是“接口层抽象”，核心职责是为上层组件（求解器 IO、几何/网格 IO、工程文件读写）提供可继承、可复用的读写骨架，而不是直接承载某个业务格式的完整实现。

## 架构

模块按“公共抽象基类 + 文件格式抽象 + HDF5工具类”组织：

1. 公共基类：`Interface::FITKAbstractIO`
2. 文本抽象：`FITKAbstractTextReader`、`FITKAbstractTextStreamReader`、`FITKAbstractTextWriter`
3. XML/JSON 抽象：`FITKAbstractQtXMLReader`、`FITKAbstractQtXMLWriter`、`FITKAbstractRapidXMLReader`、`FITKAbstractRapidXMLWriter`、`FITKAbstractQtJsonReader`
4. HDF5 抽象与工具：`IO::FITKAbstractHDF5Reader`、`IO::FITKAbstractHDF5Writer`、`IO::FITKHDF5FileTool`

上下游关系（基于仓库现有调用）：

1. 上游依赖：`FITKCore` 线程任务能力（`FITKThreadTask`）和 Qt 基础设施。
2. 下游使用：`OperatorsModel` 中工程文件读写线程、`FITK_Component` 中 INP/网格读写器、`FITKInterfaceIOHDF5` 与 `FITKInterfaceGeoIOHDF5` 中的 HDF5 适配器。

## 依赖

- 内部依赖：`FITKCore`
- 外部依赖：`Qt5::Core`、`Qt5::Gui`、`Qt5::Widgets`、`Qt5::Xml`、`HDF5::HDF5`、RapidXML（通过 `TOOLS_DIR` 包含路径）
- 运行依赖：
  - Windows 下需可访问 `TOOLS_DIR/Win64/rapidxml` 头文件路径（编译期）。
  - HDF5 运行库需与应用部署环境匹配（模块本身通过 HDF5 C++ API 调用）。

## 主要类与功能说明

| 类名 | 职责 | 备注 |
| --- | --- | --- |
| `Interface::FITKAbstractIO` | 统一文件名、数据对象、线程投递与消息输出接口 | 继承 `Core::FITKThreadTask`，派生类可直接 `push2ThreadPool()` |
| `Interface::FITKAbstractTextReader` | 基于内存缓存的逐行文本读取 | 常用于 INP 等文本格式解析 |
| `Interface::FITKAbstractTextStreamReader` | 基于流的文本读取（适合大文件） | 当前仓库内暂未检索到模块外派生类 |
| `Interface::FITKAbstractTextWriter` | 文本写出与格式化工具 | 提供 `openFileStream()`、`getStream()`、`double2String()` |
| `Interface::FITKAbstractQtXMLReader` | 基于 Qt DOM 的 XML 读取抽象 | 负责创建文档上下文 `createContent()` |
| `Interface::FITKAbstractQtXMLWriter` | 基于 Qt DOM 的 XML 写出抽象 | 负责写出前文档上下文创建 |
| `Interface::FITKAbstractRapidXMLReader` | 基于 RapidXML 的 XML 读取抽象 | 支持进度上报 |
| `Interface::FITKAbstractRapidXMLWriter` | 基于 RapidXML 的 XML 写出抽象 | 提供 `writeXML()` |
| `Interface::FITKAbstractQtJsonReader` | 基于 Qt JSON 的读取抽象 | 统一 JSON 文件与 `QJsonDocument` 上下文 |
| `IO::FITKAbstractHDF5Reader` | HDF5 读取骨架 | 提供 `createContext()`、`getHDF5FileTool()` |
| `IO::FITKAbstractHDF5Writer` | HDF5 写出骨架 | 提供 `createContext()`、`setH5Root()`、`closeFile()` |
| `IO::FITKHDF5FileTool` | HDF5 Group/Attribute/DataSet 常用操作封装 | 被 HDF5 Reader/Writer 及适配器复用 |

## 构建或集成方式

该模块通过 `FITK_Interface/FITKInterfaceIO/CMakeLists.txt` 以共享库方式构建：

1. 工程定义：`project(FITKInterfaceIO VERSION 1.0.1 LANGUAGES CXX)`
2. 目标类型：`add_library(FITKInterfaceIO SHARED)`
3. 自动生成：`CMAKE_AUTOMOC`、`CMAKE_AUTORCC`、`CMAKE_AUTOUIC`
4. 关键链接：Qt5、`HDF5::HDF5`、`FITKCore`
5. 关键前置：必须配置 `TOOLS_DIR`（用于 RapidXML 路径和 `HDF5Config.cmake`）

示例（模块级构建）：

```bash
cmake -S . -B build -DTOOLS_DIR=/path/to/Tools
cmake --build build --config Release
```

## 使用方法

1. 选择合适的抽象基类（文本、XML、JSON、HDF5）。
2. 在派生类中实现 `run()` 与 `consoleMessage()`，并按需要实现具体读写逻辑。
3. 设置文件名（`setFileName`）与数据对象（`setDataObject`）。
4. 需要异步执行时，直接调用 `push2ThreadPool()`。
5. HDF5 场景先调用 `createContext(QString& error)`，再通过 `getHDF5FileTool()` 完成组、属性与数据集操作。

## 使用示例

以下示例均为“最小闭环”写法，接口名与本模块真实头文件保持一致。

### 示例1：自定义文本读取器

```cpp
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"

class DemoInpReader : public Interface::FITKAbstractTextReader
{
public:
    void run() override
    {
        if (!loadFile()) return;

        while (!atEnd())
        {
            const QString line = readLine().simplified();
            if (line.isEmpty()) continue;
            // TODO: 解析 line。
        }

        sendCurrentPercent(100);
    }

    void consoleMessage(int level, const QString& str) override
    {
        Q_UNUSED(level)
        Q_UNUSED(str)
    }
};
```

### 示例2：自定义文本写出器

```cpp
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"

class DemoTextWriter : public Interface::FITKAbstractTextWriter
{
public:
    void run() override
    {
        if (!openFileStream()) return;

        QTextStream* os = getStream();
        if (os)
        {
            *os << "*NODE\n";
            *os << int2String(1, 8) << "," << double2String(0.0) << "\n";
        }

        closeFileStream();
    }

    void consoleMessage(int level, const QString& str) override
    {
        Q_UNUSED(level)
        Q_UNUSED(str)
    }
};
```

### 示例3：自定义 HDF5 读取器

```cpp
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

class DemoH5Reader : public IO::FITKAbstractHDF5Reader
{
public:
    void run() override
    {
        QString error;
        if (!createContext(error))
        {
            consoleMessage(3, error);
            return;
        }

        IO::FITKHDF5FileTool* tools = getHDF5FileTool();
        if (!tools)
        {
            closeFile();
            return;
        }

        auto gp = tools->openGp("/Model");
        if (std::get<0>(gp))
        {
            int version = 0;
            tools->readGroupint(std::get<1>(gp), "Version", &version);
        }

        closeFile();
    }

    void consoleMessage(int level, const QString& str) override
    {
        Q_UNUSED(level)
        Q_UNUSED(str)
    }
};
```

### 示例4：自定义 HDF5 写出器

```cpp
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

class DemoH5Writer : public IO::FITKAbstractHDF5Writer
{
public:
    void run() override
    {
        QString error;
        if (!createContext(error))
        {
            consoleMessage(3, error);
            return;
        }

        IO::FITKHDF5FileTool* tools = getHDF5FileTool();
        if (tools)
        {
            auto modelGroup = tools->createGp("/Model");
            if (std::get<0>(modelGroup))
            {
                tools->writeGroupAttrInt(std::get<1>(modelGroup), "Version", 1);
            }
        }

        closeFile();
    }

    void consoleMessage(int level, const QString& str) override
    {
        Q_UNUSED(level)
        Q_UNUSED(str)
    }
};
```

### 示例5：自定义 Qt JSON 读取器

```cpp
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractQtJsonReader.h"

class DemoJsonReader : public Interface::FITKAbstractQtJsonReader
{
public:
    void run() override
    {
        if (!createContent()) return;

        // _document 为基类中受保护成员，可在派生类中读取 JSON 内容。
        // TODO: 读取 _document 并映射到业务对象。

        closeFile();
    }

    void consoleMessage(int level, const QString& str) override
    {
        Q_UNUSED(level)
        Q_UNUSED(str)
    }
};
```

### 示例6：线程池执行方式

```cpp
DemoH5Reader* reader = new DemoH5Reader();
reader->setAutoDelete(true);
reader->setFileName("D:/work/model.hdf5");
reader->push2ThreadPool();
```

## 注意事项

1. `FITKAbstractIO` 继承 `FITKThreadTask`，派生类需实现 `run()` 与 `consoleMessage()`。
2. HDF5 读写必须先确保上下文可用：`createContext()` 成功后再调用工具方法。
3. `setH5Root()` 场景下文件指针由外部持有，析构时不会自动释放外部对象。
4. 文本写出建议配对调用 `openFileStream()` / `closeFileStream()`。
5. 当前仓库内 `FITKAbstractTextStreamReader` 尚未检索到模块外派生实现；新增大文件读取功能时可优先复用该基类。
6. 本 README 示例用于说明接入方式，具体业务字段请以对应组件的适配器实现为准。

## 许可证

本模块许可证以当前目录 `License.txt` 为准（BSD 3-Clause）。

最后更新时间：2026-04-25
