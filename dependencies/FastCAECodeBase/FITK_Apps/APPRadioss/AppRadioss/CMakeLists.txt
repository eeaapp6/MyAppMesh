# 指定最低 CMake 版本（与根项目保持一致）
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息：名称、版本、语言
project(AppRadioss
    VERSION 1.0.0
    LANGUAGES CXX
)

message(STATUS "Configuring ${PROJECT_NAME} project...")

# Qt配置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找Qt5组件
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindowGenerator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalDataFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ComponentFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RadiossAppSettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PreWindowInitializer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SignalProcessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineHandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AppInitializer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PythonRegister.h # 实际文件名为 PythonRegister.h，类名为 PyRegister
)
set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindowGenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalDataFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ComponentFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RadiossAppSettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PreWindowInitializer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SignalProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AppInitializer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PythonRegister.cpp # 实际文件名为 PythonRegister.cpp，类名为 PyRegister
)
set(RESOURCE_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resources/FastCAEAssistantSkills.qrc
)

# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

# 可执行文件目标
add_executable(${PROJECT_NAME} WIN32) 

# 添加多核编译支持
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()


# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${UI_FILES_LIST}
    ${RESOURCE_FILES_LIST}
)

# 包含目录
target_include_directories(${PROJECT_NAME}
    PRIVATE
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

# 链接项目内部库和第三方库
target_link_libraries(${PROJECT_NAME}
    PUBLIC   
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        VTK::VTK 
)

target_link_libraries(${PROJECT_NAME} PUBLIC SARibbon::SARibbon)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKPython)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKCompMessageWidget)
link_fitk_library(${PROJECT_NAME} FITKRenderWindowVTK)
link_fitk_library(${PROJECT_NAME} FITKGeoCompOCC)
link_fitk_library(${PROJECT_NAME} FITKRadiossData)
link_fitk_library(${PROJECT_NAME} FITKRadiossRadIO)
link_fitk_library(${PROJECT_NAME} PythonInterfaceGlobal)
link_fitk_library(${PROJECT_NAME} PythonInterfaceProperties)
link_fitk_library(${PROJECT_NAME} PythonInterfaceSolution)
link_fitk_library(${PROJECT_NAME} FITKGmshExeDriver)
link_fitk_library(${PROJECT_NAME} FITKGmshMshIO)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} GUIFrame)
link_fitk_library(${PROJECT_NAME} GUIWidget)
link_fitk_library(${PROJECT_NAME} OperatorsInterface)
link_fitk_library(${PROJECT_NAME} OperatorsGUI)
link_fitk_library(${PROJECT_NAME} OperatorsModel)
link_fitk_library(${PROJECT_NAME} FITKRadiossLSDynaTransfer)
link_fitk_library(${PROJECT_NAME} FITKPlotWindow)
link_fitk_library(${PROJECT_NAME} PickDataProvider)
link_fitk_library(${PROJECT_NAME} FITKRadiossDataIOHDF5)
link_fitk_library(${PROJECT_NAME} FITKGeoOCCIOHDF5)
link_fitk_library(${PROJECT_NAME} FITKNastranBDFIO)
link_fitk_library(${PROJECT_NAME} FITKAbaqusRadiossDataMapper)
link_fitk_library(${PROJECT_NAME} FITKMeshIO)
link_fitk_library(${PROJECT_NAME} FITKAbaqusIOINP)
link_fitk_library(${PROJECT_NAME} FITKAIAssistant)
link_fitk_library(${PROJECT_NAME} FITKHttpPythonDriver)

set_target_properties(${PROJECT_NAME} PROPERTIES
    # 设置生成文件的位置
    AUTOGEN_BUILD_DIR "${CMAKE_SOURCE_DIR}/generate/${PROJECT_NAME}/autogen"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_NAME:${PROJECT_NAME}> build completed."
)

message(STATUS "${PROJECT_NAME} configuration completed.")