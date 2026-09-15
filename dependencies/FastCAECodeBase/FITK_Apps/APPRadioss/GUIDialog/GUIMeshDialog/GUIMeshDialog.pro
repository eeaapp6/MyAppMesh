TEMPLATE    =   lib
CONFIG      +=  c++11 qt
TARGET      =   GUIMeshDialog
QT          +=  core gui widgets designer
DEFINES     +=  GUIMeshDialog_API

win32{
    QMAKE_CXXFLAGS += /MP
}

INCLUDEPATH	+=  ./ \
                ../ \

include(GUIMeshDialog.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/GUIMeshDialog/release/moc
    Release:RCC_DIR         = ../../generate/GUIMeshDialog/release/rcc
    Release:UI_DIR          = ../../generate/GUIMeshDialog/release/qui
    Release:OBJECTS_DIR     = ../../generate/GUIMeshDialog/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKWidget \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKRadiossData \
        -lGUIWidget \
        -lPickDataProvider \
        -lOperatorsInterface \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/GUIMeshDialog/debug/moc
    Debug:RCC_DIR         = ../../generate/GUIMeshDialog/debug/rcc
    Debug:UI_DIR          = ../../generate/GUIMeshDialog/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/GUIMeshDialog/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKWidget \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKRadiossData \
        -lGUIWidget \
        -lPickDataProvider \
        -lOperatorsInterface \

    message("Windows GUIMeshDialog generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/GUIMeshDialog/release/moc
    UI_DIR          = ../../generate/GUIMeshDialog/release/qui
    RCC_DIR         = ../../generate/GUIMeshDialog/release/rcc
    OBJECTS_DIR     = ../../generate/GUIMeshDialog/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKWidget \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKRadiossData \
        -lGUIWidget \
        -lPickDataProvider \
        -lOperatorsInterface \

    message("Linux GUIMeshDialog generated")
}
