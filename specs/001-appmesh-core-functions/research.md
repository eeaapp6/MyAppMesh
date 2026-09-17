# Research: APPMesh Core Plan

## Decision: Preserve the documented FastCAE/FITK architecture

**Rationale**: Both source documents require FastCAE lifecycle, global data, component and plugin managers, operators, thread pool, VTK and IO abstractions. The named APPMesh modules are therefore treated as architectural ownership boundaries, not optional reorganizations.

**Alternatives considered**: A monolithic application layer was rejected because it would violate the four-layer constitution and make generators, IO and UI inseparable.

## Decision: Use asynchronous snapshot-and-commit tasks

**Rationale**: Geometry import, mesh generation, large-file IO and HDF5 project operations must keep the UI responsive. Immutable inputs, temporary outputs and main-thread commit prevent partial data from entering `ModelData`.

**Alternatives considered**: Direct worker-thread mutation was rejected because Qt/VTK objects are main-thread-bound and failures could corrupt current project data.

## Decision: Pin the Windows/FastCAE ABI baseline

**Rationale**: The restored `Tools/Win64` package and FastCAE build scripts consistently target Visual Studio 2017/MSVC 14.16 v141, Qt 5.14.2 `msvc2017_64`, CMake 3.16-compatible projects and Debug runtime libraries. APPMesh will compile as C++17 and will pass `TOOLS_DIR` explicitly instead of depending on the hard-coded paths in legacy batch files. Debug is the only supported configuration.

**Alternatives considered**: Qt 6, MinGW, a newer MSVC ABI and independently upgraded third-party packages were rejected for the first release because they would require rebuilding and retesting the entire FastCAE/FITK dependency graph.

## Decision: Use the restored Tools package as the dependency lock

**Rationale**: Repository inspection resolves the concrete baseline to OCC 7.4.0 beta, VTK 9.4.2, HDF5 1.14.0, CGNS 4.2.0, Gmsh 4.5.4, SARibbon 2.0.1, Qwt 6.2.0 and Python 3.7.0 with PythonQt. Only the matching Debug libraries are selected. These bundled builds take precedence over system-wide or newly downloaded copies.

**Alternatives considered**: Selecting the latest upstream versions was rejected because binary compatibility with the pinned FastCAE sources and Qt build would be unknown.

## Decision: Use Gmsh as the first-release generator behind the plugin boundary

**Rationale**: The repository contains Gmsh 4.5.4, `FITKGmshExeDriver` 2.0.0 and `FITKGmshMshIO` 2.0.0. No equivalent ready source-and-runtime pair was found for TetGen or FastCAE Grid. The generator remains behind FastCAE mesh-generation and plugin interfaces so later engines do not change core business code.

**Alternatives considered**: Hard-coded generator selection was rejected because it requires core changes for every new engine.

## Decision: Use SARibbon for the desktop shell and keep Qwt optional

**Rationale**: The restored Tools package contains SARibbon 2.0.1 headers and matching Debug libraries, so the planned ribbon UI is now buildable. Qwt 6.2.0 is also available, but no first-release user story requires two-dimensional plotting.

**Alternatives considered**: A permanent plain `QMainWindow` shell was rejected now that the required ribbon dependency is present. Making Qwt a mandatory dependency was rejected because it expands deployment without satisfying a current requirement.

## Decision: Freeze the T010/T011 GUI seam at SARibbon plus QWidget injection

**Decision**: T010 owns a real `SARibbonMainWindow` from `Win64/SARibbon` (`include/SARibbon-2.0.1`, `libd/SARibbonBard.lib`, `bind/SARibbonBard.dll`), its File/Project, View and Mesh categories/panels, central replaceable viewport host, model-tree dock and console dock. It exposes non-owning injection calls for `QWidget` instances; after injection, normal Qt parent ownership controls destruction and replacement. T011 owns the model/view tree and console behavior only, never creates a main window or dock, and never stores `DataObject`, geometry, mesh, or FITK object pointers. Tree nodes contain value snapshots keyed by stable `ObjectId`.

