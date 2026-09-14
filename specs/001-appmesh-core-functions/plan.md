# Implementation Plan: APPMesh Core Functions

**Branch**: `001-appmesh-core-functions` | **Date**: 2026-09-13 | **Spec**: [spec.md](./spec.md)

**Input**: `docs/APPMesh软件需求规格说明书.md`, `docs/APPMesh系统设计报告.md`, and the clarified feature specification.

## Summary

APPMesh will be rebuilt as a FastCAE/FITK desktop application while preserving the established assembly and business architecture: `MeshApp` assembles the application, `GlobalDataFactory` and `ComponentFactory` provide extension points, `ModelData` owns geometry/mesh state, GUI modules present and edit that state, operators execute business actions, `GraphData` adapts data to VTK views, `FITK_Plugins` supplies generators and extensions, `HDF5IO` persists projects, and `PythonInterface` exposes controlled automation. Long-running work follows the FastCAE thread/task mechanisms; data commit and error behavior follow the concrete operator and IO implementation.

## Technical Context

**Language/Version**: C++17 minimum; exact compiler baseline follows the FastCAE/FITK build. Qt5 and Python 3.x bridge are supplied by the platform.

**Primary Dependencies**: FastCAE/FITK application, global-data, component, plugin, operator, thread-pool, geometry/mesh/IO abstractions; VTK; SARibbon; HDF5; optional Gmsh, TetGen and FastCAE Grid drivers.

**Storage**: HDF5 project container; plugin namespaces for plugin-owned data; versioned application configuration and recent-file metadata.

**Testing**: CTest-driven C++ unit tests, plugin and extension contract tests, FastCAE/VTK integration tests, fixture-based HDF5 compatibility tests, and deployment smoke tests.

**Target Platform**: Qt desktop on the supported Windows build environment; optional headless/HTTP startup only when enabled by deployment.

**Project Type**: Desktop CAE application with dynamically loaded plugins and optional automation services.

**Performance Goals**: UI remains interactive during geometry import, mesh generation, project open/save and large-file IO; progress/state is observable; unchanged data does not cause repeated display-object rebuilds. Exact large-model thresholds remain a product acceptance decision.

**Constraints**: No original APPMesh source dependency; dependencies respect the four layers; UI objects are main-thread-only; failed operations preserve last known-good data; current version does not promise stable Python/HTTP schemas or public error codes and does not provide task cancellation.

**Scale/Scope**: One desktop process, multiple geometry/mesh objects, at least one deployed and usable mesh-generator plugin for first-release acceptance, HDF5 projects, and the geometry-to-mesh-save workflow. Other generators depend on plugin and deployment availability. Physics fields, solver orchestration and complete post-processing remain outside current scope.

## Constitution Check

*GATE: PASS before and after design.*

- **FastCAE/FITK Foundation**: PASS. Lifecycle, factories, global data, components, plugins, operators, thread pool, VTK and IO are reused through published interfaces.
- **Four-Layer Architecture**: PASS. Dependencies flow from FastCAE base to components to APPMesh extensions to business/UI; business modules do not call concrete external engines directly.
- **Plugin-Based Extensibility**: PASS. Generators, IO extensions and optional capabilities use descriptors, lifecycle, capability registration, dependency checks and failure isolation.
- **Responsive and Asynchronous Execution**: PASS. Required long-running operations are asynchronous and expose progress/completion/failure; the amended constitution makes user cancellation optional and deferred for this release.
- **Traceability and Data Safety**: PASS. Design areas map to requirements and tests; failed operations report errors without intentionally clearing valid in-memory data.
- **No source reuse**: PASS. Only requirements, design documentation and FastCAE/FITK public contracts are inputs.

## Project Structure

