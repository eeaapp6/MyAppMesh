cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(GUIWidget
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_library(${PROJECT_NAME} SHARED)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIWidgetAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MainMenuBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyChildWidgetBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyEmptyWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIDialogBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIWidgetBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CompLineEdit.h
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/MainMenuBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyChildWidgetBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyEmptyWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIDialogBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIWidgetBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CompLineEdit.cpp
)

set(UI_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyEmptyWidget.ui
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resource/translations/Translate_GUIWidget_EN.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resource/translations/Translate_GUIWidget_CN.ts
)

source_group("Form Files" FILES ${UI_FILES})
source_group("Translation Files" FILES ${TS_FILES})
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

target_sources(${PROJECT_NAME} PRIVATE
    ${HEADERS}
    ${SOURCES}
    ${UI_FILES}
    ${TS_FILES}
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

link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKGeoCompOCC)
link_fitk_library(${PROJECT_NAME} FITKRenderWindowOCC)
link_fitk_library(${PROJECT_NAME} FITKOCCGraphAdaptor)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        GUIWidget_API
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

    AUTOGEN_BUILD_DIR "${CMAKE_SOURCE_DIR}/generate/${PROJECT_NAME}/autogen"
)
