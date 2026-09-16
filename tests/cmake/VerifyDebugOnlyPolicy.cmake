if(NOT DEFINED SOURCE_DIR OR NOT DEFINED BINARY_DIR OR NOT DEFINED RUNTIME_DIR OR
   NOT DEFINED CONFIG)
    message(FATAL_ERROR "SOURCE_DIR, BINARY_DIR, RUNTIME_DIR and CONFIG are required")
endif()

if(NOT CONFIG STREQUAL "Debug")
    message(FATAL_ERROR "APPMesh rejects non-Debug test configuration: ${CONFIG}")
endif()

file(STRINGS "${BINARY_DIR}/CMakeCache.txt" _configuration_types
    REGEX "^CMAKE_CONFIGURATION_TYPES:")
if(NOT _configuration_types STREQUAL "CMAKE_CONFIGURATION_TYPES:STRING=Debug")
    message(FATAL_ERROR
        "CMAKE_CONFIGURATION_TYPES must contain only Debug; found: ${_configuration_types}")
endif()

file(TO_CMAKE_PATH "${RUNTIME_DIR}" _runtime_dir)
file(TO_CMAKE_PATH "${BINARY_DIR}/runtime/Debug" _expected_runtime_dir)
if(NOT _runtime_dir STREQUAL _expected_runtime_dir)
    message(FATAL_ERROR
        "Debug runtime output directory mismatch: ${_runtime_dir}; expected ${_expected_runtime_dir}")
endif()

file(READ "${SOURCE_DIR}/cmake/AppMeshDependencies.cmake" _dependency_rules)
foreach(_forbidden IN ITEMS
        "IMPORTED_IMPLIB_RELEASE"
        "IMPORTED_LOCATION_RELEASE"
        "MAP_IMPORTED_CONFIG_RELWITHDEBINFO"
        "MAP_IMPORTED_CONFIG_MINSIZEREL")
    string(FIND "${_dependency_rules}" "${_forbidden}" _position)
    if(NOT _position EQUAL -1)
        message(FATAL_ERROR "Forbidden non-Debug import mapping remains: ${_forbidden}")
    endif()
endforeach()

message(STATUS "Verified APPMesh Debug-only CMake policy")
