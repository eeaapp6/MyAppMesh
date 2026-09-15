# FastCAE Tools External Dependency

`Tools/` is a required Windows development dependency bundle, but it is not stored in the MyAppMesh source repository. The local bundle inspected on 2026-09-15 contains 53,205 files and is approximately 7.81 GiB, so it is distributed as a separately versioned artifact.

## Expected location

After obtaining the approved bundle, extract it to:

```text
dependencies/FastCAECodeBase/Tools/
```

Do not rename the `Win64`, `cmake`, `Resources` or `QtResource5.14.2` directories. Existing FastCAE CMake modules resolve dependencies relative to this layout.

## Pinned first-release contents

| Dependency | Pinned bundle version | First-release role |
|---|---:|---|
| MSVC ABI | Visual Studio 2017 / v141 x64 | Binary compatibility baseline |
| Qt | 5.14.2 `msvc2017_64` | Desktop UI; installed separately |
| Open CASCADE | 7.4.0 beta | Geometry model and BRep/STEP/IGES IO |
| VTK | 9.4.2 (`VTK942`) | Three-dimensional rendering |
| Gmsh | 4.5.4 executable | First-release mesh generator |
| HDF5 | 1.14.0 | Project persistence |
| CGNS | 4.2.0 | Mesh exchange where supported |
| SARibbon | 2.0.1 | Desktop ribbon UI |
| Python | 3.7.0 plus PythonQt for Qt5/Python 3.7 | Extension phase |
| Qwt | 6.2.0 | Optional plotting, not core scope |

The bundle also contains dependencies used by other FastCAE applications. Do not replace individual libraries with newer system copies: all DLL/LIB files must remain compatible with Qt 5.14.2 and MSVC v141. `Python314`, `PythonQt361`, the older `VTK` directory and other unused packages are not selected by the MyAppMesh first-release plan, but they should only be removed when producing a separately tested minimal bundle.

## Maintainer delivery procedure

1. Package the complete approved `Tools/` directory as `FastCAE-Tools-Win64-v141-20260915.7z` or an equivalently versioned private artifact.
2. Keep third-party license files in the package.
3. Generate a checksum:

   ```powershell
   Get-FileHash '.\FastCAE-Tools-Win64-v141-20260915.7z' -Algorithm SHA256
   ```

4. Publish the archive and checksum through the team's private artifact storage, shared drive or another approved large-file channel.
5. Record a new artifact name and checksum whenever any bundled file changes.

Do not commit the archive or extracted `Tools/` directory to the ordinary source repository. Git LFS may be used only after the repository owner explicitly accepts the storage and clone cost.

## Receiver verification

From the MyAppMesh repository root, run:

```powershell
$repoRoot = (Resolve-Path '.').Path
$toolsDir = Join-Path $repoRoot 'dependencies\FastCAECodeBase\Tools'

Test-Path "$toolsDir\cmake\OCCConfig.cmake"
Test-Path "$toolsDir\Win64\VTK942\bin\vtkCommonCore-9.4.dll"
Test-Path "$toolsDir\Win64\OCC\bin\TKernel.dll"
Test-Path "$toolsDir\Win64\hdf5\bin\hdf5.dll"
Test-Path "$toolsDir\Win64\SARibbon\bin\SARibbonBar.dll"

& "$toolsDir\Win64\gmsh\gmsh.exe" -version
& "$toolsDir\Win64\Python37\python.exe" --version
```

Expected output is five `True` values, Gmsh `4.5.4`, and Python `3.7.0`. Also follow the SDK and Qt checks in [`specs/001-appmesh-core-functions/quickstart.md`](../../specs/001-appmesh-core-functions/quickstart.md).
