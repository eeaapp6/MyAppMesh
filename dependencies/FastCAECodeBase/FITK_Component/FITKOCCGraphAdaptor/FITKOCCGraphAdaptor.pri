# Base
#@{
HEADERS += \
    $$PWD/FITKOCCCommons.h \
    $$PWD/FITKOCCGraphAdaptorAPI.h \ 
    $$PWD/FITKAIS_ObjectBase.h \
    $$PWD/FITKOCCGraphObject3D.h \
    $$PWD/FITKOCCGraphObjectTopoBase.h \
    $$PWD/FITKOCCGraphObjectToolBase.h \
    $$PWD/FITKOCCGraphObjectDimensionToolBase.h \
    $$PWD/FITKOCCGraphObjectPickToolBase.h \
    $$PWD/FITKOCCViewAdaptorBase.h
    
SOURCES += \
    $$PWD/FITKOCCCommons.cpp \
    $$PWD/FITKAIS_ObjectBase.cpp \
    $$PWD/FITKOCCGraphObject3D.cpp \
    $$PWD/FITKOCCGraphObjectTopoBase.cpp \
    $$PWD/FITKOCCGraphObjectToolBase.cpp \
    $$PWD/FITKOCCGraphObjectDimensionToolBase.cpp \
    $$PWD/FITKOCCGraphObjectPickToolBase.cpp \
    $$PWD/FITKOCCViewAdaptorBase.cpp
    
#@}

# Preprocess 
#@{
HEADERS += \ 
    $$PWD/FITKOCCAIS_ColoredShape.h \
    $$PWD/FITKOCCAIS_Datum.h \
    $$PWD/FITKOCCMeshVS_Mesh.h \
    $$PWD/FITKOCCGraphObjectMesh.h \
    $$PWD/FITKOCCGraphObjectDatumPoint.h \
    $$PWD/FITKOCCGraphObjectDatumLine.h \
    $$PWD/FITKOCCGraphObjectDatumPlane.h \
    $$PWD/FITKOCCGraphObjectSketchCommand.h \
    $$PWD/FITKOCCGraphObjectSketchShape.h \
    $$PWD/FITKOCCUnstructuredMeshVS.h \
    $$PWD/FITKOCCStructuredMeshVS.h \
    $$PWD/FITKOCCGraphObjectModelBase.h \
    $$PWD/FITKOCCGraphObjectModelPreviewBase.h \
    $$PWD/FITKOCCGraphObjectModelCommand.h \
    $$PWD/FITKOCCGraphObjectModelCommandPreview.h \
    $$PWD/FITKOCCGraphObjectModelShape.h \
    $$PWD/FITKOCCGraphObjectModelShapePreview.h \
    $$PWD/FITKOCCGraphObjectTopoShape.h \
    $$PWD/FITKOCCGraphObjectTopoDirection.h \
    $$PWD/FITKOCCGraphObjectTopoIndexLabels.h \
    $$PWD/FITKOCCGraphObjectTopoIndexLabels.h \
    $$PWD/FITKOCCGraphObjectDimensionDistance.h \
    $$PWD/FITKOCCGraphObjectDimensionAngle.h \
    $$PWD/FITKOCCGraphObjectDimensionArea.h \
    $$PWD/FITKOCCGraphObjectDimensionVolume.h \
    $$PWD/FITKOCCGraphObjectPickDirection.h \
    $$PWD/FITKOCCGraphObjectPickProjPos.h \
    $$PWD/FITKOCCGraphObjectGlobalComponent.h \
    $$PWD/FITKOCCViewAdaptorDatumPoint.h \
    $$PWD/FITKOCCViewAdaptorDatumLine.h \
    $$PWD/FITKOCCViewAdaptorDatumPlane.h \
    $$PWD/FITKOCCViewAdaptorSketchCommand.h \
    $$PWD/FITKOCCViewAdaptorSketchShape.h \
    $$PWD/FITKOCCViewAdaptorModelCommand.h \
    $$PWD/FITKOCCViewAdaptorModelCommandPreview.h \
    $$PWD/FITKOCCViewAdaptorModelShape.h \
    $$PWD/FITKOCCViewAdaptorModelShapePreview.h \
    $$PWD/FITKOCCViewAdaptorMesh.h \
    $$PWD/FITKOCCViewAdaptorTopoShape.h \
    $$PWD/FITKOCCViewAdaptorTopoDirection.h \
    $$PWD/FITKOCCViewAdaptorTopoIndexLabels.h \
    $$PWD/FITKOCCViewAdaptorDimensionTool.h \
    $$PWD/FITKOCCViewAdaptorPickTool.h \
    $$PWD/FITKOCCViewAdaptorGlobalComponent.h
    
