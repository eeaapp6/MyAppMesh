TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKFluidVTKGraphAdaptor
QT         +=  core widgets gui
DEFINES    +=  FITKFLUIDGRAPHADAPTOR_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./vtk.pri)
include(./FITKFluidVTKGraphAdaptor.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKFluidVTKGraphAdaptor/release/moc
    Release:RCC_DIR         = ../../generate/FITKFluidVTKGraphAdaptor/release/rcc
    Release:UI_DIR          = ../../generate/FITKFluidVTKGraphAdaptor/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKFluidVTKGraphAdaptor/release/obj
    Release:LIBS += \
        -L../../output/bin \
        -lFITKVTKAlgorithm \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceMesh \
        -lFITKInterfaceMeshGen \
        -lFITKRenderWindowVTK 
        

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKFluidVTKGraphAdaptor/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKFluidVTKGraphAdaptor/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKFluidVTKGraphAdaptor/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKFluidVTKGraphAdaptor/debug/obj
    Debug:LIBS += \
        -L../../output/bin_d \
        -lFITKVTKAlgorithm \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceMesh \
        -lFITKInterfaceMeshGen \
        -lFITKRenderWindowVTK 

    message("Windows FITKFluidVTKGraphAdaptor generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKFluidVTKGraphAdaptor/release/moc
    UI_DIR          = ../../generate/FITKFluidVTKGraphAdaptor/release/qui
    RCC_DIR         = ../../generate/FITKFluidVTKGraphAdaptor/release/rcc
    OBJECTS_DIR     = ../../generate/FITKFluidVTKGraphAdaptor/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKVTKAlgorithm \
        -lFITKCore \
        -lFITKAdaptor \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceMesh \
        -lFITKInterfaceMeshGen \
        -lFITKRenderWindowVTK 

    message("Linux FITKFluidVTKGraphAdptor generated")
}

