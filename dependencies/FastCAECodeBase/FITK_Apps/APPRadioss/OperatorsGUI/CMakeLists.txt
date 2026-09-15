# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(OperatorsGUI
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
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsGUIAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OpersGUIInterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGraphPreprocess.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGraphEvent3D.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGraphPick.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGraphPickPreview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGraphPostProcess.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperResultTreeAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperResultTreeEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorChangeGraph.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperFieldOutput.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeAssemblyAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeAssemblyEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeGroupAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeGroupEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreePropertyAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreePropertyEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeSolutionAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeSolutionEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperLicense.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperScript.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/OpersGUIInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGraphPreprocess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGraphEvent3D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGraphPick.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGraphPickPreview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGraphPostProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperResultTreeAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperResultTreeEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorChangeGraph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperFieldOutput.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeAssemblyAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeAssemblyEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeGroupAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeGroupEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreePropertyAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreePropertyEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeSolutionAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTreeSolutionEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperLicense.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperScript.cpp
)

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

# 包含 VTK 配置
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

link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} GUIFrame)
link_fitk_library(${PROJECT_NAME} GUIWidget)
link_fitk_library(${PROJECT_NAME} FITKRenderWindowVTK)
link_fitk_library(${PROJECT_NAME} FITKRadiossData)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} OperatorsInterface)
link_fitk_library(${PROJECT_NAME} GraphDataProvider)
link_fitk_library(${PROJECT_NAME} GraphDataVTKAdaptor)
link_fitk_library(${PROJECT_NAME} PickDataProvider)
link_fitk_library(${PROJECT_NAME} GUIToolDialog)
link_fitk_library(${PROJECT_NAME} FITKInterfaceStructuralPost)
link_fitk_library(${PROJECT_NAME} GUIPostDialog)
link_fitk_library(${PROJECT_NAME} FITKPython)

target_link_libraries(${PROJECT_NAME} PUBLIC SARibbon::SARibbon)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        OperatorsGUI_API
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
