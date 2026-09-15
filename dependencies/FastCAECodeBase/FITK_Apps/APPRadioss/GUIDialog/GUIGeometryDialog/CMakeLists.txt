# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(GUIGeometryDialog
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
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryDialogAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryBoxDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryCylinderDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometrySphereDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryBoolDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryChamferDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryFilletDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryDefeatureDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryMirrorDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryRecPatternDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryCirPatternDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryScaleDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryRotationDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryTransDialog.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryBoxDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryCylinderDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometrySphereDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryBoolDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryChamferDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryFilletDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryDefeatureDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryMirrorDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryRecPatternDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryCirPatternDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryScaleDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryRotationDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryTransDialog.cpp
)

set(UI_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryBoxDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryCylinderDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometrySphereDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryBoolDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryChamferDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryFilletDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryDefeatureDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryMirrorDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryRecPatternDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryCirPatternDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryScaleDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryRotationDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGeometryTransDialog.ui
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

# 包含 SARibbon 和 Qwt 配置
if(NOT TARGET SARibbon::SARibbon)
    include(${TOOLS_DIR}/cmake/SARibbonConfig.cmake)
endif()
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

link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} PickDataProvider)
link_fitk_library(${PROJECT_NAME} FITKRadiossData)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} GUIFrame)
link_fitk_library(${PROJECT_NAME} GUIWidget)

target_link_libraries(${PROJECT_NAME} PUBLIC SARibbon::SARibbon qwt::qwt)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        GUIGeometryDialog_API
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
