# Tasks: APPMesh Core Functions

**Input**: `plan.md`, `spec.md`, `research.md`, `data-model.md`, `contracts/`

**Task format**: `- [ ] [TaskID] [P?] [Stage?] Description | 前置任务 | 影响模块 | 需求编号 | 设计章节 | 输入 | 输出 | 验收条件`

## Phase 1: FastCAE 工程和应用生命周期

- [ ] T001 [P] [S1] 初始化 CMake 工程和 Debug/Release 运行目录，建立 `CMakeLists.txt`、`src/`、`tests/` 和依赖查找脚本 | 前置任务: 无 | 影响模块: 构建系统、MeshApp | 需求编号: FR-001, FR-022 | 设计章节: plan.md Project Structure | 输入: Qt5、VTK、SARibbon、FastCAE/FITK、HDF5 路径 | 输出: 可配置构建工程 | 验收条件: Debug/Release 均生成空应用并定位依赖，不引用原 APPMesh 源码
- [ ] T002 [P] [S1] 实现 `src/app/SystemChecker.{h,cpp}` 和 `src/app/MeshAPPSettings.{h,cpp}`，完成环境检查、版本化配置、工作目录和最近文件读写 | 前置任务: T001 | 影响模块: SystemChecker、MeshAPPSettings | 需求编号: FR-003, FR-004 | 设计章节: plan.md Core Interfaces and Classes | 输入: 命令行、环境变量、配置文件 | 输出: `RuntimeCheckResult`、`AppSettings` | 验收条件: 缺依赖、不可写目录、磁盘不足均返回诊断错误；正常配置可恢复
- [ ] T003 实现 `src/app/GlobalDataFactory.{h,cpp}`、`src/app/ComponentFactory.{h,cpp}` 的 FastCAE 注册入口和生命周期管理 | 前置任务: T001 | 影响模块: GlobalDataFactory、ComponentFactory | 需求编号: FR-001, FR-002 | 设计章节: plan.md Module Boundaries and Dependencies | 输入: FastCAE 工厂/组件接口 | 输出: 工厂注册和组件创建 API | 验收条件: 全局数据先于组件创建，组件按依赖初始化，失败可失败诊断且不进入事件循环
- [ ] T004 实现 `src/app/MeshApp.{h,cpp}`、`MainWindowGenerator`、`AppInitializer`、`PreWindowInitializer`、`SignalProcessor`、`PyRegister` 的启动/退出编排 | 前置任务: T002, T003 | 影响模块: MeshApp、初始化辅助类 | 需求编号: FR-001, FR-002, FR-003, FR-004, FR-021 | 设计章节: plan.md Application Initialization Order | 输入: 工厂、设置、检查结果、FastCAE Application | 输出: 启动器和退出序列 | 验收条件: 检查→全局数据→组件→Python 注册→窗口→插件→命令行/Workbench→事件循环；退出逆序释放

## Phase 2: 全局数据和几何/网格数据模型

