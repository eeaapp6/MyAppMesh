# Quickstart Validation

## Prerequisites

- Supported Qt5, VTK, SARibbon, FastCAE/FITK and HDF5 runtime.
- Built APPMesh executable and at least one compatible generator plugin.
- Test fixtures for a valid STEP/BREP file, a mesh file and a writable temporary directory.

## Build and run

```text
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Launch the desktop executable with the fixture work directory. Confirm startup checks, model tree, VTK view and console appear before command-line inputs are processed.

## End-to-end checks

1. Import valid geometry: task reports success, a `GeometryObject` appears, and `GraphData` refreshes.
2. Submit a generator with valid common and plugin-specific parameters: task reports phases/progress and commits a validated `MeshData`.
3. Trigger missing executable or malformed output: task reports failure, diagnostics are shown, and the previous valid mesh remains.
4. Export a supported mesh file and reopen it through the IO path.
5. Save to HDF5, reopen with matching project type/version and loaded plugin, and verify best-effort restoration. Test a missing plugin and confirm a compatibility diagnostic without clearing current data.
6. Load and unload a sample plugin; verify registrations and UI resources are removed and the core application remains usable.
7. Invoke one Python and one HTTP operation through the controlled dispatch path; verify observable success and failure without assuming stable public schemas.
8. Close during an active task; verify new submissions stop, the task is allowed to finish, configuration is written, and resources release in reverse order.

See [data-model.md](./data-model.md), [plugin-protocol.md](./contracts/plugin-protocol.md), [project-hdf5.md](./contracts/project-hdf5.md) and [extension-interface.md](./contracts/extension-interface.md) for validation invariants.

