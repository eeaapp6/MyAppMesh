# MyAppMesh Constitution

<!--
Sync Impact Report
Version change: template -> 1.0.0
Modified principles: replaced placeholders with project-specific rules
Added sections: Architecture Constraints, Development Workflow
Removed sections: none
Templates requiring updates: none; feature artifacts remain compatible
Follow-up TODOs: none
-->

## Core Principles

### I. FastCAE/FITK Foundation

The application shall use the existing FastCAE/FITK codebase as the primary desktop-CAE foundation. New functionality must integrate through stable public interfaces and must not duplicate FITK data-model or rendering responsibilities without a documented reason.

### II. Layered, Explicit Architecture

UI, application services, domain data, and infrastructure (including file I/O and external processes) must remain separately identifiable. Dependencies flow from UI to application services and from services to domain/infrastructure; domain code must not depend on Qt widgets or process-launch details.

### III. Minimal Runnable Increments

Every implementation phase must produce a buildable and manually verifiable increment. Prefer the smallest end-to-end path first, then extend it. A task is not complete when code merely compiles; its stated acceptance check must also be executable.

### IV. Reproducible Builds and Tests

Build configuration, required tools, test commands, and expected results must be documented in the feature quickstart or project documentation. New behavior should have a focused automated test where practical, with deterministic inputs and no hidden machine-specific state.

### V. Traceable Specification

Implementation decisions must be traceable to the feature specification, plan, or an explicitly recorded research decision. When behavior or scope changes, update the relevant design artifact before changing code so that `tasks.md` remains an actionable source of truth.

## Architecture Constraints

- C++ and Qt code must follow the repository's existing toolchain and FastCAE integration conventions.
- Long-running mesh, simulation, or file operations must not block the UI thread; expose progress and failure states through application-level services.
- Paths, formats, and external executables must be validated at boundaries and reported with actionable errors.
- Generated or third-party code must remain isolated from hand-written application code.

## Development Workflow

Work proceeds in the Spec Kit order: specify, clarify when needed, plan, generate tasks, implement, then analyze and verify. Each completed phase should identify the feature artifacts it satisfies and include the commands used for verification. Unresolved assumptions belong in `research.md` or the feature spec rather than in undocumented code behavior.

## Governance

This constitution is the project's engineering baseline. A change that violates a principle requires an explicit rationale in the plan and a documented alternative or follow-up task. Amendments must update the version and last-amended date, summarize the impact on templates or workflows, and be reviewed together with the affected feature artifacts.

**Version**: 1.0.0 | **Ratified**: 2026-09-15 | **Last Amended**: 2026-09-15
