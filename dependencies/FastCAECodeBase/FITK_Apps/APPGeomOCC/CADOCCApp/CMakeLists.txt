cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(CADOCCApp
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(${PROJECT_NAME} WIN32)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationCAD.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindowGenerator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalDataFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ComponentFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineHandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SignalProcessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/WorkBenchHandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PythonRegister.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CADOCCSettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AppInitializer.h
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationCAD.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindowGenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalDataFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ComponentFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SignalProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WorkBenchHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PythonRegister.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CADOCCSettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AppInitializer.cpp
)

set(RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resource/resources.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resource/translations.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resource/FastCAEAssistant.qrc
)

source_group("Resource Files" FILES ${RESOURCES})
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

target_sources(${PROJECT_NAME} PRIVATE
    ${HEADERS}
    ${SOURCES}
    ${RESOURCES}
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

if(NOT TARGET HDF5::HDF5)
    include(${TOOLS_DIR}/cmake/HDF5Config.cmake)
endif()

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        OCC::OCC
        HDF5::HDF5
)

link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKCompMessageWidget)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKGeoCompOCC)
link_fitk_library(${PROJECT_NAME} FITKRenderWindowOCC)
link_fitk_library(${PROJECT_NAME} FITKAIAssistant)
link_fitk_library(${PROJECT_NAME} OperatorsGUI)
link_fitk_library(${PROJECT_NAME} OperatorsModel)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} GUIWidget)
link_fitk_library(${PROJECT_NAME} GUIFrame)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeoIOHDF5)
link_fitk_library(${PROJECT_NAME} FITKGeoOCCIOHDF5)
link_fitk_library(${PROJECT_NAME} FITKPython)
link_fitk_library(${PROJECT_NAME} FITKHttpPythonDriver)
link_fitk_library(${PROJECT_NAME} PythonInterface)

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

    WIN32_EXECUTABLE ON
)