**Business routing**: The tree refreshes from `ApplicationRuntime::snapshots()`. Generic mutations use `ApplicationRuntime`; Geometry mutations use `GeometryManager`; Mesh mutations use `MeshManager`. T011 therefore requires the same narrow rename, parent, visibility, and selection forwarding surface on `MeshManager` that already exists on `GeometryManager`. These methods validate type/payload ownership before delegating common fields to the runtime; they do not create a second state store.

**Thread and lifetime boundary**: GUI objects are created and destroyed on the GUI thread. ModelData publishes value events (`Added`, `Updated`, `Removed`, `Reset`) through a thread-safe RAII subscription after write locks are released. `ModelTree::unbind()` and destruction cancel the subscription, while Runtime shutdown invalidates it safely. Worker events are queued with `QMetaObject::invokeMethod`, `QPointer` and value-captured snapshots. Console calls from worker threads use the same GUI queue. T013 retains the five-second heartbeat test; T026 retains VTK/GraphData and picking.

**Ribbon boundary**: T010 links only the explicit Debug imported target `APPMesh::SARibbon`; it is separate from the unchanged FITK allowlist (`FITKCore`, `FITKAppFramework`). `SARibbonBard.dll` depends on `Qt5Widgetsd.dll`, `Qt5Guid.dll`, `Qt5Cored.dll`, `MSVCP140D.dll`, `VCRUNTIME140D.dll` and `ucrtbased.dll`. No Release SARibbon or Release Qt/HDF5 artifact is staged. The central widget remains a T026 VTK/GraphData injection point; T010 does not claim VTK rendering.

## Decision: T012 dialogs stop at validated requests

`WorkDirectoryDialog`, `GeneratorDialog` and `ProjectDialog` are GUI-thread-only QDialogs that return value-semantic request structures. They validate boundary input and retain structured diagnostics, but never create directories, read or write projects, invoke HDF5/Gmsh/plugins, start threads or mutate ModelData. Generator parameters remain a generic `QVariantMap` with non-empty unique keys; project Open/Save extension policy is deferred to HDF5IO because no extension is frozen in this increment.

## Decision: T014 defines a narrow APPMesh operator/task contract above FITKCore

**FITK audit**: The already allowlisted `FITKCore` exposes `FITKAbstractOperator` with mutable `QVariant` arguments and QObject signals, `FITKOperatorRepo` for keyed creation/ownership, `FITKThreadTask` for QRunnable progress/completion signals, and `FITKThreadPool` for execution and waiting. These are stable public registration and scheduling seams, but they do not define APPMesh immutable input snapshots, structured `ErrorInfo`, detached task queries or the required single-terminal event vocabulary.

**Decision**: `appmesh_operators_interface` is a Qt Core-only static contract library. `IOperator` accepts a const value input and returns `OperatorResult`; `Task` is a mutex-protected state holder that returns snapshots and per-task sequenced events but starts no thread and stores no observer. A future adapter in T015/T019 may register concrete APPMesh operators with `FITKOperatorRepo` and schedule execution through `FITKThreadTask`/`FITKThreadPool`. T014 does not inherit from FITK classes, access the repository singleton, implement TaskService, or expand the FITK DLL allowlist.

**Rationale**: This preserves FITK ownership of repository/thread mechanics while giving APPMesh a GUI-independent business contract that can be tested without QObject ownership or worker scheduling. It also prevents FITK pointers and mutable argument maps from becoming the public task query surface.

## Decision: HDF5 namespaces with best-effort plugin restoration

**Rationale**: HDF5 is required by the design report. Base paths remain stable while plugins own namespaced payloads. The clarified current scope only promises best-effort restoration when project type/version and required plugins match.

**Alternatives considered**: A fully unified schema and raw unknown-data preservation are deferred because they were explicitly excluded from the current release commitment.

## Decision: Controlled, non-stable Python/HTTP surface

**Rationale**: The clarification states that the current release does not promise stable schemas or public error codes. Calls still route through operators, permissions and task management so future versioning can be added without bypass paths.

