# APPMesh Data Model

## T006-T008 Shared Contract Freeze

This section is the shared boundary for T006, T007 and T008. T006 and T007 may add their own implementation files in parallel, but neither task may redefine the identifiers, ownership rules, registration protocol, diagnostic rules or FITK boundary below.

### Ownership and extension boundary

- `ApplicationRuntime` is the sole owner of common `DataObject` records, global `ObjectId` allocation, unique-name, parent-child and common display/metadata indexes.
- `DataObject` remains `final`, non-copyable and non-movable. `GeometryObject`, `MeshData` and `MeshKernel` do not inherit from it.
- `GeometryManager` is the geometry business-lifecycle entry point. It owns APPMesh geometry payload/adapter records keyed by the common `ObjectId`; it does not own a second common record or allocate another `ObjectId`.
- A geometry payload and its common record have the same `ObjectId`. `GeometryObjectSnapshot` composes a copied `DataObjectSnapshot` with copied geometry fields. No API returns a pointer or reference to a `DataObject` owned by `ApplicationRuntime`.
- T007 implements `MeshData` and `MeshKernel` as independently validatable value objects. They have no common mesh `ObjectId` and are not committed business objects until T008 registers and owns them through `MeshManager`.
- T008 owns committed mesh payloads and the Geometry-Mesh forward/reverse association indexes. T006 and T007 must not create those indexes.

“`ApplicationRuntime` owns the common record” and “`GeometryManager` owns the geometry object” therefore describe different resources: the runtime owns identity and cross-domain fields, while the manager owns only the geometry payload and its FITK adapter association. Destruction of either resource is coordinated by `GeometryManager`; there is no shared or double ownership.

### Domain registration and rollback

The existing one-step public `ApplicationRuntime::createObject()`/`removeObject()` API is sufficient for generic objects but cannot, by itself, guarantee that a concurrently visible Geometry record already has a committed geometry payload. T006 therefore owns one minimal compatibility adjustment: add a narrow manager-only domain-registration capability with these semantics:

1. `reserve`: validate the common request and reserve a nonzero monotonic `ObjectId` plus final unique name, but keep the record absent from all public queries and indexes.
2. `publish`: make a reserved record visible only after the owning domain manager has installed a complete payload.
3. `cancel`: discard an unpublished reservation; its `ObjectId` is never reused.
4. `remove`: remove a published record only when the expected `DataObjectType` matches.

The capability returns IDs, value snapshots/results and an opaque move-only reservation token, never a `DataObject*`. It is supplied only to the owning domain manager by the composition root. Ordinary `createObject()` and `removeObject()` must reject `Geometry` and `Mesh`, so business code cannot bypass their managers; generic-object behavior remains unchanged. This is a narrow registration protocol, not a general transaction framework.

Geometry creation follows this order:

1. Parse, validate and completely stage the FITK-backed geometry plus the APPMesh adapter payload without changing either manager.
2. Reserve a hidden `DataObjectType::Geometry` record without holding a `GeometryManager` lock.
3. Under the `GeometryManager` write lock, install the payload keyed by the reserved `ObjectId`, then release the lock.
4. Publish the common record. Geometry queries join through the runtime record first, so an unpublished payload is not observable as a business object.
5. If installation or publication fails, erase the payload if present, cancel the reservation and return `Common::OperationResult` diagnostics. No published common record may remain without a domain payload.

For T016, reader staging additionally owns the real `FITKOCCModelImport` command behind an opaque transaction. The APPMesh `GeometryObject` contains only `providerKey=fitk.occ`, a stable model key and value topology keys; no OCC/FITK pointer crosses the adapter. The transaction state is `Staged`, `Published`, `Finalized` or `RolledBack`: it first publishes the FITK command to `FITKGeometryRepository` while no APPMesh object is visible, then `GeometryManager` publishes the APPMesh record, and only then performs no-throw finalization. A FITK publication failure cannot create an APPMesh record. An APPMesh publication failure rolls the published FITK command back; the APPMesh diagnostic remains first and any rollback diagnostic is appended. Finalized ownership is released only by successful geometry deletion or `GeometryManager` destruction through a value-only external-resource callback after manager locks are released. No create-then-remove compensation is used for this cross-storage boundary.