```text
specs/001-appmesh-core-functions/
├── plan.md
├── research.md
├── data-model.md
├── quickstart.md
└── contracts/
    ├── plugin-protocol.md
    ├── project-hdf5.md
    └── extension-interface.md

src/
├── app/MeshApp.{h,cpp}              # application assembly and run modes
├── app/MainWindowGenerator.{h,cpp}
├── app/MeshAPPSettings.{h,cpp}
├── app/CommandLineHandler.{h,cpp}
├── app/MeshAppWorkBenchHandler.{h,cpp}
├── app/AppInitializer.{h,cpp}
├── app/SystemChecker.{h,cpp}
├── app/PreWindowInitializer.{h,cpp}
├── app/SignalProcessor.{h,cpp}
├── app/GlobalDataFactory.{h,cpp}
├── app/ComponentFactory.{h,cpp}
├── model/ModelData/                 # GeometryManager, MeshManager, MeshData, MeshKernel
├── gui/GUIFrame/                    # main frame, ribbon, console, model-tree host
├── gui/GUIWidget/                   # tree models and reusable business widgets
├── gui/GUIDialog/                   # work directory, generator and project dialogs
├── operators/OperatorsInterface/    # operation contracts and task notifications
├── operators/OperatorsModel/        # validation and business execution
├── operators/OperatorsGUI/          # QAction/dialog orchestration
├── graph/GraphData/                 # VTK actors, selection and refresh adapters
├── plugins/FITK_Plugins/            # descriptors, lifecycle and generator adapters
├── io/HDF5IO/                       # project snapshots and plugin namespaces
└── python/PythonInterface/          # controlled Python/HTTP registration bridge

tests/
├── unit/                            # model, task, serialization and validation
├── contract/                        # plugin, HDF5 and extension contracts
├── integration/                     # FastCAE assembly, VTK and generator tests
├── fixtures/                        # geometry, mesh and project fixtures
└── deployment/                      # runtime directory and dependency smoke tests
```

**Structure Decision**: A single CMake desktop application is split by architectural ownership. Public contracts live beside their owning module; concrete generators and external programs remain behind `FITK_Plugins` adapters. Tests mirror the four layers and do not include original APPMesh source.

## Module Boundaries and Dependencies

| Module | Owns | May depend on | Must not depend on |
|---|---|---|---|
| `MeshApp` | startup, shutdown, registration and mode selection | FastCAE application; factories; `GUIFrame`; `PythonInterface` | concrete generator or widget internals |
| `GlobalDataFactory` | application-wide `ModelData` and runtime services | FastCAE global-data contracts; model interfaces | GUI implementation |
| `ComponentFactory` | creation/configuration of FastCAE and APPMesh components | FastCAE component manager; descriptors | direct external-process code |
| `ModelData` | geometry/mesh entities, IDs, associations, validation and lifecycle | FastCAE abstract data interfaces | Qt widgets, VTK actors, generator executables |
| `GUIFrame` | main window, ribbon, model-tree/console composition | `GUIWidget`, `GUIDialog`, operator GUI contracts | direct mutation of kernels |
| `GUIWidget`/`GUIDialog` | presentation and input collection | model query interfaces; `OperatorsGUI` | long-running work, direct HDF5 writes |
| `OperatorsInterface` | operation/task/error contracts | model and FastCAE task abstractions | concrete UI and engine APIs |
| `OperatorsModel` | validation, task submission and result notification | `ModelData`, `FITK_Plugins`, `HDF5IO`, thread pool | Qt controls |
| `OperatorsGUI` | action/dialog routing and notifications | GUI modules; operator interfaces | algorithm implementation |
| `GraphData` | data-to-VTK adaptation, selection and refresh | `ModelData`, VTK/FastCAE view components | persistence and generator process control |
| `FITK_Plugins` | discovery, compatibility, lifecycle and generator/IO adapters | FastCAE plugin manager; public APPMesh contracts | private cross-plugin coupling |
| `HDF5IO` | HDF5 context/version handling and plugin read/write dispatch | HDF5; FastCAE plugin IO contracts | UI state mutation |
| `PythonInterface` | controlled registration and automation dispatch | operator contracts; permissions/runtime context | bypassing operators or model validation |
| `MainWindowGenerator`/`GUIFrame` | main-window construction and top-level composition | FastCAE window contracts; GUI modules | model mutation and task execution |
| `MeshAPPSettings`/`SystemChecker` | versioned settings and environment validation | FastCAE settings; Qt/VTK/runtime probes | business data ownership |
| `CommandLineHandler`/`MeshAppWorkBenchHandler` | command-line and batch input/output routing | operator interfaces; task service | direct file parsing or UI mutation |
| `AppInitializer`/`PreWindowInitializer` | ordered component and view initialization | factories; component manager; `GraphData` | plugin-private implementation |
| `SignalProcessor` | cross-module event translation and queued UI notifications | FastCAE signal/event contracts | synchronous worker-to-widget calls |

