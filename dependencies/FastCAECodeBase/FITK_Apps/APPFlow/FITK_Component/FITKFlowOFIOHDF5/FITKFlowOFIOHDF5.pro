TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKFlowOFIOHDF5
QT         +=  core widgets gui
DEFINES    +=  FITKFLOWOFIOHDF5_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKFlowOFIOHDF5.pri)
include(./hdf5.pri)
include(./occ.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKFlowOFIOHDF5/release/moc
    Release:RCC_DIR         = ../../generate/FITKFlowOFIOHDF5/release/rcc
    Release:UI_DIR          = ../../generate/FITKFlowOFIOHDF5/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKFlowOFIOHDF5/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKAppFramework \
        -lFITKInterfaceIO \
        -lFITKEasyParam \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceMesh \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceFlowOF \
        -lFITKInterfaceModel \
        -lFITKGeoOCCIOHDF5 \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKFlowOFIOHDF5/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKFlowOFIOHDF5/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKFlowOFIOHDF5/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKFlowOFIOHDF5/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKAppFramework \
        -lFITKInterfaceIO \
        -lFITKEasyParam \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceMesh \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceFlowOF \
        -lFITKInterfaceModel \
        -lFITKGeoOCCIOHDF5 \

    message("Windows FITKFlowOFIOHDF5 generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                       ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKFlowOFIOHDF5/release/moc
    UI_DIR          = ../../generate/FITKFlowOFIOHDF5/release/qui
    RCC_DIR         = ../../generate/FITKFlowOFIOHDF5/release/rcc
    OBJECTS_DIR     = ../../generate/FITKFlowOFIOHDF5/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKAppFramework \
        -lFITKInterfaceIO \
        -lFITKEasyParam \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceMesh \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceFlowOF \
        -lFITKInterfaceModel \
        -lFITKGeoOCCIOHDF5 \

    message("Linux FITKFlowOFIOHDF5 generated")
}

