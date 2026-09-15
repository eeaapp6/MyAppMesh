cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(PythonInterface
    VERSION 1.1.1
    LANGUAGES CXX
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_library(${PROJECT_NAME} SHARED)

set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/PythonInterfaceAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalFiles.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PyPhysicsFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PhysicsFactoryWapper.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalFiles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PyPhysicsFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PhysicsFactoryWapper.cpp
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

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
)

set(OUTPUT_DIR_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../output/bin_d")
set(OUTPUT_DIR_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../output/bin")

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

# 包含 SARibbon 配置
if(NOT TARGET SARibbon::SARibbon)
    include(${TOOLS_DIR}/cmake/SARibbonConfig.cmake)
endif()

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        SARibbon::SARibbon
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKInterfaceFlowOF)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMesh)
link_fitk_library(${PROJECT_NAME} FITKEasyParam)
link_fitk_library(${PROJECT_NAME} GUIFrame)
link_fitk_library(${PROJECT_NAME} GUICalculateDialog)
link_fitk_library(${PROJECT_NAME} FITKOFDriver)


# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        PythonInterface_API
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.1.1
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

    AUTOGEN_BUILD_DIR "${CMAKE_SOURCE_DIR}/generate/${PROJECT_NAME}/autogen"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_NAME:${PROJECT_NAME}> build completed."
)