Dependency direction is strictly `FastCAE base -> FastCAE components -> APPMesh extensions -> APPMesh business/UI`. Cross-module communication uses interfaces, events, data IDs and component lookup.

## Core Interfaces and Classes

- `MeshApp`: registers `GlobalDataFactory`, `ComponentFactory`, settings, command-line/workbench handlers, initializer and Python registrar; runs only after initialization succeeds.
- `GlobalDataFactory`: creates `ApplicationRuntime`, `GeometryManager`, `MeshManager`, task service and project context.
- `ComponentFactory`: creates named components such as VTK view, console, IO and plugin services, then initializes them in dependency order.
- `GeometryManager`: imports validated `GeometryObject`s, guarantees stable IDs and unique names, owns associations and visibility/selection state.
- `MeshManager`: registers `MeshDataCreator`s, creates/removes `MeshData`, validates kernels, and maintains geometry references.
- `MeshData`/`MeshKernel`: represent generator metadata, parameters, dimension, nodes, elements, sets, components and source geometry ID.
- `OperatorsInterface::IOperator`: separates GUI argument collection from professional execution and emits task/result/error events.
- `OperatorsModel::TaskService`: schedules immutable input snapshots, exposes observable execution, success and failure results, and commits only validated results. No cross-module task state machine is defined in the first release.
- `OperatorsGUI`: binds actions and dialogs to operators and updates UI on the main thread.
- `GraphData`: maps model entities to VTK/FastCAE actors, performs incremental refresh and translates picking results to stable object/entity IDs.
- `FITK_Plugins` uses the FastCAE plugin base, manager and concrete driver interfaces; no universal plugin metadata or parameter schema is required in the first release.
- `HDF5IO`: creates/checks the HDF5 context and Version metadata, then dispatches read/write to currently loaded plugins.
- `PythonInterface`/`PyRegister`: registers controlled operations and dispatches through the same operator/task path; stable external schema and public error codes are future-version work.

## Application Initialization Order

1. Parse command line and select desktop, workbench or optional HTTP mode.
2. Validate runtime environment, Qt/VTK platform, dependencies, memory and work directory.
3. Construct FastCAE application and settings/history services.
4. Register `GlobalDataFactory` and create `ApplicationRuntime`, `ModelData` managers and task service.
5. Register `ComponentFactory` and create core components (messages, IO, VTK view, console).
6. Register controlled `PythonInterface`/`PyRegister` bindings and initialize `SignalProcessor`.
7. Create the main window through `MainWindowGenerator`; initialize `GUIFrame`, `GUIWidget`, `GUIDialog`, `GraphData` and `PreWindowInitializer`.
8. Discover and validate plugins; load compatible plugins and register capabilities, creators and persistence hooks.
9. Register `OperatorsInterface`, `OperatorsModel`, `OperatorsGUI` routes.
10. Process command-line/workbench inputs only after required services and the main window exist.
11. Enter the Qt event loop. On exit, stop new submissions, finish in-flight tasks, persist configuration/history, unload plugins, destroy components, then release global data.

