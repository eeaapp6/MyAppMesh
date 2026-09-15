TEMPLATE    =   lib
CONFIG      +=  c++11 qt
TARGET      =   FITKWidget
QT          +=  core gui widgets designer
DEFINES     +=  FITKWidget_API

win32{
    QMAKE_CXXFLAGS += /MP
}

INCLUDEPATH +=  ./ \
                ../ \

include(FITKWidget.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKWidget/release/moc
    Release:RCC_DIR         = ../../generate/FITKWidget/release/rcc
    Release:UI_DIR          = ../../generate/FITKWidget/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKWidget/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
#       -lFITKEasyParam \


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKWidget/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKWidget/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKWidget/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKWidget/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
#       -lFITKEasyParam \
     

    message("Windows FITKWidget generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKWidget/release/moc
    UI_DIR          = ../../generate/FITKWidget/release/qui
    RCC_DIR         = ../../generate/FITKWidget/release/rcc
    OBJECTS_DIR     = ../../generate/FITKWidget/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
#       -lFITKEasyParam \
       

    message("Linux FITKWidget generated")
}