- [ ] T005 [P] [S2] 实现 `src/model/ModelData/DataObject.{h,cpp}`、`ApplicationRuntime` 和线程安全 ID/名称索引 | 前置任务: T003 | 影响模块: ModelData | 需求编号: FR-005, FR-007, FR-020 | 设计章节: data-model.md Common DataObject | 输入: 对象类型、名称、父对象 | 输出: 稳定 ID、唯一命名和生命周期 API | 验收条件: ID 创建后不变、管理器内名称唯一、删除无悬空索引
- [ ] T006 [P] [S2] 实现 `GeometryObject`、`GeometryManager` 及导入暂存、拓扑实体和显示状态模型 | 前置任务: T005 | 影响模块: ModelData | 需求编号: FR-005, FR-006 | 设计章节: data-model.md GeometryObject | 输入: 已解析几何、元数据 | 输出: 可校验几何对象 | 验收条件: 无效对象不入库；重名自动修正；查询、分组、可见性和删除可用
- [ ] T007 [P] [S2] 实现 `MeshKernel`、`MeshData`、节点/单元/集合结构和完整性校验 | 前置任务: T005 | 影响模块: ModelData | 需求编号: FR-007, FR-010 | 设计章节: data-model.md MeshData and MeshKernel | 输入: 节点、单元、集合、geometryId、参数 | 输出: 校验后的网格模型 | 验收条件: ID 唯一、节点引用存在、cell arity/维度/集合引用校验可定位错误
- [ ] T008 实现 `MeshManager`、`MeshDataCreator` 注册/注销和 Geometry-Mesh 关联索引 | 前置任务: T006, T007 | 影响模块: ModelData | 需求编号: FR-007, FR-008, FR-010 | 设计章节: plan.md Core Interfaces; data-model.md | 输入: 网格类型、创建器、几何 ID | 输出: 网格创建/查询/删除服务 | 验收条件: 重复注册有明确错误；创建失败不入库；删除不产生悬空关联
- [ ] T009 [P] [S2] 编写 ModelData 单元和线程安全测试 `tests/unit/model/` | 前置任务: T005-T008 | 影响模块: ModelData、测试 | 需求编号: FR-020, FR-021 | 设计章节: plan.md Testing Strategy | 输入: 重名对象、非法拓扑、并发夹具 | 输出: CTest 测试套件 | 验收条件: ID/名称/拓扑/关联不变量覆盖，失败返回 ErrorInfo

## Phase 3: 主窗口、模型树、控制台和三维窗口

- [ ] T010 [P] [S3] 实现 `src/gui/GUIFrame/MainWindow.{h,cpp}`、Ribbon、Dock 和主窗口装配 | 前置任务: T004, T006, T008 | 影响模块: GUIFrame、FastCAE GUI | 需求编号: FR-011, FR-001 | 设计章节: 系统设计报告第 5.3、14.1 节 | 输入: ComponentFactory、GraphData 视图端口 | 输出: 主窗口和菜单入口 | 验收条件: 主窗口、Ribbon、模型树、控制台和三维区域可创建
- [ ] T011 [P] [S3] 实现 `src/gui/GUIWidget/ModelTree.{h,cpp}`、节点模型和 `ConsoleWidget` | 前置任务: T006, T008 | 影响模块: GUIWidget、控制台 | 需求编号: FR-005, FR-011 | 设计章节: 系统设计报告第 5.4、7 节 | 输入: ModelData 查询、任务/日志事件 | 输出: 几何/网格树和控制台 | 验收条件: 对象增删、命名、可见性、选择与数据同步，控制台展示诊断
- [ ] T012 实现 `src/gui/GUIDialog/WorkDirectoryDialog`、`GeneratorDialog`、`ProjectDialog` 输入收集与校验 | 前置任务: T002, T010 | 影响模块: GUIDialog | 需求编号: FR-004, FR-008, FR-013 | 设计章节: plan.md GUIWidget/GUIDialog | 输入: 设置、具体生成器参数、文件路径 | 输出: 结构化参数 | 验收条件: 只收集/校验输入，不执行长任务；非法输入被拒绝
- [ ] T013 [P] [S3] 编写 GUI 冒烟和 FastCAE/VTK 装配测试 `tests/integration/gui/` | 前置任务: T010-T012 | 影响模块: GUIFrame、GUIWidget、GraphData | 需求编号: FR-011, FR-015, SC-001, SC-002 | 设计章节: plan.md Testing Strategy | 输入: 最小运行时 | 输出: GUI 集成测试 | 验收条件: UI 启动成功，后台任务期间事件循环可处理

## Phase 4: 操作器和几何导入

