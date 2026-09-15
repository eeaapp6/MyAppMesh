# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKFlowOFIOHDF5
    VERSION 1.0.0
    LANGUAGES CXX
)

# Qt配置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 设置并查找Qt5组件

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# 定义为共享库
add_library(${PROJECT_NAME} SHARED)

# 手动添加文件列表
set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFIOHDF5API.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AbstractAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFIOHDF5Interface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5Reader.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5Writer.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorGeometryMeshSize.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorGeometryComponent.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorGeometryRefine.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorMaterialPoint.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorRegionMesh.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorMesh.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorPhysicsData.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorTransportProp.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorDiscretization.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorSolution.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorInitialCondition.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorRunControl.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorBoundary.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorOperatingCond.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorTurbulence.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorRadiation.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorThermo.h  
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AbstractAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFIOHDF5Interface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5Reader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5Writer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorGeometryMeshSize.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorGeometryComponent.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorGeometryRefine.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorMaterialPoint.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorRegionMesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorMesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorPhysicsData.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorTransportProp.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorDiscretization.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorSolution.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorInitialCondition.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorRunControl.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorBoundary.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorOperatingCond.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorTurbulence.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorRadiation.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowOFHDF5AdaptorThermo.cpp 
)

# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
)

# 添加多核编译支持
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()

# 包含目录
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
)

# 输出目录配置
set(OUTPUT_DIR_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../../output/bin_d")
set(OUTPUT_DIR_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../output/bin")

# 定义一个宏来简化跨平台库链接
macro(link_fitk_library target_name lib_name)
    if(WIN32)
        target_link_libraries(${target_name} 
            PUBLIC 
                debug "${OUTPUT_DIR_DEBUG}/${lib_name}.lib"
                optimized "${OUTPUT_DIR_RELEASE}/${lib_name}.lib"
        )
    elseif(UNIX)
        target_link_libraries(${target_name} 
            PUBLIC 
                debug "${OUTPUT_DIR_DEBUG}/lib${lib_name}.so"
                optimized "${OUTPUT_DIR_RELEASE}/lib${lib_name}.so"
        )
    endif()
    if(TARGET ${lib_name})
        add_dependencies(${target_name} ${lib_name})
    endif()
endmacro()

if(NOT TOOLS_DIR)
    if(DEFINED ENV{TOOLS_DIR})
        set(TOOLS_DIR "$ENV{TOOLS_DIR}" CACHE PATH "Path to the Tools directory")
        message(STATUS "TOOLS_DIR loaded from environment: ${TOOLS_DIR}")
    else()
        set(TOOLS_DIR "" CACHE PATH "Path to the Tools directory")
        message(WARNING "TOOLS_DIR is not configured. Please set it via -DTOOLS_DIR=... or environment variable.")
        message(FATAL_ERROR "Please configure TOOLS_DIR first.")
    endif()
endif()

# OCC
if(NOT TARGET OCC::OCC)
    include(${TOOLS_DIR}/cmake/OCCConfig.cmake)
endif()

# HDF5
if(NOT TARGET HDF5::HDF5)
    include(${TOOLS_DIR}/cmake/HDF5Config.cmake)
endif()

# 链接
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
		OCC::OCC
		HDF5::HDF5
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAdaptor)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKInterfaceIO)
link_fitk_library(${PROJECT_NAME} FITKEasyParam)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMesh)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMeshGen)
link_fitk_library(${PROJECT_NAME} FITKInterfaceFlowOF)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKGeoOCCIOHDF5)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKFLOWOFIOHDF5_API
)

# 版本信息
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
	
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR_DEBUG}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR_DEBUG}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR_DEBUG}"

    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR_RELEASE}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR_RELEASE}"

    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR_RELEASE}"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR_RELEASE}"

    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${OUTPUT_DIR_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${OUTPUT_DIR_RELEASE}"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${OUTPUT_DIR_RELEASE}"

    # 设置生成文件的位置
    AUTOGEN_BUILD_DIR "${CMAKE_SOURCE_DIR}/generate/${PROJECT_NAME}/autogen"
)
