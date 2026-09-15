TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKOFMeshIO
QT         +=  core  widgets gui
DEFINES    +=  FITKOFMeshIO_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKOFMeshIO.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKOFMeshIO/release/moc
    Release:RCC_DIR         = ../../generate/FITKOFMeshIO/release/rcc
    Release:UI_DIR          = ../../generate/FITKOFMeshIO/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKOFMeshIO/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \
        -lFITKAppFramework \


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKOFMeshIO/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKOFMeshIO/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKOFMeshIO/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKOFMeshIO/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \
        -lFITKAppFramework \




    message("Windows FITKOFMeshIO generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKOFMeshIO/release/moc
    UI_DIR          = ../../generate/FITKOFMeshIO/release/qui
    RCC_DIR         = ../../generate/FITKOFMeshIO/release/rcc
    OBJECTS_DIR     = ../../generate/FITKOFMeshIO/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \
        -lFITKAppFramework \


    message("Linux FITKOFMeshIO generated")
}

