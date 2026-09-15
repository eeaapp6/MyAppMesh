TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKAdaptor
QT         +=  core widgets gui
DEFINES    +=  FITKAdaptor_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKAdaptor.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../  \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKAdaptor/release/moc
    Release:RCC_DIR         = ../../generate/FITKAdaptor/release/rcc
    Release:UI_DIR          = ../../generate/FITKAdaptor/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKAdaptor/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKAdaptor/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKAdaptor/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKAdaptor/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKAdaptor/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
     


    message("Windows FITKAdaptor generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../  \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKAdaptor/release/moc
    UI_DIR          = ../../generate/FITKAdaptor/release/qui
    RCC_DIR         = ../../generate/FITKAdaptor/release/rcc
    OBJECTS_DIR     = ../../generate/FITKAdaptor/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
       

    message("Linux FITKAdaptor generated")
}

