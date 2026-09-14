# HDF5 Project Contract

```text
/Version                  version and project_type attributes
/<plugin-defined groups>  plugin-owned project payload
```

`Project` is a business concept, not a complete first-release persistence schema. The contract is limited to creation of the HDF5 context, a `Version` group containing `version` and `project_type=AppMesh`, and read/write dispatch to currently loaded plugins. A unified Geometry/Mesh/Runtime snapshot, atomic replacement, staged restore and raw preservation of unknown plugin data are not first-release requirements.
