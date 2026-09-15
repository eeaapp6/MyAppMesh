# Research: APPMesh Core Plan

## Decision: Preserve the documented FastCAE/FITK architecture

**Rationale**: Both source documents require FastCAE lifecycle, global data, component and plugin managers, operators, thread pool, VTK and IO abstractions. The named APPMesh modules are therefore treated as architectural ownership boundaries, not optional reorganizations.

**Alternatives considered**: A monolithic application layer was rejected because it would violate the four-layer constitution and make generators, IO and UI inseparable.

## Decision: Use asynchronous snapshot-and-commit tasks

**Rationale**: Geometry import, mesh generation, large-file IO and HDF5 project operations must keep the UI responsive. Immutable inputs, temporary outputs and main-thread commit prevent partial data from entering `ModelData`.

**Alternatives considered**: Direct worker-thread mutation was rejected because Qt/VTK objects are main-thread-bound and failures could corrupt current project data.

## Decision: Pin the Windows/FastCAE ABI baseline

**Rationale**: The restored `Tools/Win64` package and FastCAE build scripts consistently target Visual Studio 2017/MSVC 14.16 v141, Qt 5.14.2 `msvc2017_64`, CMake 3.16-compatible projects and configuration-specific runtime libraries. APPMesh will compile as C++17 and will pass `TOOLS_DIR` explicitly instead of depending on the hard-coded paths in legacy batch files.

**Alternatives considered**: Qt 6, MinGW, a newer MSVC ABI and independently upgraded third-party packages were rejected for the first release because they would require rebuilding and retesting the entire FastCAE/FITK dependency graph.

## Decision: Use the restored Tools package as the dependency lock

**Rationale**: Repository inspection resolves the concrete baseline to OCC 7.4.0 beta, VTK 9.4.2, HDF5 1.14.0, CGNS 4.2.0, Gmsh 4.5.4, SARibbon 2.0.1, Qwt 6.2.0 and Python 3.7.0 with PythonQt. Release and debug libraries are both present. These bundled builds take precedence over system-wide or newly downloaded copies.

**Alternatives considered**: Selecting the latest upstream versions was rejected because binary compatibility with the pinned FastCAE sources and Qt build would be unknown.

## Decision: Use Gmsh as the first-release generator behind the plugin boundary

**Rationale**: The repository contains Gmsh 4.5.4, `FITKGmshExeDriver` 2.0.0 and `FITKGmshMshIO` 2.0.0. No equivalent ready source-and-runtime pair was found for TetGen or FastCAE Grid. The generator remains behind FastCAE mesh-generation and plugin interfaces so later engines do not change core business code.

**Alternatives considered**: Hard-coded generator selection was rejected because it requires core changes for every new engine.

## Decision: Use SARibbon for the desktop shell and keep Qwt optional

**Rationale**: The restored Tools package contains SARibbon 2.0.1 headers and matching release/debug libraries, so the planned ribbon UI is now buildable. Qwt 6.2.0 is also available, but no first-release user story requires two-dimensional plotting.

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

## Resolved planning unknowns

- Dependency package: `dependencies/FastCAECodeBase/Tools` is present and contains the required Win64 release/debug libraries and CMake dependency modules.
- Build ABI: Visual Studio 2017/MSVC v141 x64 with Qt 5.14.2 `msvc2017_64`; C++17 at the APPMesh target level; CMake minimum 3.16.
- Host build baseline: FastCAE scripts target Windows SDK 10.0.17763.0. T001 verified it under `C:\Windows Kits\10` through the required VS2017 `vcvarsall.bat` invocation and completed Debug/Release builds.
- First-release generator: Gmsh 4.5.4. TetGen and FastCAE Grid are future plugin candidates, not current dependencies.
- Runtime versions: OCC 7.4.0 beta, VTK 9.4.2, HDF5 1.14.0, CGNS 4.2.0, SARibbon 2.0.1, Python 3.7.0/PythonQt; Qwt 6.2.0 optional.
- Current-release cancellation: not implemented; tasks complete or fail and controlled shutdown is supported.
- Physics fields, solver orchestration and complete post-processing: outside current scope.
- First-release file-format list is frozen in `spec.md`. The existing OCC and CGNS components cover part of it; APPMesh must supply and test the FITKMesh/INP adapter work required by FR-012. Large-model thresholds remain a later product acceptance decision.
- Cancellation is a documented constitution exception for this release; the next release must either add cancellation or amend the constitution with owner and expiry.
- Internal diagnostic codes follow the design report's APP-/GEO-/MESH-/TASK-/IO-/PRJ-/PLG-/EXT- categories, but their external stability is deferred.