## Thread and Task Model

- UI thread: Qt widgets, model tree, VTK view mutations, task notifications and final data commit.
- FastCAE thread pool: geometry parsing, mesh generation, external-process driving, HDF5 open/save and large-file IO.
- External process worker: isolated working directory, streamed stdout/stderr, bounded log capture and exit-code translation.
- Inputs are immutable snapshots; workers write temporary result objects/files. `ModelData` accepts a result only after integrity and association validation.
- Progress is phase-aware and may be indeterminate (`-1`) when a driver cannot report it. Current release supports observation, failure and controlled shutdown, not user cancellation.
- Shutdown disables new work, waits for existing tasks, then releases dependent services in reverse initialization order.

## Plugin Protocol

Plugins use the creation entry point, lifecycle, compatibility checks and registration hooks defined by FastCAE. Plugin configuration, errors, capabilities and metadata follow the concrete FastCAE plugin interface. A universal descriptor, transactional rollback, complex unload interaction and cancellation field are not first-release requirements.

## Project File Structure

HDF5 is the container. The source-level contract is:

```text
/Version                  version and project_type attributes
/<plugin-defined groups>  plugin-owned project payload
```

Writes create an HDF5 context, write Version metadata, and invoke currently loaded plugins. Reads require project type/version compatibility and invoke currently loaded plugins. The current release does not promise a unified geometry/mesh/runtime snapshot, raw preservation of unknown plugin data, staged restore, or atomic replacement.

## Error Handling

Use `ErrorInfo { category, code, message, detail, recoverable, taskId, objectId, path }`. Categories include configuration, environment, input, validation, component, plugin, external-process, IO, project-compatibility, resource and extension. Internal codes are for diagnostics within a build but are not a public Python/HTTP compatibility promise in this release. User-facing messages are concise; logs include timestamp, thread, module, task/object IDs and sanitized paths. Failed imports, generation, reads and writes leave existing valid data untouched.

## Testing Strategy

1. Unit-test ID/name invariants, mesh topology/set validation, concrete parameter validation, task state behavior and error reporting.
2. Contract-test plugin discovery, API compatibility, capability registration, install rollback, unload cleanup and generator result validation.
3. Contract-test HDF5 context creation, Version type/version checks, plugin read/write dispatch and failure reporting.
4. Integration-test initialization order, FastCAE factories, VTK/GraphData synchronization, operator routing and UI-thread responsiveness.
5. Driver-test Gmsh, TetGen and FastCAE Grid with fake executables plus real smoke fixtures where deployed; cover missing executable, non-zero exit, malformed output and large output streams.
6. Extension-test Python and HTTP success/failure dispatch through operators, permission checks and non-serializable results; do not assert an uncommitted stable schema.
7. End-to-end test geometry-import -> mesh-generate -> display -> export -> HDF5-save and controlled shutdown/failure preservation.
8. Performance/deployment tests measure startup, memory, import/IO/generation responsiveness and fixture sizes; exact large-model acceptance thresholds are a product follow-up.

## FastCAE Component Usage

- Reuse FastCAE application lifecycle, global data, component/plugin managers, operator repository, thread pool, settings/history and message/log facilities.
- Reuse geometry, mesh, IO and VTK abstraction interfaces; implement APPMesh behavior as adapters and business operators.
- Use FastCAE VTK windows, view adapters, generic widgets, console and file-dialog infrastructure; `GUIFrame`, `GUIWidget` and `GUIDialog` supply APPMesh-specific composition and validation.
- Use FastCAE HDF5/IO primitives and let currently loaded plugins own their project data read/write behavior.
- Use FastCAE Python bridge and registration mechanisms; route calls to APPMesh operators rather than exposing mutable internals.
- Package Qt, VTK, SARibbon, FastCAE libraries, plugins, external generators and resources in separate Debug/Release runtime directories.

## Complexity Tracking

