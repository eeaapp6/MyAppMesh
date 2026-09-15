TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKAppFramework
QT         +=  core widgets gui network
DEFINES    +=  FITKAppFramework_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKAppFramework.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKAppFramework/release/moc
    Release:RCC_DIR         = ../../generate/FITKAppFramework/release/rcc
    Release:UI_DIR          = ../../generate/FITKAppFramework/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKAppFramework/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lDbgHelp \


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKAppFramework/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKAppFramework/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKAppFramework/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKAppFramework/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lDbgHelp \


    message("Windows FITKAppFramework generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKAppFramework/release/moc
    UI_DIR          = ../../generate/FITKAppFramework/release/qui
    RCC_DIR         = ../../generate/FITKAppFramework/release/rcc
    OBJECTS_DIR     = ../../generate/FITKAppFramework/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
       

    message("Linux FITKAppFramework generated")
}

