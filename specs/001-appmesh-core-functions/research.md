# Research: APPMesh Core Plan

## Decision: Preserve the documented FastCAE/FITK architecture

**Rationale**: Both source documents require FastCAE lifecycle, global data, component and plugin managers, operators, thread pool, VTK and IO abstractions. The named APPMesh modules are therefore treated as architectural ownership boundaries, not optional reorganizations.

**Alternatives considered**: A monolithic application layer was rejected because it would violate the four-layer constitution and make generators, IO and UI inseparable.

## Decision: Use asynchronous snapshot-and-commit tasks

**Rationale**: Geometry import, mesh generation, large-file IO and HDF5 project operations must keep the UI responsive. Immutable inputs, temporary outputs and main-thread commit prevent partial data from entering `ModelData`.

**Alternatives considered**: Direct worker-thread mutation was rejected because Qt/VTK objects are main-thread-bound and failures could corrupt current project data.

## Decision: Use a descriptor-driven plugin protocol

**Rationale**: Gmsh, TetGen, FastCAE Grid and future IO/geometry capabilities are loaded through the FastCAE plugin mechanism. Concrete metadata, compatibility and registration behavior follow the framework and plugin implementations.

**Alternatives considered**: Hard-coded generator selection was rejected because it requires core changes for every new engine.

## Decision: HDF5 namespaces with best-effort plugin restoration

**Rationale**: HDF5 is required by the design report. Base paths remain stable while plugins own namespaced payloads. The clarified current scope only promises best-effort restoration when project type/version and required plugins match.

**Alternatives considered**: A fully unified schema and raw unknown-data preservation are deferred because they were explicitly excluded from the current release commitment.

## Decision: Controlled, non-stable Python/HTTP surface

**Rationale**: The clarification states that the current release does not promise stable schemas or public error codes. Calls still route through operators, permissions and task management so future versioning can be added without bypass paths.

**Alternatives considered**: Free-form direct model access was rejected for safety and traceability.

## Resolved planning unknowns

- Current-release cancellation: not implemented; tasks complete or fail and controlled shutdown is supported.
- Physics fields, solver orchestration and complete post-processing: outside current scope.
- Exact first-release file-format list and large-model thresholds: product/design decisions required before acceptance fixtures are frozen.
- Cancellation is a documented constitution exception for this release; the next release must either add cancellation or amend the constitution with owner and expiry.
- Internal diagnostic codes follow the design report's APP-/GEO-/MESH-/TASK-/IO-/PRJ-/PLG-/EXT- categories, but their external stability is deferred.

