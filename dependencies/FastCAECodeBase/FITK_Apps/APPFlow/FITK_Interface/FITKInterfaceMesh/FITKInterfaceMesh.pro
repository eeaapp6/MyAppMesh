TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKInterfaceMesh
QT         +=  core widgets gui
DEFINES    +=  FITKInterfaceMesh_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKInterfaceMesh.pri)
include(./vtk.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKInterfaceMesh/release/moc
    Release:RCC_DIR         = ../../generate/FITKInterfaceMesh/release/rcc
    Release:UI_DIR          = ../../generate/FITKInterfaceMesh/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKInterfaceMesh/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKInterfaceModel \
        -lFITKVTKAlgorithm


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKInterfaceMesh/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKInterfaceMesh/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKInterfaceMesh/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKInterfaceMesh/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKInterfaceModel \
        -lFITKVTKAlgorithm

     


    message("Windows FITKInterfaceMesh generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKInterfaceMesh/release/moc
    UI_DIR          = ../../generate/FITKInterfaceMesh/release/qui
    RCC_DIR         = ../../generate/FITKInterfaceMesh/release/rcc
    OBJECTS_DIR     = ../../generate/FITKInterfaceMesh/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKInterfaceModel \
        -lFITKVTKAlgorithm


    message("Linux FITKInterfaceMesh generated")
}

