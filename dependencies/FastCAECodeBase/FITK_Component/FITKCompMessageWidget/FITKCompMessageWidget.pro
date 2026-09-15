TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKCompMessageWidget
QT         +=  core widgets gui
DEFINES    +=  FITKCompMessageWidget_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKCompMessageWidget.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKCompMessageWidget/release/moc
    Release:RCC_DIR         = ../../generate/FITKCompMessageWidget/release/rcc
    Release:UI_DIR          = ../../generate/FITKCompMessageWidget/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKCompMessageWidget/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKCompMessageWidget/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKCompMessageWidget/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKCompMessageWidget/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKCompMessageWidget/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
     


    message("Windows FITKCompMessageWidget generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKCompMessageWidget/release/moc
    UI_DIR          = ../../generate/FITKCompMessageWidget/release/qui
    RCC_DIR         = ../../generate/FITKCompMessageWidget/release/rcc
    OBJECTS_DIR     = ../../generate/FITKCompMessageWidget/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
       

    message("Linux FITKCompMessageWidget generated")
}


RESOURCES += \
    $$PWD/Resource/translations.qrc 