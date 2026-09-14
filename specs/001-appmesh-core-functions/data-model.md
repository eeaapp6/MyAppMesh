# APPMesh Data Model

## Common DataObject

`id` (stable unique integer), `name` (unique within manager), `type`, `visible`, `selected`, `parentId`, `createdAt`, and extension metadata. Objects enter a manager only after validation and are removed through the owning manager.

## GeometryObject

Contains geometry type, topology/entities, metadata and display state. `GeometryManager` owns objects and guarantees unique names. An import task stages the object and commits it only after parsing succeeds.

## MeshData and MeshKernel

`MeshData` contains generator key/version, common and generator-specific parameters, source `geometryId`, one or more kernels and display metadata. Each `MeshKernel` contains dimension (`D2`, `D3`, `Mixed`), nodes, elements, sets and components.

```text
Node    { id, x, y, z }
Element { id, cellType, nodeIds[] }
Set     { id, name, entityType, entityIds[] }
```

Validation requires unique node/element IDs, existing node references, valid cell arity, valid set references and an existing source geometry when an association is declared.

## Task

Fields: task ID, operation type, immutable input summary, observable execution result, phase/progress when provided by the driver, result reference, `ErrorInfo`, timestamps and diagnostics. The first release exposes only observable executing, success and failure results; no cross-module task state machine or cancellation transition is defined.

## Plugin Metadata

The FastCAE plugin mechanism supplies the plugin key and concrete lifecycle metadata. Additional fields depend on the plugin implementation; a universal descriptor schema is not required in the first release.

The first release exposes no unified cancellation operation.

## Project

`Project` is a business concept representing a project context and its possible geometry, mesh, plugin and runtime content. It is not a promise that all fields are persisted in the first release. The first release guarantees Version metadata and read/write dispatch to currently loaded plugins when project type/version match; it does not require a unified Geometry/Mesh/Runtime snapshot, atomic replacement or raw preservation of unknown plugin data.

## ExtensionRequest

Represents an operation received through an available Python/HTTP/AI wrapper. Exact fields and behavior follow the concrete wrapper; stable external schema is not promised.

## ErrorInfo

`category`, `code`, `message`, `detail`, `recoverable`, `taskId`, `objectId` and `path`. Codes are diagnostic identifiers in this release, not a promised stable external API.