Geometry deletion first performs all domain/reference checks, then removes the typed common record without holding the geometry lock. If that succeeds, erasing the payload under the geometry write lock is deterministic. If common-record removal fails, the payload remains unchanged. A query that raced with deletion may return the complete pre-delete value snapshot or “not found”, but never an internal pointer or a partial aggregate.

### Concurrency and lock order

`GeometryManager` may own one `QReadWriteLock`. Cross-runtime operations obey these rules:

1. Do not hold a `GeometryManager` lock while calling any `ApplicationRuntime` API.
2. For joined reads, obtain and copy the runtime snapshot first, release the runtime lock inside that call, then obtain and copy the geometry payload.
3. For writes, perform runtime reservation/removal and geometry-map changes as separate short steps using the registration order above. Do not call external callbacks while either lock is held.
4. Queries return value snapshots only. A missing side during a concurrent transition is handled as “not found” or a retryable internal diagnostic, never by returning a partial object.

The effective order is therefore `ApplicationRuntime` operation, lock released, then `GeometryManager` lock. Code must never introduce the reverse order.

### Parallel file ownership

- T006 may add only the `GeometryObject`/`GeometryManager` implementation. It is the only one of T006/T007 allowed to make the narrow domain-registration adjustment to `ApplicationRuntime`; it must preserve all T005 generic-object behavior.
- T007 may add only the mesh value-model and validation implementation. It may include and query the existing `ApplicationRuntime` API as `const`, but must not edit `ApplicationRuntime`, `GeometryManager` or any T008 association/manager code.
- Shared CMake list edits should be serialized by the integrator after both source sets exist. T006 and T007 must not independently rewrite the same list file.
- ModelData unit/concurrency test implementation remains in T009; T006 and T007 only expose the validation and failure-injection seams described below.
- T008 alone adds `MeshManager`, `MeshDataCreator` registration/unregistration, committed mesh common-record registration, and Geometry-Mesh association indexes.

## Common DataObject

`ObjectId` is `quint64`; `0` is `InvalidObjectId`. Allocation is global within one `ApplicationRuntime`, monotonic and non-reusing. A common record contains `id`, unique `name`, `DataObjectType`, `visible`, `selected`, `parentId`, `createdAt` and extension metadata. `ApplicationRuntime` is the only source of these fields and all public reads are `DataObjectSnapshot` values.

## GeometryObject

### Minimal contract

- `GeometryEntityId` is `quint64`; `0` is invalid. It is unique only inside one Geometry object and is addressed together with that geometry's `ObjectId`.
- `GeometryType` is limited to `Invalid`, `BRep`, `Step` and `Iges` for the frozen first-release import formats. File aliases `STP` and `IGS` map to `Step` and `Iges`; `Invalid` is rejected.
- `TopologyEntityType` is limited to `Invalid`, `Vertex`, `Edge`, `Face` and `Solid`. `Invalid` is rejected. Additional OCC topology kinds are added only when a requirement needs them.
- A topology entity snapshot contains `GeometryEntityId`, `TopologyEntityType` and value metadata required for grouping/picking. Any FITK/OCC lookup key remains an internal adapter value.
- `GeometryObjectSnapshot` is a detached value type containing one `DataObjectSnapshot`, `GeometryType`, topology entity snapshots and copied geometry/display metadata. It contains no owning or borrowed FITK pointer.

The common name, visibility, selection, parent and common metadata come only from `ApplicationRuntime`; `GeometryManager` must not duplicate their authoritative values. Geometry-specific metadata and topology indexes belong to its payload.

