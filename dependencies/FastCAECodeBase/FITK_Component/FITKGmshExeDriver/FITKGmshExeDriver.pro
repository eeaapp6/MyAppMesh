TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKGmshExeDriver
QT         +=  core widgets gui
DEFINES    +=  FITKGmshExeDriver_API


unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKGmshExeDriver.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKGmshExeDriver/release/moc
    Release:RCC_DIR         = ../../generate/FITKGmshExeDriver/release/rcc
    Release:UI_DIR          = ../../generate/FITKGmshExeDriver/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKGmshExeDriver/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceModel \
        -lFITKInterfaceIO \
        -lFITKGmshMshIO \
        -lFITKWidget \
        -lFITKInterfaceGeometry \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKGmshExeDriver/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKGmshExeDriver/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKGmshExeDriver/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKGmshExeDriver/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceModel \
        -lFITKInterfaceIO \
        -lFITKGmshMshIO \
        -lFITKWidget \
        -lFITKInterfaceGeometry \

    message("Windows FITKGmshExeDriver generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKGmshExeDriver/release/moc
    UI_DIR          = ../../generate/FITKGmshExeDriver/release/qui
    RCC_DIR         = ../../generate/FITKGmshExeDriver/release/rcc
    OBJECTS_DIR     = ../../generate/FITKGmshExeDriver/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceModel \
        -lFITKInterfaceIO \
        -lFITKGmshMshIO \
        -lFITKWidget \
        -lFITKInterfaceGeometry \

    message("Linux FITKGmshExeDriver generated")
}

