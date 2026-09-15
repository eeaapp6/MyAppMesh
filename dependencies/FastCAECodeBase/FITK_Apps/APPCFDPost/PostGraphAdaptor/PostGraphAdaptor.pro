TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  PostGraphAdaptor
QT         +=  core widgets gui
DEFINES    +=  PostGraphAdaptor_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./PostGraphAdaptor.pri)
include(./vtk.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \

    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/PostGraphAdaptor/release/moc
    Release:RCC_DIR         = ../generate/PostGraphAdaptor/release/rcc
    Release:UI_DIR          = ../generate/PostGraphAdaptor/release/qui
    Release:OBJECTS_DIR     = ../generate/PostGraphAdaptor/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKInterfaceCFDPost \
        -lFITKRenderWindowVTK \
        -lFITKVTKAlgorithm \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/PostGraphAdaptor/debug/moc
    Debug:RCC_DIR         = ../generate/PostGraphAdaptor/debug/rcc
    Debug:UI_DIR          = ../generate/PostGraphAdaptor/debug/qui
    Debug:OBJECTS_DIR     = ../generate/PostGraphAdaptor/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKInterfaceCFDPost \
        -lFITKRenderWindowVTK \
        -lFITKVTKAlgorithm \

    message("Windows PostGraphAdaptor generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/PostGraphAdaptor/release/moc
    UI_DIR          = ../generate/PostGraphAdaptor/release/qui
    RCC_DIR         = ../generate/PostGraphAdaptor/release/rcc
    OBJECTS_DIR     = ../generate/PostGraphAdaptor/release/obj
    LIBS += \
        -L../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKInterfaceCFDPost \
        -lFITKRenderWindowVTK \
        -lFITKVTKAlgorithm \
        
    message("Linux PostGraphAdaptor generated")
}

