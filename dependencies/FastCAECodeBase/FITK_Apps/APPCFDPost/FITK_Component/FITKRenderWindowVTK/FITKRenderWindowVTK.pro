TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKRenderWindowVTK
QT         +=  core widgets gui svg
DEFINES    +=  FITKRenderWindowVTK_API

# For VTK 7
QT         += opengl

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKRenderWindowVTK.pri)
include(./vtk.pri)
#include(./OpenGL.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       ../../FITK_Kernal \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKRenderWindowVTK/release/moc
    Release:RCC_DIR         = ../../generate/FITKRenderWindowVTK/release/rcc
    Release:UI_DIR          = ../../generate/FITKRenderWindowVTK/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKRenderWindowVTK/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKVTKAlgorithm \
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKRenderWindowVTK/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKRenderWindowVTK/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKRenderWindowVTK/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKRenderWindowVTK/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKVTKAlgorithm \
     


    message("Windows FITKRenderWindowVTK generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \
                        ../../FITK_Kernal \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKRenderWindowVTK/release/moc
    UI_DIR          = ../../generate/FITKRenderWindowVTK/release/qui
    RCC_DIR         = ../../generate/FITKRenderWindowVTK/release/rcc
    OBJECTS_DIR     = ../../generate/FITKRenderWindowVTK/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKVTKAlgorithm \
       

    message("Linux FITKRenderWindowVTK generated")
}

