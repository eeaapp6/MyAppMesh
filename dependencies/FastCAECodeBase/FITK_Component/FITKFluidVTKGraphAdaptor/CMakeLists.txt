# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKFluidVTKGraphAdaptor
    VERSION 1.0.0
    LANGUAGES CXX
)

# Qt配置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找Qt5组件

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# 定义为共享库
add_library(${PROJECT_NAME} SHARED)

# 手动添加文件列表
set(HEADERS_LIST
# Base
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKCommons.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphAdaptorAPI.h  
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObject3D.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectModelBase.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectFixedSizeBase.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKViewAdaptorBase.h 
	
# PreProcess
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKShapeColorMap.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectModelCmd.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectModelShape.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectBoundMesh.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectSelect.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectPick.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectPickPreview.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectRegionMesh.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectMeasureTools.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectMaterialPoints.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKViewAdaptorModelCmd.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKViewAdaptorModelShape.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKViewAdaptorBoundMesh.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKViewAdaptorRegionMesh.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKViewAdaptorMaterialPoints.h
)

set(SOURCES_LIST
# Base
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKCommons.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObject3D.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectModelBase.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectFixedSizeBase.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKViewAdaptorBase.cpp
	
# Preprocess 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKShapeColorMap.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectModelCmd.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectModelShape.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectBoundMesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectSelect.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectPick.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectPickPreview.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectRegionMesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectMeasureTools.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKGraphObjectMaterialPoints.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKViewAdaptorModelCmd.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKViewAdaptorModelShape.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKViewAdaptorBoundMesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKViewAdaptorRegionMesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFluidVTKViewAdaptorMaterialPoints.cpp
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

# VTK
if(NOT TARGET VTK::VTK)
    include(${TOOLS_DIR}/cmake/VTKConfig.cmake)
endif()

# 链接
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
        VTK::VTK
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAdaptor)
link_fitk_library(${PROJECT_NAME} FITKVTKAlgorithm)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMesh)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMeshGen)
link_fitk_library(${PROJECT_NAME} FITKRenderWindowVTK)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKFLUIDGRAPHADAPTOR_API
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
