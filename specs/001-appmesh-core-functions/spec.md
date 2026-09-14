# Feature Specification: APPMesh 基础功能

**Feature Branch**: `001-appmesh-core-functions`

**Created**: 2026-09-13

**Status**: Draft

**Input**: User description: 从需求规格说明书、系统设计报告和总体架构图生成 APPMesh 重开发项目基础功能规格，覆盖应用启动、工作目录、几何管理、网格管理、网格生成、三维显示、文件 IO、工程管理、插件管理、Python、HTTP 和 AI 扩展；复用 FastCAE 基座和组件，不依赖原始 APPMesh 源码。

## Clarifications

### Session 2026-09-13

- Q: 当前版本是否需要在规格中明确列出“必须支持”的几何和网格文件格式，而不是仅由 IO 组件或插件声明决定？ → A: 是；首版必须格式清单需在设计/计划阶段明确，额外格式通过插件扩展。
- Q: 网格生成器参数是否要求跨生成器统一 Schema？ → A: 否；参数由具体生成器或操作器自行定义和校验，首版不制定统一参数 Schema。
- Q: 网格生成任务取消时，系统是否必须支持“请求取消后等待生成器确认”，并在超过限定时间后将任务标记为失败或终止，同时保留已有有效网格？ → A: 否；当前版本不支持取消，任务提交后允许其完成。
- Q: 工程文件在保存和恢复时，当前版本是否必须完整保存几何、网格、对象关联、插件元数据、运行信息、工作目录和显示状态，并对未知插件数据采用“保留但不加载”的策略？ → A: 否；仅在项目类型和版本匹配且相关插件已加载时尽力恢复，工程数据主要由插件自行读写；未知数据完整保留与诊断列为后续增强。
- Q: 当前版本是否需要为 Python 和 HTTP 扩展统一定义版本化请求/响应 schema、稳定错误码，以及参数错误、权限错误、任务失败和超时等错误分类？ → A: 否；当前版本不承诺稳定接口，接口定义与稳定错误码留到后续版本。

## User Scenarios & Testing

### User Story 1 - 几何到网格工作流 (Priority: P1)

网格工程师启动桌面应用，选择工作目录，导入几何文件，检查三维模型，选择可用网格生成器并配置参数，异步生成网格，查看结果并保存工程。

**Why this priority**: 这是产品交付几何到网格成果的核心闭环。

**Independent Test**: 使用一份受支持的几何文件和一个可用生成器完成导入、生成、显示和工程保存，且界面在任务期间可操作。

**Acceptance Scenarios**:

1. **Given** 应用已启动且工作目录有效，**When** 用户导入受支持的几何文件，**Then** 系统异步读取文件，成功后创建几何对象并刷新模型树和三维视图。
2. **Given** 几何对象有效且生成器插件已加载，**When** 用户提交有效参数，**Then** 系统异步生成网格并显示进度、结果和关联关系。
3. **Given** 网格生成正在运行，**When** 任务执行失败或应用受控退出，**Then** 系统保留任务开始前已有工程数据，并显示可诊断状态；当前版本不提供用户取消任务。

### User Story 2 - 工程恢复与文件交换 (Priority: P1)

用户打开已有工程，恢复几何、网格、插件数据和显示状态，导入或导出网格文件，并将修改后的工程保存为可再次打开的文件。

**Why this priority**: 可恢复、可交换的工程数据是持续工作的基础。

**Independent Test**: 保存一个包含几何、网格和插件元数据的工程，重新打开并核对对象、关联、显示状态和版本信息。

**Acceptance Scenarios**:

1. **Given** 工程文件格式和版本受支持，**When** 用户打开工程，**Then** 系统异步恢复可用数据并报告缺失或不兼容内容。
2. **Given** 工程已有有效文件，**When** 保存操作失败，**Then** 当前工程数据不被清空；文件写入结果以实际 HDF5 IO 行为为准。
3. **Given** 网格文件格式受支持，**When** 用户导入或导出网格，**Then** 系统校验节点、单元、集合及关联信息，并报告错误。

### User Story 3 - 扩展与自动化 (Priority: P2)

开发者或高级用户安装插件，通过 Python、HTTP 或 AI 扩展提交任务、查询状态和读取结果，同时不破坏桌面工作流。

**Why this priority**: 扩展能力决定生成器、自动化和智能辅助的可持续接入方式。

**Independent Test**: 加载一个符合契约的插件，并分别通过 Python 和 HTTP 提交一个可观察的操作；不可用插件不得阻止核心应用启动。