**Alternatives considered**: Free-form direct model access was rejected for safety and traceability.

## Decision: Isolate the FastCAE registration ABI behind a lifecycle adapter

**Rationale**: The public FastCAE contracts expose `FITKApplication::regGlobalDataFactory`, `FITKAbstractGlobalDataFactory::createData`, `FITKApplication::regComponentsFactory`, and `FITKComponentFactory::create`, but the repository currently contains no linkable FITKAppFramework binaries. T003 therefore owns creation order, dependency validation, duplicate rejection, reverse-order rollback, and diagnostic results behind `IFastCAERegistrationAdapter`. The concrete adapter that invokes the public FITK application entry points is installed by the T004 startup composition root, where the real `FITKApplication` lifetime exists.

**Alternatives considered**: Compiling selected private FastCAE implementation files into APPMesh was rejected because it would duplicate framework ownership and create an incomplete, ABI-fragile subset. Pretending registration succeeded without a boundary was rejected because failures and ordering would not be testable.

## Decision: T004 uses an explicit local registration boundary until FITKAppFramework is linkable

**Verified public contracts**: The repository-pinned `FITK_Kernel/FITKAppFramework` headers identify FITKAppFramework 1.0.7 and expose `FITKApplication::checkSystem`, `regGlobalDataFactory`, `regComponentsFactory`, `regPythonRegister`, `regMainWindowGenerator`, `regAppInitalizer`, `getPluginsManager`, command-line/Workbench registration and signal processing. `FITKAbstractGlobalDataFactory::createData`, `FITKComponentFactory::create`, `FITKAbstractPythonRegister::registWapper`, `FITKAbstractMainwindowGenerator::genMainWindow`, `FITKAbstractSysChecker::check` and `FITKAbstractAppInitializer::init` are also present.

**Verified binary state at T004**: `dependencies/FastCAECodeBase/Tools` contains the third-party dependency lock but no `FITK*.lib` or `FITK*.dll`, and `dependencies/FastCAECodeBase/output` does not exist. T004 therefore could not safely instantiate or call `FITKApplication` at that time without inventing an ABI result or compiling a private framework subset. Before T041, the user supplied the ignored local `output/bin_d` binary set, enabling a separate ABI and dependency audit.

**T004 decision**: `MeshApp` composes T003 through `IFastCAERegistrationAdapter`. `LocalFastCAERegistrationAdapter` enforces real key ownership, duplicate prevention and unregister semantics while explicitly not claiming FITK registration. Python, plugin and operator stages use enabled/disabled lifecycle contracts: disabled means the capability is intentionally absent in this increment; enabled without a concrete action is a startup error. Automated tests verify the future FITK adapter's required call order, failure barrier and reverse rollback independently of a blocking event loop.

**Connection point**: T041 replaces the production local adapter with allowlisted Debug FITKAppFramework/FITKCore import libraries and runtime DLLs built for MSVC v141/Qt 5.14.2. `LocalFastCAERegistrationAdapter` remains test-only. Original APPMesh business binaries in the copied `output/` directory remain prohibited dependencies.

## Resolved planning unknowns

- Dependency package: `dependencies/FastCAECodeBase/Tools` is present and contains the required Win64 Debug libraries and CMake dependency modules.
- Build ABI: Visual Studio 2017/MSVC v141 x64 with Qt 5.14.2 `msvc2017_64`; C++17 at the APPMesh target level; CMake minimum 3.16.
- Host build baseline: FastCAE scripts target Windows SDK 10.0.17763.0. T001 verified it through the required VS2017 `vcvarsall.bat` invocation and completed the Debug build without storing a machine-specific installation root.
- First-release generator: Gmsh 4.5.4. TetGen and FastCAE Grid are future plugin candidates, not current dependencies.
- Runtime versions: OCC 7.4.0 beta, VTK 9.4.2, HDF5 1.14.0, CGNS 4.2.0, SARibbon 2.0.1, Python 3.7.0/PythonQt; Qwt 6.2.0 optional.
- Current-release cancellation: not implemented; tasks complete or fail and controlled shutdown is supported.
- Physics fields, solver orchestration and complete post-processing: outside current scope.
- First-release file-format list is frozen in `spec.md`. The existing OCC and CGNS components cover part of it; APPMesh must supply and test the FITKMesh/INP adapter work required by FR-012. Large-model thresholds remain a later product acceptance decision.
- Cancellation is a documented constitution exception for this release; the next release must either add cancellation or amend the constitution with owner and expiry.
- Internal diagnostic codes follow the design report's APP-/GEO-/MESH-/TASK-/IO-/PRJ-/PLG-/EXT- categories, but their external stability is deferred.

