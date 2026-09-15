TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKInterfaceIOHDF5
QT         +=  core widgets gui
DEFINES    +=  FITKInterfaceIOHDF5_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKInterfaceIOHDF5.pri)
include(./hdf5.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKInterfaceIOHDF5/release/moc
    Release:RCC_DIR         = ../../generate/FITKInterfaceIOHDF5/release/rcc
    Release:UI_DIR          = ../../generate/FITKInterfaceIOHDF5/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKInterfaceIOHDF5/release/obj
    Release:LIBS +=  \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfacePhysics \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKInterfaceIOHDF5/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKInterfaceIOHDF5/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKInterfaceIOHDF5/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKInterfaceIOHDF5/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfacePhysics \

    message("Windows FITKInterfaceIOHDF5 generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKInterfaceIOHDF5/release/moc
    UI_DIR          = ../../generate/FITKInterfaceIOHDF5/release/qui
    RCC_DIR         = ../../generate/FITKInterfaceIOHDF5/release/rcc
    OBJECTS_DIR     = ../../generate/FITKInterfaceIOHDF5/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfacePhysics \

    message("Linux FITKInterfaceIOHDF5 generated")
}