**Acceptance Scenarios**:

1. **Given** 插件元数据、依赖和 API 版本有效，**When** 应用启动或用户加载插件，**Then** 插件被发现、初始化并注册其能力。
2. **Given** 插件加载失败或版本不兼容，**When** 系统处理加载结果，**Then** 核心应用保持可用并提供可诊断错误。
3. **Given** Python、HTTP 或 AI 请求参数有效，**When** 用户提交请求，**Then** 系统返回任务标识、状态和结果或结构化错误。

### User Story 4 - 三维检查与对象管理 (Priority: P2)

用户在三维视图和模型树中查看、选择、隐藏、显示、分组和删除几何及网格对象，并使用点选、框选和预选检查结果。

**Why this priority**: 可观察性和对象管理支撑质量检查与后续操作。

**Independent Test**: 创建或导入多个对象，执行可见性、选择、分组和删除操作，验证模型树、三维视图和全局数据保持一致。

**Acceptance Scenarios**:

1. **Given** 对象已加入工程，**When** 用户切换可见性或选择状态，**Then** 模型树和三维视图同步更新。
2. **Given** 对象名称冲突，**When** 用户新增或导入对象，**Then** 系统生成唯一合法名称并保留原对象。
3. **Given** 对象被其他网格或插件数据引用，**When** 用户请求删除，**Then** 系统提示影响并阻止产生悬空引用。

### Edge Cases

- 启动时缺少必需组件、运行环境、权限或磁盘空间。
- 工作目录不存在、不可写、路径包含无效字符或被外部删除。
- 几何/网格文件不存在、格式损坏、版本不支持、内容为空或超出可用内存。
- 导入、生成、打开或保存任务失败、重复提交或应用退出期间仍在运行。
- 生成器不可用、参数越界、外部程序退出异常或输出结果校验失败。
- 工程包含未知插件数据、缺失依赖、旧版本数据或部分损坏内容。
- Python/HTTP 请求超时、重复、权限不足、输入不完整或返回不可序列化结果。
- AI 服务不可达、响应超时、内容不完整或请求涉及未授权的工程数据。

## Requirements

### Functional Requirements

- **FR-001**: 系统 MUST 基于 FastCAE/FITK 启动流程，并复用其应用生命周期、全局数据、组件管理、插件管理、操作器机制、线程池和抽象接口。
- **FR-002**: 系统 MUST 保持 FastCAE 基座、FastCAE 组件、APPMesh 扩展、APPMesh 业务四层结构；业务模块 MUST 通过公开接口访问下层能力。
- **FR-003**: 系统 MUST 在启动时检查运行环境、配置、依赖组件和工作目录，并在失败时给出可诊断错误且不进入不完整的工作状态。
- **FR-004**: 用户 MUST 能查看、设置和持久化工作目录；文件对话框、命令行和扩展请求 MUST 遵循当前工作目录。
- **FR-005**: 系统 MUST 提供几何对象的导入、命名、查询、分组、显示/隐藏、选择、删除和与网格对象关联能力。
- **FR-006**: 几何导入 MUST 支持 BRep、STEP/STP、IGES/IGS，异步执行，成功后才提交完整对象；其他格式可由 IO 插件声明并扩展；失败不得留下不完整几何对象。
- **FR-007**: 系统 MUST 管理多个网格数据对象及其节点、单元、维度、集合、附加数据、生成器信息和源几何关联。
- **FR-008**: 网格生成器 MUST 通过 FastCAE 插件机制接入，并使用实际驱动提供的注册、配置和错误处理能力；参数校验由具体生成器或操作器负责，统一参数 Schema 和取消契约属于后续扩展。
- **FR-009**: 网格生成 MUST 接收经具体操作器校验的几何和生成参数，异步执行并报告执行结果；当前版本不要求统一任务状态机或用户取消已提交的生成任务。
- **FR-010**: 系统 MUST 在网格生成结果通过完整性校验后才加入网格管理；失败不得替换已有有效网格。
- **FR-011**: 三维显示 MUST 反映几何和网格对象的创建、删除、选择、可见性和刷新状态，并支持点选、框选和预选反馈。
- **FR-012**: 系统 MUST 通过文件 IO 能力导入 FITKMesh 网格，并导出 FITKMesh、CGNS、INP；几何输入遵循 FR-006，校验必要结构并报告可恢复错误；额外格式 MUST 由插件能力声明后提供。
- **FR-013**: 当前版本工程管理 MUST 在项目类型和版本匹配且相关插件已加载时，尽力通过各插件提供的读写能力恢复工程数据；不要求统一保存完整的几何、网格、对象关联、运行信息、工作目录或显示状态，也不要求原样保留未知插件数据。未知数据的完整保留与诊断能力列为后续增强项。
- **FR-014**: 工程打开和保存、大型文件读写 MUST 遵循 FastCAE 线程和 IO 机制执行；首版不额外承诺临时文件或原子替换。
- **FR-015**: 任何耗时任务 MUST 不阻塞界面线程；界面 MUST 能显示进度、完成和失败，并在任务结束后安全刷新界面；任务取消交互属于后续版本。
- **FR-016**: 插件管理 MUST 支持发现、依赖检查、加载、初始化、能力注册、禁用和卸载；插件失败不得破坏核心应用和已有工程数据。
- **FR-017**: Python 扩展在当前版本提供受控的业务操作入口，并遵守同一数据和线程约束；稳定参数 schema、版本化接口和稳定错误码属于后续版本。
- **FR-018**: HTTP 扩展在当前版本提供受控的任务提交、状态查询、结果获取和错误响应；请求不得绕过权限、数据校验和异步任务管理。稳定参数 schema、版本化接口和稳定错误码属于后续版本。
- **FR-019**: AI 扩展 MUST 通过明确的输入、输出、超时和错误边界访问工程能力；AI 不得直接修改核心数据而绕过业务操作和数据保护规则。取消能力属于后续版本。
- **FR-020**: 失败和退出时不得主动破坏仍在内存中的有效数据；诊断信息沿用 FastCAE 消息机制，具体错误字段由各模块实现。
- **FR-021**: 每项需求 MUST 能追踪到设计模块、实现任务和验证测试；涉及插件、线程、持久化或接口的变更 MUST 包含集成和失败测试；取消测试在引入取消能力后纳入。
- **FR-022**: 重开发实现 MUST NOT 读取、链接、复制或依赖原始 APPMesh 源码；规格、设计和测试 MUST 以当前需求、架构约束和 FastCAE/FITK 合同为依据。

