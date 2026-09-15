TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKInterfaceGeometry
QT         +=  core widgets gui
DEFINES    +=  FITKInterfaceGeometry_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKInterfaceGeometry.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKInterfaceGeometry/release/moc
    Release:RCC_DIR         = ../../generate/FITKInterfaceGeometry/release/rcc
    Release:UI_DIR          = ../../generate/FITKInterfaceGeometry/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKInterfaceGeometry/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKInterfaceModel \
        -lFITKAppFramework \


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKInterfaceGeometry/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKInterfaceGeometry/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKInterfaceGeometry/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKInterfaceGeometry/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKInterfaceModel \
        -lFITKAppFramework \


    message("Windows FITKInterfaceGeometry generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKInterfaceGeometry/release/moc
    UI_DIR          = ../../generate/FITKInterfaceGeometry/release/qui
    RCC_DIR         = ../../generate/FITKInterfaceGeometry/release/rcc
    OBJECTS_DIR     = ../../generate/FITKInterfaceGeometry/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKInterfaceModel \
        -lFITKAppFramework \

    message("Linux FITKInterfaceGeometry generated")
}