## T001 Debug-only revalidation

The clean `build/vs2017-x64-debug` validation passed with the VS2017 v141 x64 compiler, Windows SDK 10.0.17763.0 and Qt 5.14.2. The generated solution and cache expose only `Debug`; HDF5 is imported only from `libd/hdf5_D.lib` with `bind/hdf5_D.dll`, and runtime staging creates only the Debug layout. All eight Debug tests passed, including policy, runtime-layout, binary-dependency and T002-T004 regression coverage.

`cmake --build build\vs2017-x64-debug --config Release` returned exit code 1 because the generated project has no Release configuration. `dumpbin /dependents` confirmed `APPMesh.exe` depends on `hdf5_D.dll`, `Qt5Cored.dll` and `Qt5Widgetsd.dll`, with no `hdf5.dll`, `Qt5Core.dll` or `Qt5Widgets.dll`. No library from the ignored `output/` tree is part of T001; explicit FITK base-library integration remains T041.

## Decision: T041 uses the minimal audited Debug FITK base allowlist

All 28 `FITK*.dll` modules in `output/bin_d` have matching `.lib` files. Presence and pairing alone do not grant permission to link them. The T041 production allowlist is limited to the two libraries required by the public lifecycle API:

| Library | Debug files | Public source/header evidence | T041 use | Direct `dumpbin` dependencies | Decision |
|---|---|---|---|---|---|
| `FITKCore` 1.0.4 | `output/bin_d/FITKCore.lib`, `FITKCore.dll` | `FITK_Kernel/FITKCore/FITKCoreAPI.h`, `FITKThreadPool.h`, `FITKOperatorRepo.h` | Real thread pool and operator repository | `Qt5Widgetsd`, `Qt5Cored`, MSVC/UCRT Debug, Windows | Allow |
| `FITKAppFramework` 1.0.7 | `output/bin_d/FITKAppFramework.lib`, `FITKAppFramework.dll` | `FITK_Kernel/FITKAppFramework/FITKAppFramework.h`, factory/global/component/plugin headers | `FITKApplication`, global data, components, plugins, signals and public factory registration | `FITKCore`, `Qt5Widgetsd`, `Qt5Guid`, `Qt5Networkd`, `Qt5Cored`, MSVC/UCRT Debug, Windows | Allow |

No direct or transitive dependency of these two DLLs names an original APPMesh business DLL. `FITKAdaptor`, `FITKRenderWindowVTK`, `FITKInterfaceGeometry`/`FITKGeoCommandList`, `FITKPython`, IO, mesh, geometry, generator, AI, widget and solver/data modules are rejected from the T041 allowlist because their concrete behavior belongs to T006 and later tasks; T041 records their disabled connection points without loading them. Original `MeshApp.exe`, GraphData, GUIFrame/GUIWidget/GUIDialog, ModelData, Operators, GeometryIO and HDF5IO binaries are categorically prohibited.

`FITKApplication` owns factory pointers registered through its void APIs and deletes any still-registered factory during destruction. The production adapter therefore owns only APPMesh service bookkeeping. A small public-contract bridge is transferred to FITK while registered; on reverse shutdown the adapter first calls the same public registration API with `nullptr`, then deletes the detached bridge. APPMesh continues to own its `ManagedService` instances. This prevents overwrite leaks and double deletion, while duplicate keys are rejected before calling FITK.

