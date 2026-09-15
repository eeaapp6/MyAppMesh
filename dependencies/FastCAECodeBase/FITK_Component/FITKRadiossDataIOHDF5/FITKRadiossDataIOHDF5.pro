TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKRadiossDataIOHDF5
QT         +=  core widgets gui
DEFINES    +=  FITKRadiossDataIOHDF5_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./hdf5.pri)
include(./FITKRadiossDataIOHDF5.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ 

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKRadiossDataIOHDF5/release/moc
    Release:RCC_DIR         = ../../generate/FITKRadiossDataIOHDF5/release/rcc
    Release:UI_DIR          = ../../generate/FITKRadiossDataIOHDF5/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKRadiossDataIOHDF5/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKRadiossData \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfacePhysics \


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKRadiossDataIOHDF5/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKRadiossDataIOHDF5/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKRadiossDataIOHDF5/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKRadiossDataIOHDF5/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKRadiossData \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfacePhysics \


    message("Windows FITKRadiossDataIOHDF5 generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ 

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKRadiossDataIOHDF5/release/moc
    UI_DIR          = ../../generate/FITKRadiossDataIOHDF5/release/qui
    RCC_DIR         = ../../generate/FITKRadiossDataIOHDF5/release/rcc
    OBJECTS_DIR     = ../../generate/FITKRadiossDataIOHDF5/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKRadiossData \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfacePhysics \


    message("Linux FITKRadiossDataIOHDF5 generated")
}

