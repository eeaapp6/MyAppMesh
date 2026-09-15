TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKInterfaceFlowOF
QT         +=  core widgets gui
DEFINES    +=  FITKInterfaceFlowOF_API 

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKInterfaceFlowOF.pri)


win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKInterfaceFlowOF/release/moc
    Release:RCC_DIR         = ../../generate/FITKInterfaceFlowOF/release/rcc
    Release:UI_DIR          = ../../generate/FITKInterfaceFlowOF/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKInterfaceFlowOF/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKAppFramework \
        -lFITKInterfaceMesh \
        -lFITKEasyParam \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKInterfaceFlowOF/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKInterfaceFlowOF/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKInterfaceFlowOF/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKInterfaceFlowOF/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKAppFramework \
        -lFITKInterfaceMesh \
        -lFITKEasyParam \

    message("Windows FITKInterfaceFlowOF generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKInterfaceFlowOF/release/moc
    UI_DIR          = ../../generate/FITKInterfaceFlowOF/release/qui
    RCC_DIR         = ../../generate/FITKInterfaceFlowOF/release/rcc
    OBJECTS_DIR     = ../../generate/FITKInterfaceFlowOF/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKAppFramework \
        -lFITKInterfaceMesh \
        -lFITKEasyParam \

    message("Linux FITKInterfaceFlowOF generated")
}

