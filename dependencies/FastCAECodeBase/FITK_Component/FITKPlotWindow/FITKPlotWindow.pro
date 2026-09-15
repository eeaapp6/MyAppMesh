TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKPlotWindow
QT         +=  core widgets gui
DEFINES    +=  FITKPlotWindow_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKPlotWindow.pri)
include(./qwt.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKPlotWindow/release/moc
    Release:RCC_DIR         = ../../generate/FITKPlotWindow/release/rcc
    Release:UI_DIR          = ../../generate/FITKPlotWindow/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKPlotWindow/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKPlotWindow/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKPlotWindow/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKPlotWindow/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKPlotWindow/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKAppFramework \


    message("Windows FITKPlotWindow generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKPlotWindow/release/moc
    UI_DIR          = ../../generate/FITKPlotWindow/release/qui
    RCC_DIR         = ../../generate/FITKPlotWindow/release/rcc
    OBJECTS_DIR     = ../../generate/FITKPlotWindow/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAppFramework \

    message("Linux FITKPlotWindow generated")
}

