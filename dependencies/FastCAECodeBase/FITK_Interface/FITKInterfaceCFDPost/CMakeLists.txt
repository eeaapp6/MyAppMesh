# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKInterfaceCFDPost
    VERSION 0.8.0
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
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInterfaceCFDPostAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vtkCGNSCache.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vtkCGNSFileSeriesReader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vtkCGNSReader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vtkCGNSReaderInternal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vtkFileSeriesHelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vtk_cgns.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cgio_helpers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostEnum.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractCFDPostData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPost3DManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostSteady.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostUnSteady.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostImport.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostIsosurface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostClipPlane.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostClipSphere.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostClipCylinder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostClipBox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostSlicePlane.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostSliceSphere.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostSliceCylinder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostSliceBox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostStreamLine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostGlyph.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAlgGlyph.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAlgPltReader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAlgDatReader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPltReader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAlgCGNSReader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostThreshold.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostDeform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostLight.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostLightManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostProbe.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostColorLibaryData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostColorLibaryDataManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostRuler.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/vtkCGNSFileSeriesReader.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/vtkCGNSReader.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/vtkCGNSReaderInternal.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/vtkFileSeriesHelper.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/cgio_helpers.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractCFDPostData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPost3DManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostSteady.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostUnSteady.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostImport.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostIsosurface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostClipPlane.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostClipSphere.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostClipCylinder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostClipBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostSlicePlane.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostSliceSphere.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostSliceCylinder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostSliceBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostStreamLine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostGlyph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAlgGlyph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAlgPltReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAlgDatReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPltReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAlgCGNSReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostThreshold.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostDeform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostLight.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostLightManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostProbe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostColorLibaryData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostColorLibaryDataManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCFDPostRuler.cpp
)

set(RESOURCES_SOURCES
#    ${CMAKE_CURRENT_SOURCE_DIR}/../Resources/qianfan.rc
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/License.txt
)

source_group("Resource Files" FILES ${RESOURCES_SOURCES})
source_group("Translation Files" FILES ${TS_FILES})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${RESOURCES_SOURCES}
    ${TS_FILES}
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

# CGNS
if(NOT TARGET CGNS::CGNS)
    include(${TOOLS_DIR}/cmake/CGNSConfig.cmake)
endif()

# 链接
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
        VTK::VTK
        CGNS::CGNS
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKVTKAlgorithm)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKInterfaceCFDPost_API
)

# 版本信息
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 0.8.0
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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_NAME:${PROJECT_NAME}> build completed."
)