The T041 baseline validation ran 12/12 CTest tests and 5/5 T004/T041 focused tests. After T005, the full Debug suite runs 16/16 tests while retaining the same FITK-focused coverage. The real non-blocking smoke process reports `FITKAppFramework=1.0.7`, `FITKCore=1.0.4`, the real adapter name and strict startup/reverse-shutdown logs. `APPMesh.exe` directly depends on `FITKAppFramework.dll`, `FITKCore.dll`, `hdf5_D.dll`, `Qt5Widgetsd.dll` and `Qt5Cored.dll`; it contains no Release Qt/HDF5 or original APPMesh business dependency.

## Decision: T005 uses a snapshot-based, thread-safe ApplicationRuntime

**Rationale**: `ApplicationRuntime` owns all common `DataObject` instances behind one `QReadWriteLock`. `ObjectId` is `quint64`, zero is invalid, allocation starts at one by default, increases monotonically, never reuses deleted IDs and reports exhaustion instead of wrapping. Public queries return `DataObjectSnapshot` values, so callers never retain a mutable reference or a pointer that can dangle after deletion.

Names are trimmed and indexed by `QString::toCaseFolded()` while preserving the chosen display case. Conflicts receive deterministic suffixes `Name (2)`, `Name (3)`, and so on. The runtime maintains ID, normalized-name and parent-to-children indexes as one source of truth; write operations update them under one write lock. Missing parents, self-parenting, cycles and removal of a parent with children are rejected without changing existing objects. The production composition root registers this source-built runtime through `GlobalDataFactory`; it does not link an original APPMesh ModelData binary or expand the FITK Debug allowlist.

**Alternatives considered**: Returning owned-object pointers was rejected because concurrent deletion would make their lifetime unsafe. Random IDs were rejected because they do not provide a deterministic no-collision guarantee. Case-sensitive names were rejected for the Windows target, and cascading parent deletion was deferred because no product rule currently authorizes removal of an entire subtree.

## Decision: Freeze T006-T008 as common records plus domain payloads

**Conflict found**: The earlier model text said that `GeometryManager` owns Geometry objects, names and display state, while T005 already made `ApplicationRuntime` the sole owner of `DataObject`, global IDs, unique names, parent-child indexes and common display fields. Inheriting Geometry or Mesh types from the now-`final` `DataObject` would also contradict the implemented T005 contract.

**Decision**: Domain models use composition and `ObjectId` association. `ApplicationRuntime` owns the common record; `GeometryManager` owns only an APPMesh geometry payload/adapter record with the same ID. `GeometryObjectSnapshot` combines detached common and geometry values. T007 `MeshData` and `MeshKernel` remain value objects; T008 later owns committed mesh payloads and Geometry-Mesh indexes. No manager returns runtime-owned or FITK-owned raw pointers to business callers.

**Rationale**: This retains one source of truth for ID/name/parent state, lets T006 and T007 work in separate source files, and preserves the snapshot safety proven by T005.

**Alternatives considered**: `GeometryObject : DataObject` was rejected because `DataObject` is final and runtime-owned. Giving each domain manager a second common record/index was rejected because rename, display and deletion could diverge. Making `ApplicationRuntime` own geometry payloads was rejected because it would turn the common catalog into a domain-specific manager.

## Decision: Use a narrow hidden registration protocol for domain objects

**Conflict found**: The current public one-step `createObject()` publishes a record before a separate `GeometryManager` can install its payload. Compensating deletion alone would leave a concurrently observable interval in which a Geometry common record exists without geometry data. The current public create/remove methods also let business code bypass the geometry lifecycle entry point.

**Decision**: T006 makes the minimum T005-compatible extension described in `data-model.md`: a manager-only reserve/publish/cancel/typed-remove capability. Reservations allocate IDs and names but remain outside public queries and indexes; publication occurs only after payload installation. Generic `createObject()` behavior remains unchanged, while generic create/remove reject Geometry and Mesh types. T007 depends only on the existing const snapshot query and does not edit this registration surface.

