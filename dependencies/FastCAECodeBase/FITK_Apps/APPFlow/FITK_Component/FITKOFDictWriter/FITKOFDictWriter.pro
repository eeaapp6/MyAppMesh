TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKOFDictWriter
QT         +=  core widgets gui
DEFINES    +=  FITKOFDictWriter_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKOFDictWriter.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKOFDictWriter/release/moc
    Release:RCC_DIR         = ../../generate/FITKOFDictWriter/release/rcc
    Release:UI_DIR          = ../../generate/FITKOFDictWriter/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKOFDictWriter/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceFlowOF \
        -lFITKInterfaceMesh \
        -lFITKEasyParam \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKOFDictWriter/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKOFDictWriter/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKOFDictWriter/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKOFDictWriter/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceFlowOF \
        -lFITKInterfaceMesh \
        -lFITKEasyParam \

    message("Windows FITKOFDictWriter generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKOFDictWriter/release/moc
    UI_DIR          = ../../generate/FITKOFDictWriter/release/qui
    RCC_DIR         = ../../generate/FITKOFDictWriter/release/rcc
    OBJECTS_DIR     = ../../generate/FITKOFDictWriter/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceMeshGen \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceFlowOF \
        -lFITKInterfaceMesh \
        -lFITKEasyParam \

    message("Linux FITKOFDictWriter generated")
}