SOURCES += \ 
    $$PWD/FITKOCCAIS_ColoredShape.cpp \
    $$PWD/FITKOCCAIS_Datum.cpp \
    $$PWD/FITKOCCMeshVS_Mesh.cpp \
    $$PWD/FITKOCCGraphObjectMesh.cpp \
    $$PWD/FITKOCCGraphObjectDatumPoint.cpp \
    $$PWD/FITKOCCGraphObjectDatumLine.cpp \
    $$PWD/FITKOCCGraphObjectDatumPlane.cpp \
    $$PWD/FITKOCCGraphObjectSketchCommand.cpp \
    $$PWD/FITKOCCGraphObjectSketchShape.cpp \
    $$PWD/FITKOCCUnstructuredMeshVS.cpp \
    $$PWD/FITKOCCStructuredMeshVS.cpp \
    $$PWD/FITKOCCGraphObjectModelBase.cpp \
    $$PWD/FITKOCCGraphObjectModelPreviewBase.cpp \
    $$PWD/FITKOCCGraphObjectModelCommand.cpp \
    $$PWD/FITKOCCGraphObjectModelCommandPreview.cpp \
    $$PWD/FITKOCCGraphObjectModelShape.cpp \
    $$PWD/FITKOCCGraphObjectModelShapePreview.cpp \
    $$PWD/FITKOCCGraphObjectTopoShape.cpp \
    $$PWD/FITKOCCGraphObjectTopoDirection.cpp \
    $$PWD/FITKOCCGraphObjectTopoIndexLabels.cpp \
    $$PWD/FITKOCCGraphObjectDimensionDistance.cpp \
    $$PWD/FITKOCCGraphObjectDimensionAngle.cpp \
    $$PWD/FITKOCCGraphObjectDimensionArea.cpp \
    $$PWD/FITKOCCGraphObjectDimensionVolume.cpp \
    $$PWD/FITKOCCGraphObjectPickDirection.cpp \
    $$PWD/FITKOCCGraphObjectPickProjPos.cpp \
    $$PWD/FITKOCCGraphObjectGlobalComponent.cpp \
    $$PWD/FITKOCCViewAdaptorDatumPoint.cpp \
    $$PWD/FITKOCCViewAdaptorDatumLine.cpp \
    $$PWD/FITKOCCViewAdaptorDatumPlane.cpp \
    $$PWD/FITKOCCViewAdaptorSketchCommand.cpp \
    $$PWD/FITKOCCViewAdaptorSketchShape.cpp \
    $$PWD/FITKOCCViewAdaptorModelCommand.cpp \
    $$PWD/FITKOCCViewAdaptorModelCommandPreview.cpp \
    $$PWD/FITKOCCViewAdaptorModelShape.cpp \
    $$PWD/FITKOCCViewAdaptorModelShapePreview.cpp \
    $$PWD/FITKOCCViewAdaptorMesh.cpp \
    $$PWD/FITKOCCViewAdaptorTopoShape.cpp \
    $$PWD/FITKOCCViewAdaptorTopoDirection.cpp \
    $$PWD/FITKOCCViewAdaptorTopoIndexLabels.cpp \
    $$PWD/FITKOCCViewAdaptorDimensionTool.cpp \
    $$PWD/FITKOCCViewAdaptorPickTool.cpp \
    $$PWD/FITKOCCViewAdaptorGlobalComponent.cpp
    
#@}

DISTFILES += \
    $$PWD/License.txt \
   