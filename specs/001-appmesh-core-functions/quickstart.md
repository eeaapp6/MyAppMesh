# Quickstart Validation

## Verified prerequisites

The supported Windows x64 Debug baseline is:

- Visual Studio 2017 with the MSVC 14.16.27023/v141 x64 toolset.
- Windows 10 SDK 10.0.17763.0. On the validated host it is installed under `C:\Windows Kits\10` and is selected successfully by the VS2017 environment script.
- CMake 3.16 or newer. Project files must not depend on behavior unique to the currently installed CMake 4.1.0-rc3.
- Qt 5.14.2 `msvc2017_64`. Set `QT_ROOT` to its installation directory; the example below falls back to the historical FastCAE location when the variable is absent.
- FastCAE/FITK sources at `dependencies/FastCAECodeBase`.
- The restored dependency lock at `dependencies/FastCAECodeBase/Tools`.

The Tools package has been checked for OCC 7.4.0 beta, VTK 9.4.2, HDF5 1.14.0, CGNS 4.2.0, Gmsh 4.5.4, SARibbon 2.0.1, Python 3.7.0/PythonQt and Qwt 6.2.0. Qwt is optional. Python/PythonQt is required only when the extension phase is enabled.

## Dependency preflight

Run these commands from the repository root in PowerShell:

```powershell
$vcvars = 'C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat'
cmd.exe /d /s /c ('call "' + $vcvars + '" x64 10.0.17763.0 && where cl && where rc')

$repoRoot = (Resolve-Path '.').Path
$toolsDir = Join-Path $repoRoot 'dependencies\FastCAECodeBase\Tools'
$qtDir = if ($env:QT_ROOT) { $env:QT_ROOT } else { 'C:\Qt\Qt5.14.2\5.14.2\msvc2017_64' }

Test-Path "$toolsDir\cmake\OCCConfig.cmake"
Test-Path "$toolsDir\Win64\VTK942\bind\vtkCommonCore-9.4d.dll"
Test-Path "$toolsDir\Win64\OCC\bind\TKernel.dll"
Test-Path "$toolsDir\Win64\hdf5\bind\hdf5_D.dll"
Test-Path "$toolsDir\Win64\SARibbon\bind\SARibbonBard.dll"
Test-Path "$qtDir\bin\qmake.exe"
Test-Path "$repoRoot\output\bin_d\FITKAppFramework.dll"
Test-Path "$repoRoot\output\bin_d\FITKAppFramework.lib"
Test-Path "$repoRoot\output\bin_d\FITKCore.dll"
Test-Path "$repoRoot\output\bin_d\FITKCore.lib"

& "$toolsDir\Win64\gmsh\gmsh.exe" -version
& "$toolsDir\Win64\Python37\python.exe" --version
& "$qtDir\bin\qmake.exe" -query QT_VERSION
```

Expected results are ten `True` values followed by Gmsh `4.5.4`, Python `3.7.0`, and Qt `5.14.2`. The copied `output/` directory is a local, ignored dependency source: only explicitly allowlisted FITK base artifacts may be used; original APPMesh business binaries in that directory must never be linked. Also verify that a Windows SDK is installed before configuring:

```powershell
$sdkCandidates = @(
  (Join-Path ${env:ProgramFiles(x86)} 'Windows Kits\10\Include\10.0.17763.0'),
  'C:\Windows Kits\10\Include\10.0.17763.0'
)
$sdkCandidates | Where-Object { Test-Path $_ }
```

The environment command must resolve `cl.exe` and `rc.exe`, and the candidate check must print the installed `10.0.17763.0` include directory.

## Configure, build and test

After T001 creates the root `CMakeLists.txt`, configure Debug without relying on machine-wide dependency discovery:

```powershell
$repoRoot = (Resolve-Path '.').Path
$toolsDir = (Join-Path $repoRoot 'dependencies\FastCAECodeBase\Tools').Replace('\', '/')
$fitkOutputDir = (Join-Path $repoRoot 'output').Replace('\', '/')
$qtRoot = if ($env:QT_ROOT) { $env:QT_ROOT } else { 'C:\Qt\Qt5.14.2\5.14.2\msvc2017_64' }
$qt5Dir = (Join-Path $qtRoot 'lib\cmake\Qt5').Replace('\', '/')
$buildDir = Join-Path $repoRoot 'build\vs2017-x64-debug'

cmake -S $repoRoot -B $buildDir `
  -G "Visual Studio 15 2017" -A x64 `
  "-DCMAKE_SYSTEM_VERSION=10.0.17763.0" `
  "-DCMAKE_CONFIGURATION_TYPES=Debug" `
  "-DTOOLS_DIR=$toolsDir" `
  "-DFITK_OUTPUT_DIR=$fitkOutputDir" `
  "-DQt5_DIR=$qt5Dir"

cmake --build $buildDir --config Debug
ctest --test-dir $buildDir -C Debug --output-on-failure
```

Debug must load DLLs from the Tools `bind` directories and link libraries from `libd`. Non-Debug configurations are unsupported and must not be built, tested or packaged.

The supported one-command validation entry point is `cmd.exe /d /c scripts\build-vs2017.cmd`. It uses the same clean Debug-only directory, runs all Debug tests, and then requires `cmake --build build\vs2017-x64-debug --config Release` to fail.

Expected first-stage result: the APPMesh desktop executable starts, completes FastCAE environment/component checks, creates the SARibbon main window, VTK view, model tree and message console, and enters the Qt event loop without missing-DLL diagnostics.

