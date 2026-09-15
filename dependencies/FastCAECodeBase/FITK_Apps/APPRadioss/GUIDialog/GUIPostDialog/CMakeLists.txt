# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(GUIPostDialog
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

set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPostDialogAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUISolverSettingDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRadiossResultRequestDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUISolverRunDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIColorBarDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistoryACCELDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUICSVCurveDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistoryINTERDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistoryRBODYDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistoryRWALLDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistorySECTIODialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistorySPHCELDialog.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GUISolverSettingDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRadiossResultRequestDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUISolverRunDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIColorBarDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistoryACCELDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUICSVCurveDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistoryINTERDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistoryRBODYDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistoryRWALLDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistorySECTIODialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistorySPHCELDialog.cpp
)

set(UI_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GUISolverSettingDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRadiossResultRequestDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUISolverRunDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIColorBarDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistoryACCELDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUICSVCurveDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistoryINTERDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistoryRBODYDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistoryRWALLDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistorySECTIODialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITimeHistorySPHCELDialog.ui
)

source_group("From Files" FILES ${UI_FILES_LIST})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${UI_FILES_LIST}
    ${RESOURCE_FILES_LIST}
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

# 根据平台添加 qwt 包含路径和库路径
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

# 包含 Qwt 配置
if(NOT TARGET qwt::qwt)
    include(${TOOLS_DIR}/cmake/qwtConfig.cmake)
endif()

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

# 链接
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
)
# 链接 qwt 库
target_link_libraries(${PROJECT_NAME} PUBLIC qwt::qwt)

link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKRadiossData)
link_fitk_library(${PROJECT_NAME} GUIWidget)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} FITKPlotWindow)
link_fitk_library(${PROJECT_NAME} PickDataProvider)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        GUIPostDialog_API
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
