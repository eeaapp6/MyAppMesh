cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(GUIDialog
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_library(${PROJECT_NAME} SHARED)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIDialogAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ComponentInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Curve3DBSplineInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveBridgeCurveInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveCurveFromSurfaceInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveSplitterInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FeatureStiffenerDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/BoolOperatorInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ChamferInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FilletInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ReferenceLineInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ReferencePlaneInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ReferencePointInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidClosedSurfaceInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidMultiSectionInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceBridgeSurfaceInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceClosedWireInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceExtrudeInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceMultiSectionInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceOffsetInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceRevolInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceSurfaceFromSolidInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceSweepInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidExtrudeInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidRevolInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidSweepInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimitivesBoxInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimitivesCylinderInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimitivesSphereInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MergeEdgeDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DefeatureInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DeleteFloatingEdgeDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PointCoordinateInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PointCurveEndAndDistanceInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PointInterOfCurveAndSurfaceInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PointCurveEndInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PointInterTwoCurveInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PointInteractionInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TransformationInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PointPointProjOnCurveInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PointCurveAndRadioInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PointPointProjOnSurfaceInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveCurveProjSurfaceInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveThreePointsEllipseInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveOffsetCurveInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveThreePointsHyperbolaInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveSurfaceInterSurfaceInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveThreePointsParabolaInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveThreePointsCircleInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveTwoPointsSegmentInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FillFaceInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SplitEdgeMidpointDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ImprintSolidDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AppendCompoundDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RemoveCompoundDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceSplitterInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidSplitterInfoDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FaceHiddenDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/NBodyUniteInfoDialog.h
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/BoolOperatorInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ChamferInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ComponentInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Curve3DBSplineInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveBridgeCurveInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveCurveFromSurfaceInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveSplitterInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FeatureStiffenerDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FilletInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ReferenceLineInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ReferencePlaneInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ReferencePointInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidClosedSurfaceInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidMultiSectionInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceBridgeSurfaceInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceClosedWireInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceExtrudeInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceMultiSectionInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceOffsetInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceRevolInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceSurfaceFromSolidInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceSweepInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidExtrudeInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidRevolInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidSweepInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimitivesBoxInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimitivesCylinderInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimitivesSphereInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MergeEdgeDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DefeatureInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DeleteFloatingEdgeDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PointCoordinateInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PointCurveEndAndDistanceInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PointInterOfCurveAndSurfaceInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PointCurveEndInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PointInterTwoCurveInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TransformationInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PointPointProjOnCurveInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PointCurveAndRadioInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PointPointProjOnSurfaceInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PointInteractionInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveCurveProjSurfaceInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveThreePointsEllipseInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveOffsetCurveInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveThreePointsHyperbolaInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveSurfaceInterSurfaceInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveThreePointsParabolaInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveThreePointsCircleInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveTwoPointsSegmentInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FillFaceInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SplitEdgeMidpointDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ImprintSolidDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AppendCompoundDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RemoveCompoundDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceSplitterInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidSplitterInfoDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FaceHiddenDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NBodyUniteInfoDialog.cpp
)

set(UI_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/BoolOperatorInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ChamferInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ComponentInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/Curve3DBSplineInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveBridgeCurveInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveCurveFromSurfaceInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveSplitterInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/FeatureStiffenerDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/FilletInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ReferenceLineInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ReferencePlaneInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ReferencePointInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidClosedSurfaceInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidMultiSectionInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceBridgeSurfaceInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceClosedWireInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceExtrudeInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceMultiSectionInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceOffsetInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceRevolInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceSurfaceFromSolidInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceSweepInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidExtrudeInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidRevolInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidSweepInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimitivesBoxInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimitivesCylinderInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimitivesSphereInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/MergeEdgeDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/DefeatureInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/DeleteFloatingEdgeDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/PointCoordinateInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/PointCurveEndAndDistanceInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/PointInterOfCurveAndSurfaceInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/PointCurveEndInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/PointInterTwoCurveInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/TransformationInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/PointPointProjOnCurveInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/PointCurveAndRadioInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/PointPointProjOnSurfaceInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveCurveProjSurfaceInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveThreePointsEllipseInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveOffsetCurveInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveThreePointsHyperbolaInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveSurfaceInterSurfaceInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveThreePointsParabolaInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveThreePointsCircleInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/CurveTwoPointsSegmentInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/FillFaceInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SplitEdgeMidpointDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ImprintSolidDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/AppendCompoundDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/RemoveCompoundDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SurfaceSplitterInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SolidSplitterInfoDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/FaceHiddenDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/NBodyUniteInfoDialog.ui
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resource/translations/Translate_GUIDialog_EN.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resource/translations/Translate_GUIDialog_CN.ts
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

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
)

link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKPython)
link_fitk_library(${PROJECT_NAME} FITKEasyParam)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} GUIWidget)
link_fitk_library(${PROJECT_NAME} GUIFrame)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} PickDataProvider)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        GUIDialog_API
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