### Key Entities

- **ApplicationRuntime**: 启动状态、配置、工作目录、组件、插件和任务服务的运行上下文。
- **GeometryObject**: 几何模型、拓扑、名称、显示状态、元数据和唯一标识。
- **MeshData**: 网格类型、生成器、参数、关联几何、一个或多个网格内核及显示状态。
- **MeshKernel**: 节点、单元、集合、维度和附加网格数据。
- **Task**: 操作类型、输入摘要、状态、进度、结果和错误信息；取消能力属于后续版本。
- **Plugin metadata**: 由 FastCAE 插件接口提供的标识、版本、依赖、能力和持久化信息。
- **Project**: 几何、网格、插件数据、运行信息、显示状态和格式版本的可恢复集合。
- **ExtensionRequest**: 来自 Python、HTTP 或 AI 的操作、参数、权限上下文和请求标识。

### Inputs and Outputs

- **Inputs**: 用户界面操作、命令行参数、几何/网格文件、工程文件、插件包、生成器参数，以及 Python/HTTP/AI 请求。
- **Outputs**: 几何和网格对象、三维视图更新、任务进度与状态、导出文件、可重新打开的工程文件、扩展响应和结构化错误。
- **Error output**: 错误 MUST 包含错误类别或代码、用户可理解消息、诊断详情、可恢复性和关联任务/对象标识；类别和代码的公开稳定性由后续接口版本定义。

## Success Criteria

### Measurable Outcomes

- **SC-001**: 在受支持环境中，应用能够完成源码对应的检查、注册、初始化并进入事件循环；启动失败提供 FastCAE 消息诊断。
- **SC-002**: 几何导入、网格生成、工程打开/保存和大型文件 IO 的 100% 验收测试中，界面线程保持可交互，任务状态可观察。
- **SC-003**: 源码中已有的网格生成插件能够按 FastCAE 插件机制注册和使用；新增生成器是否支持由具体插件和部署状态决定。
- **SC-004**: 100% 的失败和受控退出验收测试保留操作前的最后已知有效工程数据，且目标工程文件仍可打开。
- **SC-005**: 在项目类型和版本匹配且相关插件已加载的前提下，保存并重新打开工程时，各插件声明并实现的工程数据应完成尽力恢复；统一恢复全部几何、网格、关联、运行信息、工作目录、显示状态及未知数据保留不属于当前版本验收范围。
- **SC-006**: 100% 的核心用户场景可追踪到至少一条功能需求、一项设计决策、一个实现任务和一个自动化或手工验收测试。
- **SC-007**: Python、HTTP 和 AI 扩展各至少完成一次可观察的成功请求和失败场景验证；稳定接口 schema、错误码及取消场景不属于当前版本验收承诺。
- **SC-008**: 使用标准测试数据集时，“导入几何-生成网格-保存工程”主流程能够完成，并对成功、失败和数据保留结果留有验收记录。