- [ ] T014 [P] [S4] 定义 `src/operators/OperatorsInterface/IOperator`、`Task`、`ErrorInfo`、事件和结果通知接口 | 前置任务: T005, T004 | 影响模块: OperatorsInterface、TaskService | 需求编号: FR-009, FR-015, FR-020, FR-021 | 设计章节: plan.md Core Interfaces; data-model.md Task/ErrorInfo | 输入: FastCAE 操作器/线程池接口 | 输出: 操作器和任务契约 | 验收条件: GUI/业务执行分离；状态 executing/success/failure；错误字段完整
- [ ] T015 实现 `ImportGeometryOperator`、`OperatorsModel::TaskService` 和 `OperatorsGUI` 路由 | 前置任务: T010, T012, T014 | 影响模块: OperatorsModel、OperatorsGUI | 需求编号: FR-006, FR-009, FR-015 | 设计章节: 系统设计报告第 6.1、14.4 节 | 输入: 文件路径、工作目录、几何 IO 适配器 | 输出: 异步导入任务 | 验收条件: 成功后原子提交 GeometryObject；失败不改变工程并显示诊断
- [ ] T016 [P] [S4] 接入首版必选几何格式适配器 `src/io/GeometryIO/` 并声明格式能力 | 前置任务: T015 | 影响模块: GeometryIO、FITK 适配 | 需求编号: FR-006, FR-012 | 设计章节: plan.md Module Boundaries; spec.md Clarifications | 输入: 已冻结的 BRep/STEP/STP/IGES/IGS 清单 | 输出: 格式注册、读取和校验适配器 | 验收条件: 清单内有效文件可导入；损坏/空文件/不支持版本有错误；清单变更同步测试夹具
- [ ] T017 [P] [S4] 编写几何导入单元、失败和端到端测试 `tests/integration/geometry/` | 前置任务: T015, T016 | 影响模块: OperatorsModel、GeometryIO、ModelData | 需求编号: FR-006, FR-020, SC-008 | 设计章节: quickstart.md End-to-end checks | 输入: 有效/无效/超内存文件 | 输出: 自动化验收结果 | 验收条件: 成功刷新树/视图；失败无残留；受控退出保留最后有效数据

## Phase 5: 网格管理和网格生成

- [ ] T018 实现各具体生成器操作器的参数收集和校验 | 前置任务: T008, T012, T014 | 影响模块: ModelData、OperatorsModel、GUIDialog | 需求编号: FR-008, FR-009 | 设计章节: contracts/plugin-protocol.md | 输入: 具体驱动要求的参数 | 输出: 驱动可接受的参数 | 验收条件: 按具体生成器规则拒绝非法参数；不引入统一首版 Schema
- [ ] T019 实现 `MeshGeneratorOperator`、隔离工作目录、流式日志和结果校验 | 前置任务: T014, T018 | 影响模块: OperatorsModel、FITK_Plugins adapter、TaskService | 需求编号: FR-009, FR-010, FR-009, FR-010 | 设计章节: plan.md Thread and Task Model | 输入: geometryId、参数、驱动配置 | 输出: 临时网格结果或 ErrorInfo | 验收条件: UI 不阻塞；状态/进度可观察；外部失败或畸形输出不替换旧网格；当前版不提供用户取消
- [ ] T020 [P] [S5] 接入 Gmsh、TetGen、FastCAE Grid 驱动适配器 `src/plugins/FITK_Plugins/generators/` | 前置任务: T019 | 影响模块: FITK_Plugins、外部程序适配 | 需求编号: FR-008, FR-008, FR-016, SC-003 | 设计章节: plan.md Plugin Protocol | 输入: FastCAE 插件接口、程序路径、参数 | 输出: 生成器驱动结果 | 验收条件: 已部署的驱动可按插件机制加载和执行；缺程序和非零退出可诊断
- [ ] T021 [P] [S5] 编写生成器契约、参数、失败隔离和结果校验测试 `tests/contract/generators/` | 前置任务: T018-T020 | 影响模块: FITK_Plugins、ModelData、TaskService | 需求编号: FR-008, FR-009, FR-010, SC-003, SC-004 | 设计章节: plan.md Testing Strategy | 输入: fake executable、非法参数、畸形输出 | 输出: 契约测试报告 | 验收条件: 注册/参数/结果/失败测试通过，旧网格保持不变

