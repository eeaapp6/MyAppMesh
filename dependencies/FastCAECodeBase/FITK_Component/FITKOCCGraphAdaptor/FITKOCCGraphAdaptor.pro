TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKOCCGraphAdaptor
QT         +=  core widgets gui
DEFINES    +=  FITKOCCGRAPHADAPTOR_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./occ.pri)
include(./FITKOCCGraphAdaptor.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKOCCGraphAdaptor/release/moc
    Release:RCC_DIR         = ../../generate/FITKOCCGraphAdaptor/release/rcc
    Release:UI_DIR          = ../../generate/FITKOCCGraphAdaptor/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKOCCGraphAdaptor/release/obj
    Release:LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        -lFITKRenderWindowOCC 
        

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKOCCGraphAdaptor/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKOCCGraphAdaptor/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKOCCGraphAdaptor/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKOCCGraphAdaptor/debug/obj
    Debug:LIBS += \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        -lFITKRenderWindowOCC 

    message("Windows FITKOCCGraphAdptor generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKOCCGraphAdaptor/release/moc
    UI_DIR          = ../../generate/FITKOCCGraphAdaptor/release/qui
    RCC_DIR         = ../../generate/FITKOCCGraphAdaptor/release/rcc
    OBJECTS_DIR     = ../../generate/FITKOCCGraphAdaptor/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        -lFITKRenderWindowOCC 

    message("Linux FITKOCCGraphAdptor generated")
}

