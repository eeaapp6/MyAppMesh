TEMPLATE    =   lib
CONFIG      +=  c++11 qt
TARGET      =   GUIToolDialog
QT          +=  core gui widgets designer
DEFINES     +=  GUIToolDialog_API

win32{
    QMAKE_CXXFLAGS += /MP
}

INCLUDEPATH	+=  ./ \
                ../ \

include(GUIToolDialog.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/GUIToolDialog/release/moc
    Release:RCC_DIR         = ../../generate/GUIToolDialog/release/rcc
    Release:UI_DIR          = ../../generate/GUIToolDialog/release/qui
    Release:OBJECTS_DIR     = ../../generate/GUIToolDialog/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKWidget \
        -lGUIWidget \
        -lFITKRadiossData \
        -lFITKInterfacePhysics \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/GUIToolDialog/debug/moc
    Debug:RCC_DIR         = ../../generate/GUIToolDialog/debug/rcc
    Debug:UI_DIR          = ../../generate/GUIToolDialog/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/GUIToolDialog/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKWidget \
        -lGUIWidget \
        -lFITKRadiossData \
        -lFITKInterfacePhysics \

    message("Windows GUIToolDialog generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/GUIToolDialog/release/moc
    UI_DIR          = ../../generate/GUIToolDialog/release/qui
    RCC_DIR         = ../../generate/GUIToolDialog/release/rcc
    OBJECTS_DIR     = ../../generate/GUIToolDialog/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKWidget \
        -lGUIWidget \
        -lFITKRadiossData \
        -lFITKInterfacePhysics \

    message("Linux GUIToolDialog generated")
}
