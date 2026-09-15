# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKInterfaceGeometry
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
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoFeature.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoSplitter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoTransformation.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInterfaceGeometryAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoEnum.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/PyFITKGeoBehavior.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/PyFITKSketch2D.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoCommand.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoCommandProp.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoDelete.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoInterfaceFactory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoShapeAgent.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoCommandList.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoModelManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoDatum.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoSketch2D.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelBox.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelCylinder.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelCone.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelSphere.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFrustum.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelImport.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelExport.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoOperBool.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeometryMeshVS.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeometryMeshEntity.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsVirtualTopo.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKVirtualTopoManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsVirtualTopoMapper.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeomTools.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelPoint.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelCurve.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelSurface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelSolid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFilledRectangle.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFilledEllipse.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFilledPolygon.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFilledParaboloid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelCut.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelOperLine.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelOperFace.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoRemoveWireEdges.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelOperSolid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoReferencePoint.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoReferenceLine.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoReferencePlane.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelOperImprintSolid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelOperCompound.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoMultiDelete.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoPart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoInstance.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGlobalGeoComponent.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoImportedPart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKImportFileThread.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsConstruction.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKExportFileThread.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoImportBase.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoStitch.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoSampleCurve.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoCoverEdges.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelNearestPointLine.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelExtractCenter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFromMesh.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoRepairSmallEdges.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoRepairSmallFaces.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelImportFlattenAssembly.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelPartitionEdge.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelPartitionFace.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelPartitionSolid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoSketchFillet.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoSketchDeleter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsExtendTool.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoReplaceFaces.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoOperNBodyUnite.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoVirtualTopoCombine.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelExportTopos.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoMergeEdges.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelMergePart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelTransformPart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelSplitPart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFromSketch.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelPlanarShell.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoOffsetFace.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelSolidFromShell.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelShellFromSolid.h 
)
set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoCommand.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoCommandProp.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoDelete.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoFeature.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoSplitter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/PyFITKGeoBehavior.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/PyFITKSketch2D.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoTransformation.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoInterfaceFactory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoShapeAgent.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoCommandList.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoModelManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoDatum.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoSketch2D.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelBox.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelCylinder.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelCone.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelSphere.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFrustum.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelImport.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelExport.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoOperBool.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeometryMeshVS.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeometryMeshEntity.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsVirtualTopo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKVirtualTopoManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsVirtualTopoMapper.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeomTools.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelPoint.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelCurve.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelSurface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelSolid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFilledRectangle.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFilledEllipse.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFilledPolygon.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFilledParaboloid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelCut.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelOperLine.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelOperFace.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoRemoveWireEdges.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelOperSolid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoReferencePoint.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoReferenceLine.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoReferencePlane.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelOperImprintSolid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelOperCompound.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoMultiDelete.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoPart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoInstance.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGlobalGeoComponent.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoImportedPart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKImportFileThread.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsConstruction.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKExportFileThread.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoImportBase.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoStitch.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoSampleCurve.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoCoverEdges.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelNearestPointLine.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelExtractCenter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFromMesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoRepairSmallEdges.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoRepairSmallFaces.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelImportFlattenAssembly.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelPartitionEdge.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelPartitionFace.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelPartitionSolid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoSketchFillet.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoSketchDeleter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoReplaceFaces.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoOperNBodyUnite.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoVirtualTopoCombine.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelExportTopos.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoMergeEdges.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoWireEdgeFromEdge.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelMergePart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelTransformPart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelSplitPart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelFromSketch.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelPlanarShell.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoOffsetFace.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelSolidFromShell.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsGeoModelShellFromSolid.cpp 
)

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
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

# 链接
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKInterfaceGeometry_API
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