**Rationale**: The protocol enforces the postcondition without holding a geometry lock during a runtime call, preserves non-reused IDs on failure, and avoids introducing a general transaction coordinator. It also gives T008 the same narrow boundary for Mesh without moving its association work forward.

**Alternatives considered**: Holding both manager locks was rejected because it creates lock-order inversion risk. Creating then deleting on failure was rejected because readers could observe a half-initialized Geometry. A general multi-resource transaction framework was rejected as unnecessary for T006-T008.

## Decision: T007 validates geometryId through the common catalog only

**Decision**: A zero `geometryId` represents an unassociated mesh candidate. For a nonzero value, T007 calls `ApplicationRuntime::findById()` through a read-only reference and requires `DataObjectType::Geometry`. It does not include, call or lock `GeometryManager` and does not create Geometry-Mesh indexes.

**Rationale**: Existence and type are common-record facts already available from T005. This keeps T006 and T007 parallel and reserves association ownership, reverse lookup and delete constraints for T008.

**Alternatives considered**: Calling `GeometryManager` was rejected because it introduces an unnecessary T006 dependency and shared-file pressure. Treating every nonzero `ObjectId` as Geometry was rejected because a Generic or Mesh ID would pass incorrectly.

## Decision: APPMesh is the FITK business adapter/snapshot layer

**Source evidence**: FITK already supplies global `FITKAbstractDataObject` identity, thread-safe `FITKAbstractDataManager`, `FITKGeoModelManager`/geometry command and OCC shape abstractions, `FITKAbstractMesh`, node and element lists, unstructured topology and geometry-to-mesh mapping. Those are established owners of heavy geometry and mesh data.

**Decision**: FITK/OCC retains canonical committed BRep/topology, mesh nodes/elements/topology and component objects. APPMesh owns its separate business `ObjectId`, source/generator provenance, UI/business metadata, validation snapshots and stable mappings to FITK data/topology keys. T007's node/element/set containers are transient validation/interchange values; T008 adapts them into FITK rather than retaining a second long-lived heavy mesh graph. `GeometryObjectSnapshot`, `MeshData` and `MeshKernel` are detached APPMesh values and never expose FITK pointers.

**Rationale**: This follows Constitution I by adapting public FITK abstractions instead of reimplementing their storage/managers, while retaining deterministic validation and testability before a failed import or generator result can enter FITK-owned state.

**Alternatives considered**: A fully independent APPMesh geometry/mesh repository was rejected as duplicated ownership and synchronization risk. Exposing FITK manager pointers directly was rejected because their lifetime and thread guarantees do not satisfy the T005 snapshot contract. Using FITK's signed integer IDs as APPMesh-wide IDs was rejected because those IDs belong to the FITK object scope; adapters instead range-check APPMesh `quint64` entity IDs at conversion.

## Decision: Freeze only the minimal identifiers and validation vocabulary

**Decision**: Geometry entity, node, element and set IDs are nonzero `quint64` values scoped to their containing Geometry or `MeshKernel`. Geometry formats are limited to BRep/STEP/IGES, topology kinds to vertex/edge/face/solid, mesh dimension to D2/D3/Mixed, set targets to node/element, and cell types to Line2/Triangle3/Quadrilateral4/Tetrahedron4/Hexahedron8 with fixed arity. Generator key/version, common parameters, generator parameters and display metadata remain separate copied fields.

**Rationale**: These values cover the known T006-T008 requirements and frozen import/generator path without inventing a plugin schema or enumerating unsupported higher-order elements. Logical diagnostic paths identify the geometry entity, kernel ordinal, node, element or set using the existing `Common::Diagnostic` fields.

**Alternatives considered**: A comprehensive OCC/Gmsh/CGNS enum mirror was rejected because it would freeze unused third-party details. Implementing T014 `ErrorInfo` early was rejected; T006 and T007 continue to use `Common::OperationResult` and `Diagnostic`, whose codes remain internal identifiers.

