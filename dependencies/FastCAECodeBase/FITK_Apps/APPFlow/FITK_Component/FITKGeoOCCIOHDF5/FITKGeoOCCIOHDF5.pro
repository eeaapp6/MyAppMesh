TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKGeoOCCIOHDF5
QT         +=  core widgets gui
DEFINES    +=  FITKGEOOCCIOHDF5_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./occ.pri)
include(./hdf5.pri)
include(./FITKGeoOCCIOHDF5.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ 

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKGeoOCCIOHDF5/release/moc
    Release:RCC_DIR         = ../../generate/FITKGeoOCCIOHDF5/release/rcc
    Release:UI_DIR          = ../../generate/FITKGeoOCCIOHDF5/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKGeoOCCIOHDF5/release/obj
    Release:LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKAppFramework \
        -lFITKInterfaceIO \
        -lFITKInterfaceGeoIOHDF5 \
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKGeoOCCIOHDF5/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKGeoOCCIOHDF5/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKGeoOCCIOHDF5/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKGeoOCCIOHDF5/debug/obj
    Debug:LIBS += \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKAppFramework \
        -lFITKInterfaceIO \
        -lFITKInterfaceGeoIOHDF5 \
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        

    message("Windows FITKGeoOCCIOHDF5 generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ 

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKGeoOCCIOHDF5/release/moc
    UI_DIR          = ../../generate/FITKGeoOCCIOHDF5/release/qui
    RCC_DIR         = ../../generate/FITKGeoOCCIOHDF5/release/rcc
    OBJECTS_DIR     = ../../generate/FITKGeoOCCIOHDF5/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKAppFramework \
        -lFITKInterfaceIO \
        -lFITKInterfaceGeoIOHDF5 \
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        

    message("Linux FITKGeoOCCIOHDF5 generated")
}