## T002 settings recovery check

Run the `t002.settings` test in Debug. It verifies that a valid working directory is persisted and restored even when recent-file history contains missing, deleted, non-regular or duplicate paths. Invalid history entries are filtered, valid entries retain their order, Windows path comparison is case-insensitive, and the configured recent-file limit still applies. Unsupported schema versions, invalid working directories, invalid settings paths and actual read/write failures must continue to fail with structured diagnostics.

## T004 startup and failure isolation check

Run `ctest --test-dir build/vs2017-x64-debug -C Debug -R "t001.bootstrap|t004.mesh-app-lifecycle" --output-on-failure`. The T004 suite strictly checks command parsing, settings load, environment validation, global data, components, Python, minimal window, pre-window/signals, plugins, app initialization, operator boundary, command processing and event-loop entry. It injects failures at the environment, global-data, component, Python, window, pre-window and app-initializer stages and verifies that the event loop is not entered, only completed stages roll back, the original diagnostic remains first, rollback errors are appended, settings are saved through `MeshAPPSettings`, and repeated shutdown is idempotent.

`APPMesh.exe --smoke-test` now executes the same composition root with a non-blocking event-loop adapter, creates the minimal window offscreen, performs controlled shutdown and then reports the Qt/HDF5 runtime versions. The minimal window is an orchestration fixture owned by T004; it is not the T010 Ribbon UI.

## T041 real FITK base validation

T041 uses only the audited Debug `FITKCore` and `FITKAppFramework` library/DLL pairs from `output/bin_d`. The production process constructs `FITKApplication`; the local registration adapter is test-only. Concrete ModelData, FITKRenderWindowVTK/FITKAdaptor GraphData, FITKGeoCommandList operations, HDF5/IO adapters, Python wrappers, formal GUI and plugins remain later-task boundaries and are not loaded from the copied original APPMesh output.

Run the focused lifecycle and dependency suite:

```powershell
ctest --test-dir build\vs2017-x64-debug -C Debug -R "t041|fitk|t004" --output-on-failure

$env:QT_QPA_PLATFORM = 'offscreen'
$env:Path = "C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\bin;$PWD\build\vs2017-x64-debug\runtime\Debug;$env:Path"
build\vs2017-x64-debug\runtime\Debug\APPMesh.exe --smoke-test
```

The validated result is 5/5 focused tests and a zero-exit smoke process. Its log begins with `fitk.application`, reaches the event loop only after all mapped stages, and then reports operators, initializer, plugins, pre-window/signals, window, Python, components, global data and FITK runtime shutdown in reverse dependency order. The final banner includes `adapter=FITKFastCAERegistrationAdapter`, `FITKAppFramework=1.0.7` and `FITKCore=1.0.4`.

Use the VS2017 `dumpbin /dependents` command on `APPMesh.exe`, `output\bin_d\FITKAppFramework.dll` and `output\bin_d\FITKCore.dll`. `APPMesh.exe` must contain `FITKAppFramework.dll`, `FITKCore.dll`, `hdf5_D.dll`, `Qt5Widgetsd.dll` and `Qt5Cored.dll`; the framework transitively requires `Qt5Guid.dll` and `Qt5Networkd.dll`. Release Qt/HDF5 names and original APPMesh business DLL names must be absent.

## UI responsiveness heartbeat

Use the same deterministic heartbeat assertion for geometry import, Gmsh generation, project open/save and large-file IO:

1. Use a controlled fixture that keeps the background worker active for at least 5 seconds.
2. Start a `QTimer` with a 100 ms interval on the UI thread before submitting the operation.
3. Record a monotonic timestamp in every timeout callback and collect at least 50 consecutive callbacks while the task is in `running` state.
4. Calculate each adjacent callback interval as `timestamp[n] - timestamp[n-1]`.
5. Pass only when the maximum adjacent interval is no greater than 500 ms, the task is observed in `running` at least once, and exactly one completion or failure terminal state is emitted.
6. Write the sample count, maximum interval, observed states and task ID into the test report. A visual impression that the window is responsive is not an acceptance result.

## End-to-end checks

1. Import valid BRep/STEP/IGES geometry through the OCC component: the background task succeeds, a `GeometryObject` appears, and `GraphData` refreshes the VTK view.
2. Submit the bundled Gmsh 4.5.4 generator with valid parameters: the task reports progress and commits a validated `MeshData`.
3. Trigger a missing Gmsh executable or malformed MSH output: diagnostics are shown and the previous valid mesh remains.
4. Validate the frozen FITKMesh import and FITKMesh/CGNS/INP export contracts. Treat FITKMesh/INP as APPMesh adapter work rather than assuming an absent FastCAE component supplies it.
5. Save to HDF5 1.14.0, reopen with matching project type/version and loaded plugins, and verify best-effort restoration.
6. Load and unload a sample plugin; verify registrations and UI resources are removed and the core application remains usable.
7. In the extension phase, invoke one Python/PythonQt and one HTTP operation through controlled operator dispatch; neither may bypass model validation.
8. Close during an active task; verify new submissions stop, in-flight work completes, settings are written, and resources release in reverse dependency order.

See [data-model.md](./data-model.md), [plugin-protocol.md](./contracts/plugin-protocol.md), [project-hdf5.md](./contracts/project-hdf5.md) and [extension-interface.md](./contracts/extension-interface.md) for validation invariants.
