TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKInterfaceFlowPHengLEI
QT         +=  core widgets gui
DEFINES    +=  FITKInterfaceFlowPHengLEI_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKInterfaceFlowPHengLEI.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKInterfaceFlowPHengLEI/release/moc
    Release:RCC_DIR         = ../../generate/FITKInterfaceFlowPHengLEI/release/rcc
    Release:UI_DIR          = ../../generate/FITKInterfaceFlowPHengLEI/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKInterfaceFlowPHengLEI/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKEasyParam \
        -lFITKInterfaceIO \
        -lFITKInterfaceMesh
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKInterfaceFlowPHengLEI/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKInterfaceFlowPHengLEI/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKInterfaceFlowPHengLEI/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKInterfaceFlowPHengLEI/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKEasyParam \
        -lFITKInterfaceIO \
        -lFITKInterfaceMesh

    message("Windows FITKInterfaceFlowPHengLEI generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                       ../../

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKInterfaceFlowPHengLEI/release/moc
    UI_DIR          = ../../generate/FITKInterfaceFlowPHengLEI/release/qui
    RCC_DIR         = ../../generate/FITKInterfaceFlowPHengLEI/release/rcc
    OBJECTS_DIR     = ../../generate/FITKInterfaceFlowPHengLEI/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKEasyParam \
        -lFITKInterfaceIO \
        -lFITKInterfaceMesh

    message("Linux FITKInterfaceFlowPHengLEI generated")
}