## Phase 6: 插件系统

- [ ] T022 实现 `FastCAE plugin metadata`、动态库入口、发现目录和 API/依赖兼容性检查 | 前置任务: T003, T020 | 影响模块: FITK_Plugins、ComponentFactory | 需求编号: FR-008, FR-016, FR-016 | 设计章节: contracts/plugin-protocol.md Discovery | 输入: DLL、descriptor、应用 API 版本 | 输出: 可加载清单和诊断 | 验收条件: 不兼容插件不加载，核心仍可启动
- [ ] T023 实现插件能力注册、组件/操作器注销和失败诊断 | 前置任务: T022 | 影响模块: FITK_Plugins、GlobalDataFactory、ComponentFactory | 需求编号: FR-016, FR-016 | 设计章节: contracts/plugin-protocol.md Lifecycle | 输入: 能力、创建器、UI 资源 | 输出: 注册状态和失败诊断结果 | 验收条件: 注册失败有明确诊断且核心应用保持可用
- [ ] T024 实现卸载流程及插件网格数据处置 `PluginManager.{h,cpp}` | 前置任务: T023, T008 | 影响模块: FITK_Plugins、ModelData、HDF5IO | 需求编号: FR-016 | 设计章节: plan.md Plugin Protocol; 系统设计报告第 17.3 节 | 输入: 插件任务、插件拥有数据 | 输出: 卸载结果和失败诊断 | 验收条件: 按具体 FastCAE 插件接口执行卸载，并对失败情况提供诊断
- [ ] T025 [P] [S6] 编写插件生命周期、兼容性、失败诊断、隔离和卸载测试 `tests/contract/plugins/` | 前置任务: T022-T024 | 影响模块: FITK_Plugins | 需求编号: FR-016, FR-021, SC-003 | 设计章节: plan.md Testing Strategy | 输入: 兼容/不兼容/半安装插件、活动任务、现存网格 | 输出: 插件契约测试 | 验收条件: 插件失败不影响核心；卸载数据处置分支全部通过

## Phase 7: 图形显示、点选和框选

- [ ] T026 实现 `src/graph/GraphData/GraphDataProvider`、几何/网格 actor 和增量刷新 | 前置任务: T006, T008, T010 | 影响模块: GraphData、VTK/FastCAE view | 需求编号: FR-011, FR-011 | 设计章节: 系统设计报告第 5.6、6、10 节 | 输入: ModelData 事件和对象 ID | 输出: actor 映射和刷新事件 | 验收条件: 状态变化只刷新受影响对象，树与视图一致
- [ ] T027 实现点选、框选、预选适配器和稳定实体 ID 回传 | 前置任务: T026, T011 | 影响模块: GraphData、GUIFrame | 需求编号: FR-011, FR-011 | 设计章节: plan.md Core Interfaces; 系统设计报告第 5.7 节 | 输入: VTK picking 事件 | 输出: 选择结果 | 验收条件: 点选/框选/预选高亮正确，结果可传给操作器
- [ ] T028 [P] [S7] 编写 GraphData/VTK 同步和交互测试 `tests/integration/graph/` | 前置任务: T026-T027 | 影响模块: GraphData、GUIFrame、ModelData | 需求编号: FR-011, SC-002 | 设计章节: plan.md Testing Strategy | 输入: 多对象夹具和拾取坐标 | 输出: 图形集成测试 | 验收条件: 状态同步、拾取 ID、刷新次数和 UI 线程约束可验证

## Phase 8: 文件 IO 和 HDF5 工程

