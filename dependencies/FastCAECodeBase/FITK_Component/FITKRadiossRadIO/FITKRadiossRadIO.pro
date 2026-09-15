TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKRadiossRadIO
QT         +=  core widgets gui
DEFINES    +=  FITKRadiossRadIO_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKRadiossRadIO.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../  \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKRadiossRadIO/release/moc
    Release:RCC_DIR         = ../../generate/FITKRadiossRadIO/release/rcc
    Release:UI_DIR          = ../../generate/FITKRadiossRadIO/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKRadiossRadIO/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceIO \
        -lFITKRadiossData \
        -lFITKInterfaceModel \
        -lFITKInterfacePhysics \
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKRadiossRadIO/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKRadiossRadIO/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKRadiossRadIO/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKRadiossRadIO/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceIO \
        -lFITKRadiossData \
        -lFITKInterfaceModel \
        -lFITKInterfacePhysics \
     


    message("Windows FITKRadiossRadIO generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../  \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKRadiossRadIO/release/moc
    UI_DIR          = ../../generate/FITKRadiossRadIO/release/qui
    RCC_DIR         = ../../generate/FITKRadiossRadIO/release/rcc
    OBJECTS_DIR     = ../../generate/FITKRadiossRadIO/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceIO \
        -lFITKRadiossData \
        -lFITKInterfaceModel \
        -lFITKInterfacePhysics \
       

    message("Linux FITKRadiossRadIO generated")
}

