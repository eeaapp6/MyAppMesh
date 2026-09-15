TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKMeshGenOF
QT         +=  core widgets gui
DEFINES    +=  FITKMeshGenOF_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKMeshGenOF.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKMeshGenOF/release/moc
    Release:RCC_DIR         = ../../generate/FITKMeshGenOF/release/rcc
    Release:UI_DIR          = ../../generate/FITKMeshGenOF/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKMeshGenOF/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceMeshGen \
        -lFITKOFDriver \
        -lFITKInterfaceIO \
        -lFITKInterfaceMesh \
        -lFITKOFMeshIO \
        -lFITKGeoCompOCC \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKOFDictWriter \
        -lFITKOFMeshIO \
        -lFITKInterfaceFlowOF \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKMeshGenOF/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKMeshGenOF/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKMeshGenOF/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKMeshGenOF/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceMeshGen \
        -lFITKOFDriver \
        -lFITKInterfaceIO \
        -lFITKInterfaceMesh \
        -lFITKOFMeshIO \
        -lFITKGeoCompOCC \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKOFDictWriter \
        -lFITKOFMeshIO \
        -lFITKInterfaceFlowOF \

    message("Windows FITKMeshGenOF generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKMeshGenOF/release/moc
    UI_DIR          = ../../generate/FITKMeshGenOF/release/qui
    RCC_DIR         = ../../generate/FITKMeshGenOF/release/rcc
    OBJECTS_DIR     = ../../generate/FITKMeshGenOF/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceMeshGen \
        -lFITKOFDriver \
        -lFITKInterfaceIO \
        -lFITKInterfaceMesh \
        -lFITKOFMeshIO \
        -lFITKGeoCompOCC \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKOFDictWriter \
        -lFITKOFMeshIO \
        -lFITKInterfaceFlowOF \

    message("Linux FITKMeshGenOF generated")
}

