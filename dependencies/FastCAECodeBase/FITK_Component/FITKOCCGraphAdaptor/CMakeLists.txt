# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKOCCGraphAdaptor
    VERSION 1.0.1
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

# 手动添加文件列表
set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCCommons.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphAdaptorAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAIS_ObjectBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObject3D.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectTopoBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectToolBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDimensionToolBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectPickToolBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCAIS_ColoredShape.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCAIS_Datum.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCMeshVS_Mesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDatumPoint.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDatumLine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDatumPlane.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectSketchCommand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectSketchShape.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCUnstructuredMeshVS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCStructuredMeshVS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectModelBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectModelPreviewBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectModelCommand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectModelCommandPreview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectModelShape.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectModelShapePreview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectTopoShape.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectTopoDirection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectTopoIndexLabels.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectTopoIndexLabels.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDimensionDistance.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDimensionAngle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDimensionArea.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDimensionVolume.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectPickDirection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectPickProjPos.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectGlobalComponent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorDatumPoint.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorDatumLine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorDatumPlane.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorSketchCommand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorSketchShape.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorModelCommand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorModelCommandPreview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorModelShape.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorModelShapePreview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorTopoShape.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorTopoDirection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorTopoIndexLabels.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorDimensionTool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorPickTool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorGlobalComponent.h
    )

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCCommons.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAIS_ObjectBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObject3D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectTopoBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectToolBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDimensionToolBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectPickToolBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCAIS_ColoredShape.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCAIS_Datum.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCMeshVS_Mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectMesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDatumPoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDatumLine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDatumPlane.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectSketchCommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectSketchShape.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCUnstructuredMeshVS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCStructuredMeshVS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectModelBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectModelPreviewBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectModelCommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectModelCommandPreview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectModelShape.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectModelShapePreview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectTopoShape.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectTopoDirection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectTopoIndexLabels.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDimensionDistance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDimensionAngle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDimensionArea.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectDimensionVolume.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectPickDirection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectPickProjPos.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGraphObjectGlobalComponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorDatumPoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorDatumLine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorDatumPlane.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorSketchCommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorSketchShape.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorModelCommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorModelCommandPreview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorModelShape.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorModelShapePreview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorMesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorTopoShape.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorTopoDirection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorTopoIndexLabels.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorDimensionTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorPickTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCViewAdaptorGlobalComponent.cpp
)

set(UI_FILES_LIST
)

set(RESOURCES_SOURCES
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/License.txt
)

source_group("Ui Files" FILES ${UI_FILES_LIST})
source_group("Resource Files" FILES ${RESOURCES_SOURCES})
source_group("Translation Files" FILES ${TS_FILES_LIST})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${UI_FILES_LIST}
    ${RESOURCES_SOURCES}
	${TS_FILES}
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

# OCC
if(NOT TARGET OCC::OCC)
    set(TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Tools")
    include(${CMAKE_CURRENT_SOURCE_DIR}/OCCConfig.cmake)
endif()

# 链接
target_link_libraries(${PROJECT_NAME}
    PUBLIC 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
        OCC::OCC
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAdaptor)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKGeoCompOCC)
link_fitk_library(${PROJECT_NAME} FITKRenderWindowOCC)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKOCCGRAPHADAPTOR_API
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
