# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(GUIWidget
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
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIWidgetAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ControlPanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelCaseTree.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ParamWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphArea.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphMdiArea.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PreGraphWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIDialogBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITreeWidgetBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PostGraphWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ResultTreeWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeWidgetAssembly.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeWidgetGroup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeWidgetProperty.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeWidgetSolution.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/ControlPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelCaseTree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ParamWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphArea.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphMdiArea.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PreGraphWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIDialogBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUITreeWidgetBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PostGraphWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ResultTreeWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeWidgetAssembly.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeWidgetGroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeWidgetProperty.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeWidgetSolution.cpp
)
set(UI_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/ParamWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphArea.ui
)
set(RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resources/images.qrc
)
set(TS_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/../ZH_CN/GUIWidget_ZH_CN.ts
)

source_group("From Files" FILES ${UI_FILES_LIST})
source_group("Translation Files" FILES ${TS_FILES})
source_group("Resource Files" FILES ${RESOURCES})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${UI_FILES_LIST}
    ${RESOURCES}
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

# SARibbon 包含路径
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

# 包含 SARibbon 配置
if(NOT TARGET SARibbon::SARibbon)
    include(${TOOLS_DIR}/cmake/SARibbonConfig.cmake)
endif()

# 输出目录配置
set(OUTPUT_DIR_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../output/bin_d")
set(OUTPUT_DIR_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../output/bin")

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
        SARibbon::SARibbon
)

link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} FITKRenderWindowVTK)
link_fitk_library(${PROJECT_NAME} FITKRadiossData)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKInterfacePhysics)
link_fitk_library(${PROJECT_NAME} OperatorsInterface)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKInterfaceStructuralPost)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        GUIWidget_API
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