### FITK boundary

Committed geometry/BRep/topology storage and geometry operations remain in FITK geometry/OCC objects and their manager. APPMesh stores the business `ObjectId`, the stable mapping from `GeometryEntityId` to an adapter/FITK topology key, source-format provenance and APPMesh-only metadata. The internal payload may own an adapter record or stable FITK data key, but snapshots expose neither raw FITK pointers nor duplicated BRep/topology objects.

## MeshData and MeshKernel

### Identity and value semantics

- `NodeId`, `ElementId` and `SetId` are `quint64`; `0` is invalid. Each is unique only within one `MeshKernel`; equal numeric values in different kernels are unrelated.
- T007 `MeshData` and `MeshKernel` are copyable/movable value types whose returned containers are detached values. Their validation must not retain pointers into `ApplicationRuntime` or FITK.
- A `MeshData` has `geometryId`, generator provenance, parameters, one or more kernels and display metadata. `geometryId == InvalidObjectId` means an unassociated/orphan mesh candidate.
- A `MeshKernel` has `MeshDimension`, nodes, elements and sets. Its ordinal inside `MeshData` is used in validation paths; no separate persistent `KernelId` is introduced in T007.

```text
Node     { NodeId id, double x, double y, double z }
Element  { ElementId id, CellType cellType, NodeId nodeIds[] }
MeshSet  { SetId id, name, MeshEntityType entityType, entityIds[] }
```

`MeshDimension` is exactly `D2`, `D3`, or `Mixed` in this increment. `CellType` and fixed arity are limited to:

| CellType | Arity | Dimension |
|---|---:|---:|
| `Line2` | 2 | 1 |
| `Triangle3` | 3 | 2 |
| `Quadrilateral4` | 4 | 2 |
| `Tetrahedron4` | 4 | 3 |
| `Hexahedron8` | 8 | 3 |

An element is valid only when its type is in this table, its node count equals the fixed arity, every node ID is nonzero and exists in the same kernel, and the element dimension is compatible with the kernel dimension. `D2` permits 2D cells and their 1D boundary cells, `D3` permits 3D cells and lower-dimensional boundary cells, and `Mixed` permits the frozen cell set. Higher-order and variable-arity cells are deferred.

`MeshEntityType` is limited to `Node` and `Element`. A `MeshSet` has a nonzero `SetId`, a non-empty name and no duplicate member IDs. Every member must exist in the same kernel's corresponding node or element index. A set cannot mix node and element references.

### Geometry reference validation

T007 does not depend on `GeometryManager`. When `geometryId` is nonzero, validation calls `ApplicationRuntime::findById()` through a read-only reference and succeeds only if the returned snapshot exists and has `DataObjectType::Geometry`. Zero is accepted as “no geometry association”. T007 neither reads geometry payloads nor creates a reverse association. Geometry-Mesh forward/reverse indexes are exclusively T008 work.

### Generator and metadata contract

Generator provenance is stored as `{ QString key, QString version }`; `key` is non-empty and stable within the plugin registry, while `version` is an opaque plugin-supplied value. `QVariantMap commonParameters`, `QVariantMap generatorParameters` and `QVariantMap displayMetadata` are stored separately and copied in value snapshots. APPMesh validates only known common fields; generator-specific meaning remains with the selected generator. T006-T008 do not introduce a universal plugin schema.

### FITK boundary

FITK already provides geometry/model managers, `FITKAbstractMesh`, node/element lists and unstructured topology. It remains the canonical owner of committed heavy mesh arrays, topology and component objects. T007 value objects are validation/staging and interchange snapshots used before commit; they do not establish a second long-lived mesh repository. T008 adapts a validated value into a FITK mesh and stores only APPMesh business identity, generator provenance, display metadata and stable adapter/index keys. Conversion to FITK's current integer IDs must range-check every `quint64` value and report failure rather than truncate it.

## Validation and diagnostics for T006-T008

