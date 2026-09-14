# Plugin Protocol Contract

## Discovery and metadata

Each dynamic library exposes the creation entry point required by the FastCAE plugin mechanism. Discovery, loading, registration and compatibility checks use FastCAE interfaces. Plugin configuration, errors, capabilities and metadata follow the concrete FastCAE plugin interface; no universal descriptor or cancellation field is required in the first release.

## Lifecycle

The applicable FastCAE lifecycle and registration hooks are used for discover, validate, create, load/register, initialize, disable/unload and destroy operations.

Incompatible or failed plugins are not loaded or registered and produce diagnostics. Registration and unload behavior follows the concrete FastCAE plugin interface; the first release does not impose transactional rollback, data conversion/export prompts or complex unload interaction.

## Generator capability

A generator receives the parameters and input data required by its concrete driver and reports the result through the existing operator/plugin path. A universal typed parameter schema, staged result contract and cancellation contract are future extensions.
