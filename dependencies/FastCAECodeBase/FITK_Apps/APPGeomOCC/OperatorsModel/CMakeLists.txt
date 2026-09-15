cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(OperatorsModel VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_library(${PROJECT_NAME} SHARED)

set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsModelAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OpersModelInterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperManagerBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperBoolManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperComponentManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperCompoundManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperCreateGeoManagerBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperCurveManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperDeleteFloatingEdgeManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperExportManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperFaceHidden.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperFeatureManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperFillFaceManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperImportManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperImprintSolidManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperMergeEdgeManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperNBodyUniteManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperOpenHDF5.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperPointManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperPrimitiveManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperReferenceManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSaveHDF5.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSketchManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSolidManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSolidOperManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSplitByMiddleEdgeManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSplitterManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSurfaceManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTransformationManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperUndoRedo.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/OpersModelInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperManagerBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperBoolManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperComponentManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperCompoundManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperCreateGeoManagerBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperCurveManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperDeleteFloatingEdgeManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperExportManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperFaceHidden.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperFeatureManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperFillFaceManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperImportManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperImprintSolidManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperMergeEdgeManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperNBodyUniteManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperOpenHDF5.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperPointManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperPrimitiveManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperReferenceManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSaveHDF5.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSketchManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSolidManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSolidOperManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSplitByMiddleEdgeManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSplitterManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSurfaceManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperTransformationManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperUndoRedo.cpp
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resource/translations/Translate_OperatorsModel_EN.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resource/translations/Translate_OperatorsModel_CN.ts
)

source_group("Translations Files" FILES ${TS_FILES})
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
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

macro(link_fitk_library_om target_name lib_name)
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

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
)

link_fitk_library_om(${PROJECT_NAME} FITKCore)
link_fitk_library_om(${PROJECT_NAME} FITKAppFramework)
link_fitk_library_om(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library_om(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library_om(${PROJECT_NAME} FITKInterfaceGeoIOHDF5)
link_fitk_library_om(${PROJECT_NAME} FITKGeoOCCIOHDF5)
link_fitk_library_om(${PROJECT_NAME} FITKRenderWindowOCC)
link_fitk_library_om(${PROJECT_NAME} OperatorsInterface)
link_fitk_library_om(${PROJECT_NAME} GUIWidget)
link_fitk_library_om(${PROJECT_NAME} GUIFrame)
link_fitk_library_om(${PROJECT_NAME} GUIDialog)
link_fitk_library_om(${PROJECT_NAME} PickDataProvider)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        OperatorsModel_API
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

    OUTPUT_NAME "OperatorsModel"

    # 设置生成文件的位置
    AUTOGEN_BUILD_DIR "${CMAKE_SOURCE_DIR}/generate/${PROJECT_NAME}/autogen"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_NAME:${PROJECT_NAME}> build completed."
)