- [ ] T029 实现 `HDF5IO` 工程上下文、Version 信息及插件读写分发 | 前置任务: T006, T008, T023 | 影响模块: HDF5IO、FITK_Plugins | 需求编号: FR-013, FR-014, FR-013, FR-014 | 设计章节: contracts/project-hdf5.md | 输入: 工程路径、版本信息、已加载插件 | 输出: HDF5 上下文和插件读写结果 | 验收条件: 正确写入/检查 `version` 和 `project_type`；按当前插件列表调用读写接口
- [ ] T030 实现工程打开的分阶段校验、匹配条件下插件尽力恢复和兼容性诊断 | 前置任务: T029, T023 | 影响模块: HDF5IO、ModelData、FITK_Plugins | 需求编号: FR-013, FR-013, FR-020 | 设计章节: plan.md Project File Structure; data-model.md Project | 输入: HDF5、当前上下文、已加载插件 | 输出: 新工程上下文或诊断列表 | 验收条件: 类型/版本/插件匹配时尽力恢复；未知数据诊断但不清空当前工程
- [ ] T031 [P] [S8] 实现 FITKMesh 导入及 FITKMesh/CGNS/INP 导出适配器和节点/单元/集合校验 | 前置任务: T008, T029 | 影响模块: FastCAE IO、HDF5IO、OperatorsModel | 需求编号: FR-012 | 设计章节: plan.md Module Boundaries; spec.md FR-012 | 输入: 冻结格式清单和文件 | 输出: 统一 MeshData 或导出文件 | 验收条件: 支持格式可读写验证；错误不破坏内存；大文件后台执行
- [ ] T032 编写 HDF5 版本检查、插件读写分发和失败处理测试 `tests/integration/io/` | 前置任务: T029-T031 | 影响模块: HDF5IO、OperatorsModel | 需求编号: FR-013, FR-014, FR-020, SC-005 | 设计章节: plan.md Testing Strategy; quickstart.md | 输入: 有效、损坏、旧版本、缺插件文件 | 输出: IO 集成测试 | 验收条件: `Version` 校验正确；已加载插件读写被调用；失败不清空当前内存数据

## Phase 9: Python、HTTP 和 AI 扩展

- [ ] T033 实现 `PyRegister` 和现有 Python 业务包装注册 | 前置任务: T014, T015, T029 | 影响模块: PythonInterface、OperatorsModel | 需求编号: FR-017, FR-017 | 设计章节: contracts/extension-interface.md | 输入: FastCAE Python 注册接口、APPMesh 业务对象 | 输出: 已注册 Python 包装 | 验收条件: 注册对象可调用并遵守现有操作器和数据约束；不引入稳定公共 Schema
- [ ] T034 接入 FastCAE HTTP/Python 驱动及现有服务模式 | 前置任务: T033 | 影响模块: PythonInterface、HTTP | 需求编号: FR-018, FR-018 | 设计章节: plan.md Core Interfaces; contracts/extension-interface.md | 输入: 服务模式配置和驱动接口 | 输出: 可用的 HTTP/Python 接入 | 验收条件: 启用方式和行为与现有驱动一致；不承诺统一请求字段或稳定错误码
- [ ] T035 实现 AI Dock/资源初始化、allowlist、权限和失败隔离 `src/ai/` | 前置任务: T010, T033 | 影响模块: AI、GUIFrame、PythonInterface | 需求编号: FR-019, FR-019 | 设计章节: plan.md FastCAE Component Usage; contracts/extension-interface.md | 输入: AI 配置、资源、工程上下文 | 输出: AI 请求适配和错误 | 验收条件: 仅允许 read/query/import/generate/export/save；禁止原始文件/进程 API；资源失败不阻止核心启动
- [ ] T036 [P] [S9] 编写 Python/HTTP/AI 成功、失败、超时、权限和不可序列化结果测试 `tests/contract/extensions/` | 前置任务: T033-T035 | 影响模块: PythonInterface、HTTP、AI | 需求编号: FR-017, FR-018, FR-019, SC-007 | 设计章节: plan.md Testing Strategy | 输入: 合法/非法请求、不可达 AI 服务 | 输出: 扩展契约测试 | 验收条件: 成功/失败可观察，ErrorInfo 具有关联任务，不依赖未来稳定 schema

