cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(PythonInterfaceGlobal VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalFiles.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PythonInterfaceGlobalAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalGeometrys.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalMeshs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelSetWapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelSurfaceWapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MeshGenerateWapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SolverWapper.h
)
set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalFiles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalGeometrys.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalMeshs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelSetWapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelSurfaceWapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MeshGenerateWapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SolverWapper.cpp
)

source_group("Header Files" FILES ${HEADERS_LIST})
source_group("Source Files" FILES ${SOURCES_LIST})
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME} PRIVATE
    ${HEADERS_LIST}
    ${SOURCES_LIST}
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
)

set(OUTPUT_DIR_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../../output/bin_d")
set(OUTPUT_DIR_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../output/bin")

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

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKPython)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKRadiossData)
link_fitk_library(${PROJECT_NAME} OperatorsInterface)
link_fitk_library(${PROJECT_NAME} FITKHttpPythonDriver)
link_fitk_library(${PROJECT_NAME} FITKInterfacePhysics)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMeshGen)

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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_NAME:${PROJECT_NAME}> build completed."
)
