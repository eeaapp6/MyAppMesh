TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  GraphDataVTKAdaptor
QT         +=  core widgets gui
DEFINES    +=  GraphDataVTKAdaptor_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(../vtk.pri)
include(./GraphDataVTKAdaptor.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
							 
    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/GraphDataVTKAdaptor/release/moc
    Release:RCC_DIR         = ../generate/GraphDataVTKAdaptor/release/rcc
    Release:UI_DIR          = ../generate/GraphDataVTKAdaptor/release/qui
    Release:OBJECTS_DIR     = ../generate/GraphDataVTKAdaptor/release/obj
    Release:LIBS += \
        -L../output/bin \
        -lFITKRenderWindowVTK \
        -lFITKVTKAlgorithm \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKAppFramework \
        -lFITKRadiossData \
        -lFITKInterfaceMesh \
        -lFITKInterfaceStructuralPost \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/GraphDataVTKAdaptor/debug/moc
    Debug:RCC_DIR         = ../generate/GraphDataVTKAdaptor/debug/rcc
    Debug:UI_DIR          = ../generate/GraphDataVTKAdaptor/debug/qui
    Debug:OBJECTS_DIR     = ../generate/GraphDataVTKAdaptor/debug/obj
    Debug:LIBS += \
        -L../output/bin_d \
        -lFITKRenderWindowVTK \
        -lFITKVTKAlgorithm \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKAppFramework \
        -lFITKRadiossData \
        -lFITKInterfaceMesh \
        -lFITKInterfaceStructuralPost \

    message("Windows GraphDataVTKAdaptor generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
							 
    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/GraphDataVTKAdaptor/release/moc
    UI_DIR          = ../generate/GraphDataVTKAdaptor/release/qui
    RCC_DIR         = ../generate/GraphDataVTKAdaptor/release/rcc
    OBJECTS_DIR     = ../generate/GraphDataVTKAdaptor/release/obj
    LIBS += \
        -L../output/bin \
        -lFITKRenderWindowVTK \
        -lFITKVTKAlgorithm \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKAppFramework \
        -lFITKRadiossData \
        -lFITKInterfaceMesh \
        -lFITKInterfaceStructuralPost \
        
    message("Linux FITKFluidVTKGraphAdptor generated")
}