## Phase 10: 集成测试、部署和验收

- [ ] T037 建立全链路集成测试 `tests/integration/e2e/`，覆盖启动→导入→生成→显示→导出→保存→重开 | 前置任务: T004, T017, T021, T028, T032, T036 | 影响模块: 全部核心模块 | 需求编号: FR-001, FR-002, FR-003, FR-004, FR-005, FR-006, FR-007, FR-008, FR-009, FR-010, FR-011, FR-012, FR-013, FR-014, FR-015, FR-016, FR-017, FR-018, FR-019, FR-020, FR-021, SC-008 | 设计章节: quickstart.md | 输入: 标准几何/网格/插件夹具 | 输出: E2E 报告和日志 | 验收条件: 主流程完成并记录结果；失败/受控退出保留最后有效数据
- [ ] T038 实现部署打包和运行目录验证 `tests/deployment/`、`deploy/` | 前置任务: T001, T004, T022, T034, T035 | 影响模块: 构建/部署、插件、HTTP、AI | 需求编号: FR-003, FR-016, FR-018, SC-001 | 设计章节: plan.md FastCAE Component Usage | 输入: Qt/VTK/FastCAE DLL、插件、外部程序、资源 | 输出: Debug/Release 部署包 | 验收条件: 依赖可发现；缺失依赖有明确提示
- [ ] T039 [P] [S10] 进行性能、资源和大模型基线测试 `tests/performance/`，记录格式、模型规模和观测结果 | 前置任务: T031, T037 | 影响模块: 异步任务、GraphData、HDF5IO | 需求编号: FR-014, FR-015, SC-002 | 设计章节: plan.md Technical Context/Testing Strategy | 输入: 测试格式、节点/单元数、文件大小、启动/导入/保存/生成观测数据 | 输出: 性能基线、内存报告和未决指标清单 | 验收条件: UI 保持可交互；模型规模和性能阈值仅记录为后续产品决策，不作为当前实现前置条件
- [ ] T040 建立需求-设计-任务-测试追踪矩阵并执行最终验收 `docs/traceability/APPMesh-traceability.md` | 前置任务: T037-T039 | 影响模块: 质量、文档 | 需求编号: FR-021, SC-006, SC-007 | 设计章节: plan.md Constitution Check/Testing Strategy | 输入: spec/plan/tasks 和测试报告 | 输出: 追踪矩阵、已知限制和发布结论 | 验收条件: 每条功能需求关联设计、任务和测试；明确取消、稳定接口、未知数据、物理/后处理边界

## Dependencies and Execution Order

`Phase 1 -> Phase 2 -> Phase 3 -> Phase 4 -> Phase 5 -> Phase 6 -> Phase 7 -> Phase 8 -> Phase 9 -> Phase 10`。阶段内按任务前置条件执行；标记 `[P]` 的任务在其前置任务完成后可并行。

## Parallel Execution Examples

- T005、T006、T007 的后续建模测试可分工；T010、T011 可在数据层完成后并行。
- T020 与 T021 在 T019 定义适配器契约后并行。
- T031 与 HDF5 测试夹具可在 T029 后并行；T034 与 T035 在 T033 后并行。

## Implementation Strategy

MVP 为 Phase 1-4 加 Phase 5 的一个生成器和 T021 对应子集，先验证“导入几何→生成网格→显示”。随后按 Phase 6-10 增加插件隔离、HDF5/文件 IO、扩展入口、AI 和发布验收。大模型性能阈值在 T039 中记录为后续产品决策，不作为当前实现前置条件。

## Notes

当前版本不提供用户取消任务，任务接口保留未来扩展点；Python/HTTP 不承诺稳定公共 schema/错误码；工程恢复仅在类型、版本和插件匹配时尽力完成，未知插件原始数据及完整运行/显示状态不属于当前验收承诺。

