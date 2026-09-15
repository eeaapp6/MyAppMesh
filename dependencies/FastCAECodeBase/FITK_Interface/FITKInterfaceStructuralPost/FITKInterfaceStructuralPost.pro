TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKInterfaceStructuralPost
QT         +=  core widgets gui
DEFINES    +=  FITKInterfaceStructuralPost_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKInterfaceStructuralPost.pri)
include(./hdf5.pri)
include(./vtk.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../  \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKInterfaceStructuralPost/release/moc
    Release:RCC_DIR         = ../../generate/FITKInterfaceStructuralPost/release/rcc
    Release:UI_DIR          = ../../generate/FITKInterfaceStructuralPost/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKInterfaceStructuralPost/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKVTKAlgorithm \
        -lFITKAppFramework \
        -lFITKInterfaceMesh


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKInterfaceStructuralPost/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKInterfaceStructuralPost/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKInterfaceStructuralPost/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKInterfaceStructuralPost/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d  \
        -lFITKCore \
        -lFITKVTKAlgorithm \
        -lFITKAppFramework \
        -lFITKInterfaceMesh


    message("Windows FITKInterfaceStructuralPost generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKInterfaceStructuralPost/release/moc
    UI_DIR          = ../../generate/FITKInterfaceStructuralPost/release/qui
    RCC_DIR         = ../../generate/FITKInterfaceStructuralPost/release/rcc
    OBJECTS_DIR     = ../../generate/FITKInterfaceStructuralPost/release/obj
    LIBS += \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKVTKAlgorithm \
        -lFITKAppFramework \
        -lFITKInterfaceMesh

    message("Linux FITKInterfaceStructuralPost generated")
}

