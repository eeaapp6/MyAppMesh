# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(OperatorsInterface
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
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsInterfaceAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphEventOperator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphInteractionOperator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ParaWidgetInterfaceOperator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeEventOperator.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphEventOperator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphInteractionOperator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ParaWidgetInterfaceOperator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeEventOperator.cpp
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
if(WIN32)
    target_include_directories(${PROJECT_NAME} 
        PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/../Tools/Win64/SARibbon/include/SARibbon-2.0.1/
    )
else()
    target_include_directories(${PROJECT_NAME} 
        PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/../Tools/Linux64/SARibbon/include/
    )
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
)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        OperatorsInterface_API
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)

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
