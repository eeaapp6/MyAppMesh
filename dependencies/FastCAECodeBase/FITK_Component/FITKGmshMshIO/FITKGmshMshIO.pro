TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKGmshMshIO
QT         +=  core widgets gui
DEFINES    +=  FITKGmshMshIO_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKGmshMshIO.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKGmshMshIO/release/moc
    Release:RCC_DIR         = ../../generate/FITKGmshMshIO/release/rcc
    Release:UI_DIR          = ../../generate/FITKGmshMshIO/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKGmshMshIO/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKGmshMshIO/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKGmshMshIO/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKGmshMshIO/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKGmshMshIO/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \

    message("Windows FITKGmshMshIO generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKGmshMshIO/release/moc
    UI_DIR          = ../../generate/FITKGmshMshIO/release/qui
    RCC_DIR         = ../../generate/FITKGmshMshIO/release/rcc
    OBJECTS_DIR     = ../../generate/FITKGmshMshIO/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \
  

    message("Linux FITKGmshMshIO generated")
}

