if(NOT DEFINED RUNTIME_DIR OR NOT DEFINED CONFIG)
    message(FATAL_ERROR "RUNTIME_DIR and CONFIG are required")
endif()

if(CONFIG STREQUAL "Debug")
    set(_expected "${RUNTIME_DIR}/hdf5_D.dll")
    set(_forbidden "${RUNTIME_DIR}/hdf5.dll")
else()
    set(_expected "${RUNTIME_DIR}/hdf5.dll")
    set(_forbidden "${RUNTIME_DIR}/hdf5_D.dll")
endif()

if(NOT EXISTS "${_expected}")
    message(FATAL_ERROR "Expected configuration runtime is missing: ${_expected}")
endif()

if(EXISTS "${_forbidden}")
    message(FATAL_ERROR "Mixed Debug/Release runtime detected: ${_forbidden}")
endif()

message(STATUS "Verified ${CONFIG} runtime: ${_expected}")