T006 and T007 return the existing `Common::OperationResult` and `Common::Diagnostic`. They do not implement or expose T014 `Task`/`ErrorInfo`.

- `Diagnostic::objectId` identifies the top-level common object when one exists.
- `Diagnostic::stage` identifies the validation stage, for example `geometry.validate` or `mesh.validate`.
- `Diagnostic::path` is a logical model location, not a filesystem path, for example `topology/entities/12`, `kernels/0/nodes/7`, `kernels/0/elements/9`, or `kernels/0/sets/3`.
- `Diagnostic::detail` records the rejected value and expected constraint without embedding a machine-specific path.

Diagnostic codes are internal test/log identifiers in this release and are not a stable public API.

### Deferred test boundary

T009 remains responsible for implementation, but T006-T008 must leave seams for these checks:

- barrier-controlled concurrent create/query/delete loops complete within a bounded timeout and preserve both runtime and domain-index validators;
- injected failure after validation, reservation, payload installation and publication leaves no published common record without payload and no leaked reservation;
- failed deletion leaves both sides intact; successful deletion leaves neither side nor a stale Geometry-Mesh index;
- joined queries during mutation return a complete before/after value or “not found”, never a pointer or partial aggregate;
- nonzero `geometryId` rejects missing and non-Geometry records without consulting `GeometryManager`;
- duplicate/zero IDs, bad arity, missing node references, incompatible dimensions and invalid set references produce diagnostics at the exact logical path.

## Task

`TaskId` is a process-local monotonic `quint64`; zero is invalid, allocated IDs are never reused, and exhaustion is reported without wraparound. `Task` stores a copied `OperatorInput` containing an operation key, parameter snapshot, source `ObjectId` values and optional description. It stores no UI, manager, runtime, FITK object or file-handle pointer.

The frozen states are `Created`, `Executing`, `Succeeded` and `Failed`. The only legal transitions are `Created -> Executing`, `Executing -> Succeeded` and `Executing -> Failed`. Created is a submission-time state, not background execution. Succeeded and Failed are mutually exclusive terminal states; no transition, progress, result or business diagnostic is accepted after either terminal state. Cancellation, pause and retry are not defined.

`Task` protects its narrow mutable state with one mutex and returns detached `TaskSnapshot` values. It records UTC creation/start/completion timestamps, optional 0-100 progress and phase, an optional value-semantic result reference, diagnostics and an optional primary `ErrorInfo`. It starts no thread and stores no observer. Each accepted update returns a value `TaskEvent` with a task-local monotonic sequence covering Started, ProgressChanged, DiagnosticReported, Succeeded or Failed. A future TaskService owns scheduling and observer dispatch.

## Plugin Metadata

The FastCAE plugin mechanism supplies the plugin key and concrete lifecycle metadata. Additional fields depend on the plugin implementation; a universal descriptor schema is not required in the first release.

The first release exposes no unified cancellation operation.

## Project

`Project` is a business concept representing a project context and its possible geometry, mesh, plugin and runtime content. It is not a promise that all fields are persisted in the first release. The first release guarantees Version metadata and read/write dispatch to currently loaded plugins when project type/version match; it does not require a unified Geometry/Mesh/Runtime snapshot, atomic replacement or raw preservation of unknown plugin data.

## ExtensionRequest

Represents an operation received through an available Python/HTTP/AI wrapper. Exact fields and behavior follow the concrete wrapper; stable external schema is not promised.

## ErrorInfo

`category`, `code`, `message`, `detail`, `recoverable`, `taskId`, `objectId` and `path`. Code and message must be non-empty. `ErrorInfo` is a copyable/movable value and converts losslessly to and from `Common::Diagnostic`; the latter now carries optional string-form task/object IDs for this boundary. Codes are diagnostic identifiers in this release, not a promised stable external API. Earlier ModelData tasks continue using `Common::Diagnostic` directly.
