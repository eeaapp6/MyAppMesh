if(NOT DEFINED RUNTIME_DIR OR NOT DEFINED CONFIG)
    message(FATAL_ERROR "RUNTIME_DIR and CONFIG are required")
endif()

if(NOT CONFIG STREQUAL "Debug")
    message(FATAL_ERROR "APPMesh runtime validation rejects non-Debug configuration: ${CONFIG}")
endif()

set(_expected "${RUNTIME_DIR}/hdf5_D.dll")
set(_forbidden "${RUNTIME_DIR}/hdf5.dll")

if(NOT EXISTS "${_expected}")
    message(FATAL_ERROR "Expected configuration runtime is missing: ${_expected}")
endif()

if(EXISTS "${_forbidden}")
    message(FATAL_ERROR "Forbidden non-Debug artifact detected: ${_forbidden}")
endif()

if(NOT EXISTS "${RUNTIME_DIR}/SARibbonBard.dll")
    message(FATAL_ERROR "Expected Debug SARibbon runtime is missing: ${RUNTIME_DIR}/SARibbonBard.dll")
endif()
if(EXISTS "${RUNTIME_DIR}/SARibbonBar.dll")
    message(FATAL_ERROR "Forbidden non-Debug artifact detected: ${RUNTIME_DIR}/SARibbonBar.dll")
endif()

message(STATUS "Verified Debug-only runtime: ${_expected}")
