TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKEasyParam
QT         +=  core widgets gui
DEFINES    +=  FITKEasyParam_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKEasyParam.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKEasyParam/release/moc
    Release:RCC_DIR         = ../../generate/FITKEasyParam/release/rcc
    Release:UI_DIR          = ../../generate/FITKEasyParam/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKEasyParam/release/obj
    Release:LIBS +=  \
        -L../../output/bin \
        -lFITKCore \
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKEasyParam/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKEasyParam/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKEasyParam/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKEasyParam/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
     


    message("Windows FITKEasyParam generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKEasyParam/release/moc
    UI_DIR          = ../../generate/FITKEasyParam/release/qui
    RCC_DIR         = ../../generate/FITKEasyParam/release/rcc
    OBJECTS_DIR     = ../../generate/FITKEasyParam/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
       

    message("Linux FITKEasyParam generated")
}

