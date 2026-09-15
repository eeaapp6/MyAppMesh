TEMPLATE    =   lib
CONFIG      +=  c++11
CONFIG      +=  qt
TARGET      =   PickDataProvider
QT          +=  core widgets gui
DEFINES     +=  PickDataProvider_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(../vtk.pri)
include(./PickDataProvider.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       
    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/PickDataProvider/release/moc
    Release:RCC_DIR         = ../generate/PickDataProvider/release/rcc
    Release:UI_DIR          = ../generate/PickDataProvider/release/qui
    Release:OBJECTS_DIR     = ../generate/PickDataProvider/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKVTKAlgorithm \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \
        -lFITKGeoCompOCC \
        -lFITKRadiossData \
        -lGraphDataVTKAdaptor \
        -lOperatorsInterface \
        
    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/PickDataProvider/debug/moc
    Debug:RCC_DIR         = ../generate/PickDataProvider/debug/rcc
    Debug:UI_DIR          = ../generate/PickDataProvider/debug/qui
    Debug:OBJECTS_DIR     = ../generate/PickDataProvider/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKVTKAlgorithm \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \
        -lFITKGeoCompOCC \
        -lFITKRadiossData \
        -lGraphDataVTKAdaptor \
        -lOperatorsInterface \

    message("Windows PickDataProvider generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/PickDataProvider/release/moc
    UI_DIR          = ../generate/PickDataProvider/release/qui
    RCC_DIR         = ../generate/PickDataProvider/release/rcc
    OBJECTS_DIR     = ../generate/PickDataProvider/release/obj
    LIBS += \
        -L../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKVTKAlgorithm \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \
        -lFITKGeoCompOCC \
        -lFITKRadiossData \
        -lGraphDataVTKAdaptor \
        -lOperatorsInterface \
        

    message("Linux PickDataProvider generated")
}

