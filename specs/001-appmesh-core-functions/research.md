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
- Host build baseline: FastCAE scripts target Windows SDK 10.0.17763.0. T001 verified it under `C:\Windows Kits\10` through the required VS2017 `vcvarsall.bat` invocation and completed the Debug build.
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

No direct or transitive dependency of these two DLLs names an original APPMesh business DLL. `FITKAdaptor`, `FITKRenderWindowVTK`, `FITKInterfaceGeometry`/`FITKGeoCommandList`, `FITKPython`, IO, mesh, geometry, generator, AI, widget and solver/data modules are rejected from the T041 allowlist because their concrete behavior belongs to T005 and later tasks; T041 records their disabled connection points without loading them. `MeshApp.exe`, GraphData, GUIFrame/GUIWidget/GUIDialog, ModelData, Operators, GeometryIO and HDF5IO binaries are categorically prohibited.

`FITKApplication` owns factory pointers registered through its void APIs and deletes any still-registered factory during destruction. The production adapter therefore owns only APPMesh service bookkeeping. A small public-contract bridge is transferred to FITK while registered; on reverse shutdown the adapter first calls the same public registration API with `nullptr`, then deletes the detached bridge. APPMesh continues to own its `ManagedService` instances. This prevents overwrite leaks and double deletion, while duplicate keys are rejected before calling FITK.

The final Debug validation runs 12/12 CTest tests and 5/5 T004/T041 focused tests. The real non-blocking smoke process reports `FITKAppFramework=1.0.7`, `FITKCore=1.0.4`, the real adapter name and strict startup/reverse-shutdown logs. `APPMesh.exe` directly depends on `FITKAppFramework.dll`, `FITKCore.dll`, `hdf5_D.dll`, `Qt5Widgetsd.dll` and `Qt5Cored.dll`; it contains no Release Qt/HDF5 or original APPMesh business dependency. Concrete ModelData is still T005.

