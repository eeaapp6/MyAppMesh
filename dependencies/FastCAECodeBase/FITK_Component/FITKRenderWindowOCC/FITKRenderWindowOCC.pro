TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKRenderWindowOCC
QT         +=  core widgets gui opengl printsupport
DEFINES    +=  FITKRENDERWINDOWOCC_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./occ.pri)
include(./FITKRenderWindowOCC.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKRenderWindowOCC/release/moc
    Release:RCC_DIR         = ../../generate/FITKRenderWindowOCC/release/rcc
    Release:UI_DIR          = ../../generate/FITKRenderWindowOCC/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKRenderWindowOCC/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore 

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKRenderWindowOCC/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKRenderWindowOCC/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKRenderWindowOCC/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKRenderWindowOCC/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d  \
        -lFITKAppFramework \
        -lFITKCore 

    message("Windows FITKRenderWindowOCC generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKRenderWindowOCC/release/moc
    UI_DIR          = ../../generate/FITKRenderWindowOCC/release/qui
    RCC_DIR         = ../../generate/FITKRenderWindowOCC/release/rcc
    OBJECTS_DIR     = ../../generate/FITKRenderWindowOCC/release/obj
    LIBS += \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore 

    message("Linux FITKRenderWindowOCC generated")
}


TRANSLATIONS = \
    ./Resource/translations/Translate_FITKRenderWindowOCC_EN.ts \
    ./Resource/translations/Translate_FITKRenderWindowOCC_CN.ts \