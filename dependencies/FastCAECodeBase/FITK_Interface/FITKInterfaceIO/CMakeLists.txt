# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKInterfaceIO
    VERSION 1.0.1
    LANGUAGES CXX
)

# Qt配置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找Qt5组件

find_package(Qt5 COMPONENTS Core Gui Widgets Xml REQUIRED)

# 定义为共享库
add_library(${PROJECT_NAME} SHARED)

# 手动添加文件列表
set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractRapidXMLWriter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInterfaceIOAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractIO.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractTextReader.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractTextStreamReader.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractTextWriter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractHDF5Reader.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractHDF5Writer.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractQtJsonReader.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractQtXMLReader.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractQtXMLWriter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractRapidXMLReader.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKHDF5FileTool.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractIO.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractRapidXMLWriter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractTextReader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractTextStreamReader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractTextWriter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractHDF5Reader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractHDF5Writer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractQtJsonReader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractQtXMLReader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractQtXMLWriter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractRapidXMLReader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKHDF5FileTool.cpp 
)

set(UI_FILES_LIST
#    ${CMAKE_CURRENT_SOURCE_DIR}/PluginManageDialog.ui 
)

set(RESOURCES_SOURCES
#    ${CMAKE_CURRENT_SOURCE_DIR}/../qrc/qianfan.qrc
)

source_group("Ui Files" FILES ${UI_FILES_LIST})
source_group("Resource Files" FILES ${RESOURCES_SOURCES})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*|.*autouic_.*\\.stamp$")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${UI_FILES_LIST}
    ${RESOURCES_SOURCES}
)

# 包含目录
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
)

# 添加多核编译支持
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
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

# 添加第三方库 rapidxml 的包含路径
if(WIN32)
    target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
        "${TOOLS_DIR}/Win64/rapidxml"
    )
else()
    target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
        "${TOOLS_DIR}/Linux64/rapidxml"
    )
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
        Qt5::Xml
	    HDF5::HDF5
)

link_fitk_library(${PROJECT_NAME} FITKCore)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKInterfaceIO_API
)

# 版本信息
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0.1
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