## Assumptions

- 初始版本面向桌面 CAE 用户，支持无图形或服务模式属于扩展能力，不改变桌面核心数据合同。
- 首版必选格式已冻结为几何 BRep/STEP/STP/IGES/IGS、网格导入 FITKMesh、导出 FITKMesh/CGNS/INP，并纳入验收数据集；清单之外的格式由已发布 IO 组件或插件能力声明决定。
- 工程保存主要依赖各插件写入和读取自身数据；未知插件数据的完整保留和诊断能力属于后续增强项，具体序列化实现属于设计阶段决定的实现细节。
- 权限、认证和 AI 服务凭据由部署环境提供；本规格要求扩展遵守权限和数据边界，但不规定单一认证方案。
- 原始 APPMesh 源码不可作为设计、实现或测试输入；需求文档、设计报告、架构图和 FastCAE/FITK 公开合同是允许的依据。

## Frozen First-Release Decisions

1. Geometry formats: BRep, STEP/STP, and IGES/IGS.
2. Mesh import: FITKMesh. Mesh export: FITKMesh, CGNS, and INP.
3. Generator parameters follow the concrete generator and operator implementations; no universal first-release parameter Schema is required.
4. There is no unified user task cancellation in the first release; tasks may complete after submission. A future cancellation-capable plugin must declare that capability.
5. Project restore is best effort when project type/version match and required plugins are loaded. Unknown plugin data is skipped and diagnosed; raw preservation is not required.
6. Python and HTTP have no stable versioned schema or stable public error-code promise in this release.
7. Physics fields, solver integration, and complete post-processing are outside the current release scope.

## Traceability Matrix

| 需求编号 | 设计模块 | 实现任务 | 验收测试 |
|---|---|---|---|
| FR-001 | MeshApp、GlobalDataFactory | T001、T003、T004、T041 | AT-01 |
| FR-002 | 四层架构与组件边界 | T001、T003、T041 | AT-02 |
| FR-003 | SystemChecker、MeshAPPSettings | T002、T004、T041 | AT-01、AT-02 |
| FR-004 | MeshAPPSettings、CommandLineHandler、GUIDialog | T002、T012 | AT-01 |
| FR-005 | ModelData、GUIWidget、Operators | T005、T006、T011 | AT-03、AT-05 |
| FR-006 | GeometryIO、ImportGeometryOperator | T015、T016、T017 | AT-03 |
| FR-007 | MeshManager、MeshData、MeshKernel | T005、T007、T008、T009 | AT-04 |
| FR-008 | FITK_Plugins、Generator operators | T018、T020、T022 | AT-04、AT-06 |
| FR-009 | OperatorsInterface、TaskService | T014、T019 | AT-04、AT-11 |
| FR-010 | MeshGeneratorOperator、MeshManager | T007、T019 | AT-04 |
| FR-011 | GUIFrame、GraphData | T010、T011、T026、T027 | AT-05 |
| FR-012 | FastCAE IO、HDF5IO | T031 | AT-07 |
| FR-013 | HDF5IO、FITK_Plugins | T029、T030 | AT-08 |
| FR-014 | HDF5IO、TaskService | T029、T031、T032 | AT-08、AT-11 |
| FR-015 | TaskService、SignalProcessor | T014、T019、T039 | AT-11 |
| FR-016 | FITK_Plugins、PluginManager | T022、T023、T024、T025 | AT-06 |
| FR-017 | PythonInterface | T033、T036 | AT-09 |
| FR-018 | PythonInterface、HTTP adapter | T034、T036 | AT-09 |
| FR-019 | AI extension boundary | T035、T036 | AT-10 |
| FR-020 | ErrorInfo、diagnostics | T009、T019、T032、T040 | AT-11 |
| FR-021 | Traceability and test strategy | T040、T043 | AT-11、AT-12 |
| FR-022 | Build and deployment boundary | T001、T038、T043 | AT-12 |

