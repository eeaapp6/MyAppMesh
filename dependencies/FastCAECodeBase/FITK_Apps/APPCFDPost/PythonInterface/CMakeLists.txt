cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(PythonInterface VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_library(${PROJECT_NAME} SHARED)

set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/PythonInterfaceAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalFiles.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalFiles.cpp
)
#
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

set(OUTPUT_DIR_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../output/bin_d")
set(OUTPUT_DIR_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../output/bin")

macro(link_fitk_library_pyi target_name lib_name)
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

if(NOT TARGET Python37::Python37)
    include(${TOOLS_DIR}/cmake/Python37Config.cmake)
endif()

if(NOT TARGET PythonQt::PythonQt)
    include(${TOOLS_DIR}/cmake/PythonQtConfig.cmake)
endif()

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
)

# 导入内部库
link_fitk_library_pyi(${PROJECT_NAME} FITKAppFramework)
link_fitk_library_pyi(${PROJECT_NAME} FITKCore)
link_fitk_library_pyi(${PROJECT_NAME} FITKHttpPythonDriver)

target_link_libraries(${PROJECT_NAME} PUBLIC Python37::Python37)
target_link_libraries(${PROJECT_NAME} PUBLIC PythonQt::PythonQt)

target_compile_definitions(${PROJECT_NAME} PUBLIC PythonInterface_API)

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

    OUTPUT_NAME "PythonInterface"
	
    # 设置生成文件的位置
    AUTOGEN_BUILD_DIR "${CMAKE_SOURCE_DIR}/generate/${PROJECT_NAME}/autogen"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_NAME:${PROJECT_NAME}> build completed."
)
