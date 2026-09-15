cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(PickDataProvider VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_library(${PROJECT_NAME} SHARED)

set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/PickDataProviderAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ContextMenuHandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPickInfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphPickedData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphPickedDataProvider.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/ContextMenuHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPickInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphPickedData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphPickedDataProvider.cpp
)

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

macro(link_fitk_library_pdp target_name lib_name)
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

if(NOT TARGET OCC::OCC)
    include(${TOOLS_DIR}/cmake/OCCConfig.cmake)
endif()

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        OCC::OCC
)

link_fitk_library_pdp(${PROJECT_NAME} FITKCore)
link_fitk_library_pdp(${PROJECT_NAME} FITKAppFramework)
link_fitk_library_pdp(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library_pdp(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library_pdp(${PROJECT_NAME} FITKGeoCompOCC)
link_fitk_library_pdp(${PROJECT_NAME} FITKRenderWindowOCC)
link_fitk_library_pdp(${PROJECT_NAME} FITKOCCGraphAdaptor)
link_fitk_library_pdp(${PROJECT_NAME} FITKWidget)
link_fitk_library_pdp(${PROJECT_NAME} GUIFrame)

target_compile_definitions(${PROJECT_NAME} 
    PUBLIC 
        PickDataProvider_API
)

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

    OUTPUT_NAME "PickDataProvider"

    # 设置生成文件的位置
    AUTOGEN_BUILD_DIR "${CMAKE_SOURCE_DIR}/generate/${PROJECT_NAME}/autogen"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_NAME:${PROJECT_NAME}> build completed."
)
