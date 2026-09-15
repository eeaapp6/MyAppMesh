cmake_minimum_required(VERSION 3.16)

project(APPMesh VERSION 0.1.0 LANGUAGES CXX)

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

set(TOOLS_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/FastCAECodeBase/Tools"
    CACHE PATH "FastCAE dependency bundle root")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(AppMeshDependencies)
include(AppMeshRuntime)

appmesh_configure_dependencies()
appmesh_configure_output_directories()

include(CTest)

add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT appmesh)
