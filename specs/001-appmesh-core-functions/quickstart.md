# Quickstart Validation

## Verified prerequisites

The first-release Windows x64 baseline is:

- Visual Studio 2017 with the MSVC 14.16.27023/v141 x64 toolset.
- Windows 10 SDK 10.0.17763.0, or a deliberately approved compatible SDK after a clean configure/build test. The 2026-09-15 host check found no Windows 10 SDK installation, so this is the remaining environment blocker.
- CMake 3.16 or newer. Project files must not depend on behavior unique to the currently installed CMake 4.1.0-rc1.
- Qt 5.14.2 `msvc2017_64`. Set `QT_ROOT` to its installation directory; the example below falls back to the historical FastCAE location when the variable is absent.
- FastCAE/FITK sources at `dependencies/FastCAECodeBase`.
- The restored dependency lock at `dependencies/FastCAECodeBase/Tools`.

The Tools package has been checked for OCC 7.4.0 beta, VTK 9.4.2, HDF5 1.14.0, CGNS 4.2.0, Gmsh 4.5.4, SARibbon 2.0.1, Python 3.7.0/PythonQt and Qwt 6.2.0. Qwt is optional. Python/PythonQt is required only when the extension phase is enabled.

## Dependency preflight

Run these commands from the repository root in PowerShell:

```powershell
$repoRoot = (Resolve-Path '.').Path
$toolsDir = Join-Path $repoRoot 'dependencies\FastCAECodeBase\Tools'
$qtDir = if ($env:QT_ROOT) { $env:QT_ROOT } else { 'C:\Qt\Qt5.14.2\5.14.2\msvc2017_64' }

Test-Path "$toolsDir\cmake\OCCConfig.cmake"
Test-Path "$toolsDir\Win64\VTK942\bin\vtkCommonCore-9.4.dll"
Test-Path "$toolsDir\Win64\OCC\bin\TKernel.dll"
Test-Path "$toolsDir\Win64\hdf5\bin\hdf5.dll"
Test-Path "$toolsDir\Win64\SARibbon\bin\SARibbonBar.dll"
Test-Path "$qtDir\bin\qmake.exe"

& "$toolsDir\Win64\gmsh\gmsh.exe" -version
& "$toolsDir\Win64\Python37\python.exe" --version
& "$qtDir\bin\qmake.exe" -query QT_VERSION
```

Expected results are six `True` values followed by Gmsh `4.5.4`, Python `3.7.0`, and Qt `5.14.2`. Also verify that a Windows SDK is installed before configuring:

```powershell
$sdkIncludeRoot = Join-Path ${env:ProgramFiles(x86)} 'Windows Kits\10\Include'
Get-ChildItem $sdkIncludeRoot -Directory
```

The command must list at least one SDK version. Prefer `10.0.17763.0` because that is the version used by the FastCAE build scripts.

## Configure, build and test

After T001 creates the root `CMakeLists.txt`, configure without relying on machine-wide dependency discovery:

```powershell
$repoRoot = (Resolve-Path '.').Path
$toolsDir = (Join-Path $repoRoot 'dependencies\FastCAECodeBase\Tools').Replace('\', '/')
$qtRoot = if ($env:QT_ROOT) { $env:QT_ROOT } else { 'C:\Qt\Qt5.14.2\5.14.2\msvc2017_64' }
$qt5Dir = (Join-Path $qtRoot 'lib\cmake\Qt5').Replace('\', '/')
$buildDir = Join-Path $repoRoot 'build\vs2017-x64'

cmake -S $repoRoot -B $buildDir `
  -G "Visual Studio 15 2017" -A x64 `
  "-DTOOLS_DIR=$toolsDir" `
  "-DQt5_DIR=$qt5Dir"

cmake --build $buildDir --config Debug
ctest --test-dir $buildDir -C Debug --output-on-failure
```

Repeat with `Release` before packaging. Debug must load DLLs from the Tools `bind` directories and link libraries from `libd`; Release must use `bin` and `lib`. A mixed configuration is a failed validation even if the executable starts.

Expected first-stage result: the APPMesh desktop executable starts, completes FastCAE environment/component checks, creates the SARibbon main window, VTK view, model tree and message console, and enters the Qt event loop without missing-DLL diagnostics.

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
