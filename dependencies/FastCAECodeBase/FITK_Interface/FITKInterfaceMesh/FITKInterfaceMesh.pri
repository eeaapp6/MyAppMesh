HEADERS += \ 
    $$PWD/FITKInterfaceMeshAPI.h \
    $$PWD/FITKMeshVTKMap.h \
    $$PWD/FITKUnstructuredMeshVTK.h \
    $$PWD/FITKMeshFeatureVTK.h \
    $$PWD/FITKStructuredMeshVTK.h \
    $$PWD/FITKUnstructuredFluidMeshVTK.h \
    $$PWD/FITKPartitionInterface.h \

SOURCES += \
    $$PWD/FITKInterfaceMeshAPI.cpp \
    $$PWD/FITKUnstructuredMeshVTK.cpp \
    $$PWD/FITKMeshFeatureVTK.cpp \
    $$PWD/FITKStructuredMeshVTK.cpp \
    $$PWD/FITKUnstructuredFluidMeshVTK.cpp \
    $$PWD/FITKPartitionInterface.cpp \
    
FORMS += \
#    $$PWD/PluginManageDialog.ui \
 
 
#RESOURCES += ../qrc/qianfan.qrc

#RC_FILE += ../qrc/qianfan.rc
 
 DISTFILES += \
    $$PWD/License.txt \
    