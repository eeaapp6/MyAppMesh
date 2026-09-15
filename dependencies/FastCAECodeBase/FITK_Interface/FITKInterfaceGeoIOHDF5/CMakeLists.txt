# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKInterfaceGeoIOHDF5
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
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInterfaceGeoIOHDF5API.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoHDF5KeyWords.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGeoHDF5Adaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoCommand.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelBox.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelCylinder.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelSphere.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelBool.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelPoint.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModel.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelSketch.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelTransform.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelCurve.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelThreePointsCurve.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelFace.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelSolid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelSplitter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoDatum.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoDatumModel.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelPartition.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelCone.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelFrustum.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelFilledEllipse.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelFilledRectangle.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelFilledParaboloid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelFilledPolygon.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelMergePart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelSplitPart.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGeoHDF5Adaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoCommand.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelBox.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelCylinder.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelSphere.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelBool.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelPoint.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelSketch.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelTransform.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelCurve.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelThreePointsCurve.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelFace.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelSolid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelSplitter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoDatum.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoDatumModel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelPartition.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelCone.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelFrustum.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelFilledEllipse.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelFilledRectangle.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelFilledParaboloid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelFilledPolygon.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelMergePart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAdaptorGeoModelSplitPart.cpp 
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
	    HDF5::HDF5
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAdaptor)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKInterfaceIO)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKINTERFACEGEOIOHDF5_API
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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_NAME:${PROJECT_NAME}> build completed."
)
