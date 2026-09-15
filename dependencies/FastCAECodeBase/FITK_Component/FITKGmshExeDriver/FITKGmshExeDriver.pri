HEADERS += \ 
    $$PWD/FITKGmshExeDriverAPI.h \
    $$PWD/FITKGmshExecProgramDriver.h \
    $$PWD/FITKGmshGeoScriptWrite.h \
    $$PWD/FITKGmshExecProgramInputInfo.h \
    $$PWD/FITKGmshGeoScriptManuallyWrite.h \
    $$PWD/FITKMeshAlgorithmGeneratorGmshExec.h \
    $$PWD/FITKMesherDriverGmshExec.h \
    $$PWD/FITKMeshGenerateProcessorGmshExec.h \
    $$PWD/FITKMeshGenGmshExecInterface.h \
    $$PWD/FITKMeshSizeGeneratorGmshExec.h \
    $$PWD/GUIGmshSettings.h \
    $$PWD/GUIRegionMeshSizeBoxDialog.h \
    $$PWD/GUIRegionMeshSizeCylinderDialog.h \
    $$PWD/GUIRegionMeshSizeSphereDialog.h \
    $$PWD/GUIRegionMeshInfoDialog.h \

SOURCES += \
    $$PWD/FITKGmshExecProgramDriver.cpp \
    $$PWD/FITKGmshGeoScriptWrite.cpp \
    $$PWD/FITKGmshExecProgramInputInfo.cpp \
    $$PWD/FITKGmshGeoScriptManuallyWrite.cpp \
    $$PWD/FITKMeshAlgorithmGeneratorGmshExec.cpp \
    $$PWD/FITKMesherDriverGmshExec.cpp \
    $$PWD/FITKMeshGenerateProcessorGmshExec.cpp \
    $$PWD/FITKMeshGenGmshExecInterface.cpp \
    $$PWD/FITKMeshSizeGeneratorGmshExec.cpp \
    $$PWD/GUIGmshSettings.cpp \
    $$PWD/GUIRegionMeshSizeBoxDialog.cpp \
    $$PWD/GUIRegionMeshSizeCylinderDialog.cpp \
    $$PWD/GUIRegionMeshSizeSphereDialog.cpp \
    $$PWD/GUIRegionMeshInfoDialog.cpp \

FORMS += \
    $$PWD/GUIGmshSettings.ui \
    $$PWD/GUIRegionMeshSizeBoxDialog.ui \
    $$PWD/GUIRegionMeshSizeCylinderDialog.ui \
    $$PWD/GUIRegionMeshSizeSphereDialog.ui \
    $$PWD/GUIRegionMeshInfoDialog.ui \

RESOURCES += ./gmsh.qrc
