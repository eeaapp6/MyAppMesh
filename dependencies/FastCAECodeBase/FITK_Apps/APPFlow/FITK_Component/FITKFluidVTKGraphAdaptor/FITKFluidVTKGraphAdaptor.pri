# Base
#@{
HEADERS += \
    $$PWD/FITKFluidVTKCommons.h \
    $$PWD/FITKFluidVTKGraphAdaptorAPI.h \ 
    $$PWD/FITKFluidVTKGraphObject3D.h \
    $$PWD/FITKFluidVTKGraphObjectModelBase.h \
    $$PWD/FITKFluidVTKGraphObjectFixedSizeBase.h \
    $$PWD/FITKFluidVTKViewAdaptorBase.h 
    
SOURCES += \
    $$PWD/FITKFluidVTKCommons.cpp \
    $$PWD/FITKFluidVTKGraphObject3D.cpp \
    $$PWD/FITKFluidVTKGraphObjectModelBase.cpp \
    $$PWD/FITKFluidVTKGraphObjectFixedSizeBase.cpp \
    $$PWD/FITKFluidVTKViewAdaptorBase.cpp
    
#@}

# Preprocess 
#@{
HEADERS += \ 
    $$PWD/FITKFluidVTKShapeColorMap.h \
    $$PWD/FITKFluidVTKGraphObjectModelCmd.h \
    $$PWD/FITKFluidVTKGraphObjectModelShape.h \
    $$PWD/FITKFluidVTKGraphObjectBoundMesh.h \
    $$PWD/FITKFluidVTKGraphObjectSelect.h \
    $$PWD/FITKFluidVTKGraphObjectPick.h \
    $$PWD/FITKFluidVTKGraphObjectPickPreview.h \
    $$PWD/FITKFluidVTKGraphObjectRegionMesh.h \
    $$PWD/FITKFluidVTKGraphObjectMeasureTools.h \
    $$PWD/FITKFluidVTKGraphObjectMaterialPoints.h \
    $$PWD/FITKFluidVTKViewAdaptorModelCmd.h \
    $$PWD/FITKFluidVTKViewAdaptorModelShape.h \
    $$PWD/FITKFluidVTKViewAdaptorBoundMesh.h \
    $$PWD/FITKFluidVTKViewAdaptorRegionMesh.h \
    $$PWD/FITKFluidVTKViewAdaptorMaterialPoints.h
    
SOURCES += \ 
    $$PWD/FITKFluidVTKShapeColorMap.cpp \
    $$PWD/FITKFluidVTKGraphObjectModelCmd.cpp \
    $$PWD/FITKFluidVTKGraphObjectModelShape.cpp \
    $$PWD/FITKFluidVTKGraphObjectBoundMesh.cpp \
    $$PWD/FITKFluidVTKGraphObjectSelect.cpp \
    $$PWD/FITKFluidVTKGraphObjectPick.cpp \
    $$PWD/FITKFluidVTKGraphObjectPickPreview.cpp \
    $$PWD/FITKFluidVTKGraphObjectRegionMesh.cpp \
    $$PWD/FITKFluidVTKGraphObjectMeasureTools.cpp \
    $$PWD/FITKFluidVTKGraphObjectMaterialPoints.cpp \
    $$PWD/FITKFluidVTKViewAdaptorModelCmd.cpp \
    $$PWD/FITKFluidVTKViewAdaptorModelShape.cpp \
    $$PWD/FITKFluidVTKViewAdaptorBoundMesh.cpp \
    $$PWD/FITKFluidVTKViewAdaptorRegionMesh.cpp \
    $$PWD/FITKFluidVTKViewAdaptorMaterialPoints.cpp
    
#@}

DISTFILES += \
    $$PWD/License.txt \
    