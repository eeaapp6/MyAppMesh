# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(CFDPostAPP      # 项目名称
    VERSION 1.0.0    # 项目版本
    LANGUAGES CXX    # 项目语言
)

# Qt配置
set(CMAKE_AUTOMOC ON)    # MOC
set(CMAKE_AUTORCC ON)    # RESOURCE
set(CMAKE_AUTOUIC ON)    # UI

# 查找Qt5组件，根据项目增删
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# 可执行文件目标
add_executable(${PROJECT_NAME}) 

# 根据路径添加文件
set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindowGenerator.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/ComponentFactory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalDataFactory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineHandler.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/WorkBenchHandler.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/CFDPostAPPSettings.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/PreWindowInitializer.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/PythonRegister.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/AppInitializer.h
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindowGenerator.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/ComponentFactory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalDataFactory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineHandler.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/WorkBenchHandler.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/CFDPostAPPSettings.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/PreWindowInitializer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/PythonRegister.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/AppInitializer.cpp
)

set(RESOURCE_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resource/FastCAEAssistant.qrc
)

# 图标文件配置
if(WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/icon.ico")
    set(APP_ICON_RESOURCE "${CMAKE_CURRENT_BINARY_DIR}/app_icon.rc")
    file(WRITE "${APP_ICON_RESOURCE}" "IDI_ICON1 ICON DISCARDABLE \"${CMAKE_CURRENT_SOURCE_DIR}/icon.ico\"\n")
endif()

# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${HEADERS}
    ${SOURCES}
    ${APP_ICON_RESOURCE}
    ${RESOURCE_FILES_LIST}
)

# 添加多核编译支持
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()

# 包含目录根据pro文件中的 INCLUDEPATH 填写
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
)

# 输出目录配置  根据当前的路径找到output路径
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

# 用户自定义TOOLS路径选项
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

# 三方库，从TOOLS中获取
if(NOT TARGET VTK::VTK)
    include(${TOOLS_DIR}/cmake/VTKConfig.cmake)
endif()

# 链接
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        # Qt5的组件
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
		
        # 三方库
        VTK::VTK
)

# 导入内部库
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKPython)
link_fitk_library(${PROJECT_NAME} FITKRenderWindowVTK)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} FITKInterfaceCFDPost)
link_fitk_library(${PROJECT_NAME} FITKPlotWindow)
link_fitk_library(${PROJECT_NAME} FITKHttpPythonDriver)
link_fitk_library(${PROJECT_NAME} GUIFrame)
link_fitk_library(${PROJECT_NAME} GUIWidget)
link_fitk_library(${PROJECT_NAME} OperatorsModel)
link_fitk_library(${PROJECT_NAME} OperatorsGUI)
link_fitk_library(${PROJECT_NAME} FITKEasyParam)        # Unix配置中包含
link_fitk_library(${PROJECT_NAME} OperatorsInterface)   # Unix配置中包含
link_fitk_library(${PROJECT_NAME} PythonInterface)
link_fitk_library(${PROJECT_NAME} FITKAIAssistant)

# 版本信息
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0.0
	
    # DEBUG
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR_DEBUG}"

    # RELEASE
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR_RELEASE}"

    # RELWITHDEBINFO
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR_RELEASE}"

    # MINSIZEREL
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${OUTPUT_DIR_RELEASE}"

    # 设置生成文件的位置
    AUTOGEN_BUILD_DIR "${CMAKE_SOURCE_DIR}/generate/${PROJECT_NAME}/autogen"
)