| Violation | Why Needed | Simpler Alternative Rejected Because |
|---|---|---|
| None | The constitution amendment explicitly makes cancellation optional for this release. | N/A |

Additional intentional scope decisions are no stable Python/HTTP public contract, best-effort plugin-owned project restoration, and no raw unknown-plugin payload guarantee. The format list is frozen; large-model thresholds remain a product follow-up.

## Frozen Scope and Traceability Amendment

The first-release geometry formats are BRep, STEP/STP, and IGES/IGS. Mesh import is FITKMesh; mesh export is FITKMesh, CGNS, and INP. Generator parameters follow concrete generator implementations. There is no unified user cancellation; task behavior follows the FastCAE task and driver interfaces. Project restore is best effort for matching project type/version and loaded plugins; unknown plugin payloads are not required to be preserved. Python and HTTP remain controlled internal entry points without a stable versioned schema or public error-code contract.

Canonical startup order is: `FITKApplication` and command-line mode; `SystemChecker` and settings; `GlobalDataFactory` and `ModelData`; `ComponentFactory` and core components; Python registration; `MainWindowGenerator` with `GUIFrame`/`GUIWidget`/`GUIDialog` and `GraphData`; compatible plugin discovery/load; `AppInitializer` finalization and operator registration; command-line/workbench dispatch; Qt event loop. Shutdown is the reverse dependency order after in-flight work finishes.

FastCAE mapping required by implementation and tests: `FITKGlobalData` -> `GlobalDataFactory`; `FITKComponentManager` -> `ComponentFactory`; `FITKPluginsManager` -> `FITK_Plugins`; `FITKOperatorRepo` -> `OperatorsInterface/OperatorsModel`; `FITKRenderWindowVTK` and `FITKAdaptor` -> `GraphData`; `FITKGeoCommandList` -> geometry import/edit operators; FastCAE thread pool -> `TaskService`; FastCAE HDF5/IO and Python bridge -> `HDF5IO` and `PythonInterface` adapters.

| 需求范围 | 设计模块 | 任务 | 验收 |
|---|---|---|---|
| FR-001 | lifecycle/factories | T001,T003,T004,T041 | AT-01 |
| FR-002 | architecture boundaries | T001,T003,T041 | AT-02 |
| FR-003 | SystemChecker/settings | T002,T004,T041 | AT-01,AT-02 |
| FR-004 | settings/workbench | T002,T012 | AT-01 |
| FR-005 | ModelData/GUI | T005,T006,T011 | AT-03,AT-05 |
| FR-006 | GeometryIO/operators | T015,T016,T017 | AT-03 |
| FR-007 | MeshData/MeshManager | T005,T007,T008,T009 | AT-04 |
| FR-008 | plugins/generator operators | T018,T020,T022 | AT-04,AT-06 |
| FR-009 | operators/task service | T014,T019 | AT-04,AT-11 |
| FR-010 | result validation | T007,T019 | AT-04 |
| FR-011 | GUI/GraphData | T010,T011,T026,T027 | AT-05 |
| FR-012 | FastCAE IO | T031 | AT-07 |
| FR-013 | HDF5IO/plugins | T029,T030 | AT-08 |
| FR-014 | HDF5IO/task service | T029,T031,T032 | AT-08,AT-11 |
| FR-015 | task service/signals | T014,T019,T039 | AT-11 |
| FR-016 | plugin manager | T022,T023,T024,T025 | AT-06 |
| FR-017 | PythonInterface | T033,T036 | AT-09 |
| FR-018 | HTTP adapter | T034,T036 | AT-09 |
| FR-019 | AI boundary | T035,T036 | AT-10 |
| FR-020 | diagnostics/ErrorInfo | T009,T019,T032,T040 | AT-11 |
| FR-021 | traceability/testing | T040,T043 | AT-11,AT-12 |
| FR-022 | build/deployment | T001,T038,T043 | AT-12 |


