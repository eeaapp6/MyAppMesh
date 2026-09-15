# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKGeoCompOCC
    VERSION 1.0.0
    LANGUAGES CXX
)

# Qt配置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 设置并查找Qt5组件

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# 定义为共享库
add_library(${PROJECT_NAME} SHARED)

# 手动添加文件列表
set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCExtendTool.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCFeature.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCReferenceLine.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCReferencePlane.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCReferencePoint.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelSketch2D.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoCompOCCAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoCompOCCInterface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractOCCModel.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelSimpleShape.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelImport.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelExport.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCComandCommon.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCOperBool.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelRefine.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCSplitter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCTransformation.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCVirtualTopoCreator.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCShapeTriangulate.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGeomTools.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelPoint.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelCurve.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelSurface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelSolid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelFilledRectangle.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelFilledEllipse.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelFilledPolygon.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelFilledParaboloid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelOperLine.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelOperFace.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelRemoveWireEdges.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelWireEdgeFromEdge.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelOperSolid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelOperImprintSolid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelOperCompound.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCDXFReader.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCSampleCurve.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelNearestPointLine.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelExtractCenter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelFromMesh.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelPart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelImportFlattenAssembly.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelImportedPart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCAlgTools.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCNBodyUnite.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelCut.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelPartitionEdge.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelPartitionFace.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelPartitionSolid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelStitch.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelCircularHole.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGeoSketchDeleter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelExportTopos.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKShapeFix_FixSmallFace.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelMergeEdges.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelMergePart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelTransformPart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelSplitPart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelFromSketch.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelPlanarShell.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelOffsetFace.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCFeature.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelSketch2D.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoCompOCCInterface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractOCCModel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelSimpleShape.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelImport.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelExport.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCComandCommon.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCOperBool.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelRefine.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCReferenceLine.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCReferencePlane.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCReferencePoint.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCSplitter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCTransformation.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCVirtualTopoCreator.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCShapeTriangulate.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGeomTools.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelPoint.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelCurve.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelSurface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelSolid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelFilledRectangle.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelFilledEllipse.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelFilledPolygon.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelFilledParaboloid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelOperLine.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelOperFace.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelRemoveWireEdges.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelWireEdgeFromEdge.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelOperSolid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelOperImprintSolid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelOperCompound.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCDXFReader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCSampleCurve.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelNearestPointLine.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelExtractCenter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelFromMesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelPart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelImportFlattenAssembly.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelImportedPart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCAlgTools.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCNBodyUnite.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelCut.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelPartitionEdge.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelPartitionFace.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelPartitionSolid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelStitch.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelCircularHole.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCGeoSketchDeleter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelExportTopos.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKShapeFix_FixSmallFace.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelMergeEdges.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelMergePart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelTransformPart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelSplitPart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelFromSketch.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelPlanarShell.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOCCModelOffsetFace.cpp 
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Resource/translations/Translate_FITKGeoCompOCC_EN.ts 
    ${CMAKE_CURRENT_SOURCE_DIR}/Resource/translations/Translate_FITKGeoCompOCC_CN.ts 
)

source_group("Translation Files" FILES ${TS_FILES_LIST})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*|.*autouic_.*\\.stamp$")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
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

# OCC
if(NOT TARGET OCC::OCC)
    include(${TOOLS_DIR}/cmake/OCCConfig.cmake)
endif()

# Dime
if(NOT TARGET DIME::DIME)
    include(${TOOLS_DIR}/cmake/DimeConfig.cmake)
endif()

# 链接
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
		OCC::OCC
		DIME::DIME
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKGeoCompOCC_API
)

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
