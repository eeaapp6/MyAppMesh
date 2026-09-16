cmake_minimum_required(VERSION 3.16)

set(CMAKE_TRY_COMPILE_CONFIGURATION Debug CACHE STRING
    "Configuration used by compiler and dependency probes" FORCE)

project(APPMesh VERSION 0.1.0 LANGUAGES CXX)

get_property(_appmesh_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(_appmesh_multi_config)
    set(CMAKE_CONFIGURATION_TYPES Debug CACHE STRING
        "APPMesh supported build configurations" FORCE)
else()
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING
            "APPMesh supported build configuration" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(FATAL_ERROR
            "APPMesh build policy is Debug only; rejected CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}.")
    endif()
endif()

message(STATUS "APPMesh build policy: Debug only")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT WIN32)
    message(FATAL_ERROR "APPMesh currently supports the Windows x64 FastCAE baseline only.")
endif()

if(NOT MSVC)
    message(FATAL_ERROR "APPMesh requires MSVC v141 from Visual Studio 2017.")
endif()

if(NOT MSVC_VERSION EQUAL 1916)
    message(FATAL_ERROR
        "APPMesh requires MSVC 19.16/v141; detected MSVC_VERSION=${MSVC_VERSION}.")
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "APPMesh requires a 64-bit toolchain.")
endif()

find_program(APPMESH_DUMPBIN_EXECUTABLE NAMES dumpbin)
if(NOT APPMESH_DUMPBIN_EXECUTABLE)
    message(FATAL_ERROR
        "dumpbin is required for APPMesh dependency validation.")
endif()

set(TOOLS_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/FastCAECodeBase/Tools"
    CACHE PATH "FastCAE dependency bundle root")
set(FITK_OUTPUT_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/output"
    CACHE PATH "Root containing the explicitly allowlisted FITK Debug binaries")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(AppMeshDependencies)
include(AppMeshFITKDependencies)
include(AppMeshRuntime)

appmesh_configure_dependencies()
appmesh_configure_fitk_dependencies()
appmesh_configure_output_directories()

include(CTest)

add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT appmesh)
