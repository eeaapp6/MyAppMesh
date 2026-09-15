# FITKHttpPythonDriver

## 模块说明
`FITKHttpPythonDriver` 是一个后台 HTTP 驱动组件，用于接收 HTTP 请求并通过 `FITKPython` 在宿主进程内执行 Python 命令，再将执行结果以 JSON 形式返回给调用方。

当前组件以服务形态运行，不提供界面部件（`getWidget()` 返回 `nullptr`）。

## 主要文件
- `FITKHttpPythonDriverInterface.h/.cpp`：组件生命周期入口，负责初始化并启动 `HttpClient`。
- `HttpClient.h/.cpp`：HTTP 监听、配置加载、请求解析、跨域响应、回包。
- `FITKHttpPythonExecutor.h/.cpp`：封装 Python 命令执行与错误汇总。
- `RunTimeMessage.h/.cpp`：缓存运行时错误消息并提供读取/清理接口。
- `FITKHttpPythonDriver.ini.template`：默认配置模板。

## 请求处理流程
1. 组件初始化时启动 `HttpClient`。
2. `HttpClient` 从可执行目录读取 `FITKHttpPythonDriver.ini`（不存在时按模板/默认值创建）。
3. `WFHttpServer` 接收请求后进入 `handleTask()`：
   - 解析 method、uri、body。
   - 添加 CORS 响应头。
   - 对 `OPTIONS` 预检直接返回 `204`。
   - 其余请求将 body 作为 Python 命令执行。
4. 执行失败返回 `400` + JSON 错误信息；执行成功返回 `200` + JSON 确认信息。

## 配置
配置文件路径：可执行目录下 `FITKHttpPythonDriver.ini`

示例：

```ini
[server]
host=127.0.0.1
port=8890
```

- `host`：监听地址
- `port`：监听端口（有效范围 1-65535）

## 响应约定
- 成功：

```json
{"code":0,"message":"received","uri":"/..."}
```

- 失败：

```json
{"code":1,"message":"...error...","uri":"/..."}
```

## 联调建议
- 浏览器端出现 `Failed to fetch` 且后端可达时，优先检查是否已加载最新 DLL 以及 CORS/OPTIONS 响应是否生效。
- 若构建报 `LNK1168`，通常是目标 DLL 正被运行中的进程占用，需先关闭占用进程再重建。
- 若返回 `400`，可结合 `RunTimeMessage` 缓存错误和 `FITKPythonInterface` 错误信息定位 Python 执行问题。
