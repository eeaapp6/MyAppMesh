TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKCore
QT         +=  core gui widgets
DEFINES    +=  FITKCore_API
DEFINES    +=  FITKUseAppFramework

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKCore.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKCore/release/moc
    Release:RCC_DIR         = ../../generate/FITKCore/release/rcc
    Release:UI_DIR          = ../../generate/FITKCore/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKCore/release/obj
    Release:LIBS +=  \
#        -L../../output/bin  \
#        -lCommon \
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKCore/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKCore/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKCore/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKCore/debug/obj
    Debug:LIBS +=  \
#        -L../../output/bin_d \
#        -lCommon \
     


    message("Windows FITKCore generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKCore/release/moc
    UI_DIR          = ../../generate/FITKCore/release/qui
    RCC_DIR         = ../../generate/FITKCore/release/rcc
    OBJECTS_DIR     = ../../generate/FITKCore/release/obj
    LIBS += \
#        -L../../output/bin \
#        -lCommon \
       

    message("Linux FITKCore generated")
}