## Supplemental Tasks from Specification Analysis

- [ ] T041 [P] [S1] 固化 FastCAE 组件映射和唯一初始化顺序，覆盖 `FITKGlobalData`、`FITKComponentManager`、`FITKPluginsManager`、`FITKOperatorRepo`、`FITKRenderWindowVTK`、`FITKAdaptor`、`FITKGeoCommandList`、线程池、HDF5/IO 和 Python 桥接 | 前置任务: T001-T004 | 影响模块: MeshApp、GlobalDataFactory、ComponentFactory、GraphData、OperatorsInterface | 需求编号: FR-001, FR-002, FR-003, FR-021 | 设计章节: plan.md Frozen Scope and Traceability Amendment | 输入: FastCAE 组件清单、启动日志 | 输出: 组件映射表、初始化顺序测试 | 验收条件: 测试日志严格验证 FITKApplication→SystemChecker/Settings→GlobalDataFactory/ModelData→ComponentFactory→Python→主窗口/GraphData→插件→AppInitializer/Operators→命令行/Workbench→事件循环，失败时不进入事件循环
- [ ] T042 [S6] 覆盖 FastCAE 插件加载、注册、插件工程读写和卸载流程 | 前置任务: T016-T018、T033-T035 | 影响模块: FITK_Plugins、OperatorsModel、HDF5IO | 需求编号: FR-008, FR-013, FR-016, FR-021 | 设计章节: plan.md Plugin Protocol、Project File Structure | 输入: 实际插件接口、运行任务、插件数据 | 输出: 生命周期和读写诊断 | 验收条件: 不兼容插件不加载；已加载插件可注册能力并参与读写；失败不破坏核心应用；首版不增加统一取消接口
- [ ] T043 [S10] 建立首版格式和性能验收配置及全链路需求-设计-任务-测试追踪表 | 前置任务: T031、T036-T040 | 影响模块: HDF5IO、PythonInterface、部署和验收 | 需求编号: FR-006, FR-012, FR-020, FR-021, FR-022, SC-001..SC-008 | 设计章节: plan.md Testing Strategy、Frozen Scope and Traceability Amendment | 输入: BRep/STEP/STP/IGES/IGS、FITKMesh/CGNS/INP 夹具、性能阈值决策 | 输出: AT-01..AT-12、验收报告、剩余问题清单 | 验收条件: 所有 FR/SC 至少映射到设计模块、任务和测试；格式夹具覆盖冻结清单；Python/HTTP 不测试未承诺的稳定 schema/错误码；性能阈值未冻结时明确标记为未决而不伪造通过

## Requirement-to-Test Index

| 验收测试 | 覆盖任务 | 覆盖需求 |
|---|---|---|
| AT-01 启动与受控退出 | T001-T004,T041 | FR-001,FR-002,FR-003,FR-004 |
| AT-02 工厂/组件失败隔离 | T003,T041 | FR-001,FR-002,FR-003,FR-016 |
| AT-03 几何导入格式 | T005-T010 | FR-005,FR-006 |
| AT-04 网格生成参数与结果校验 | T011-T018,T042 | FR-007,FR-008,FR-009,FR-010 |
| AT-05 GUI/GraphData 点选框选 | T019-T024 | FR-011 |
| AT-06 插件安装卸载 | T033-T035,T042 | FR-008,FR-016 |
| AT-07 网格 IO 格式 | T025-T028 | FR-012 |
| AT-08 HDF5 版本检查/插件读写与尽力恢复 | T029-T032,T042 | FR-013,FR-014,FR-020 |
| AT-09 Python/HTTP 受控调用 | T036-T037 | FR-017,FR-018 |
| AT-10 AI 边界与失败隔离 | T038 | FR-019 |
| AT-11 线程、异常和数据安全 | T039-T040,T043 | FR-015,FR-020,FR-021 |
| AT-12 部署和端到端验收 | T039-T043 | FR-022,SC-001..SC-008 |




