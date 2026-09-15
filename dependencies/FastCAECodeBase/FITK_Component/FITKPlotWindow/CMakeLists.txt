# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKPlotWindow
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
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPlotWindowAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKXYPlotWindow.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKXYPlotCurveStyle.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKBarChartWindow.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPlotWindowsInterface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKXYPlotProperty.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKXYCurveDrawManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKBarChartProperty.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKBarChartDrawManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPolarPlotWindow.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPolarPlotProperty.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPolarCurveDrawManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKSpectrogramManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKSpectrogramProperty.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKSpectrogramWindow.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKXYPlotWindow.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKXYPlotCurveStyle.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKBarChartWindow.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPlotWindowsInterface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKXYPlotProperty.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKXYCurveDrawManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKBarChartProperty.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKBarChartDrawManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPolarPlotWindow.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPolarPlotProperty.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPolarCurveDrawManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKSpectrogramManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKSpectrogramProperty.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKSpectrogramWindow.cpp 
)

set(UI_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKXYPlotWindow.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKXYPlotCurveStyle.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKBarChartWindow.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPolarPlotWindow.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKSpectrogramWindow.ui 
)

set(RESOURCES_SOURCES
#    ${CMAKE_CURRENT_SOURCE_DIR}/../qrc/qianfan.qrc
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/ZH_CN/FITKPlotWindow_ZH_CN.ts 
)

source_group("Resource Files" FILES ${RESOURCES_SOURCES})
source_group("Translation Files" FILES ${TS_FILES})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*|.*autouic_.*\\.stamp$")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${UI_FILES_LIST}
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

if(NOT TARGET qwt::qwt)
    include(${TOOLS_DIR}/cmake/qwtConfig.cmake)
endif()

# 链接
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
		qwt::qwt
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKPlotWindow_API
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


