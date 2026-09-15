TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKInterfaceMeshGen
QT         +=  core widgets gui
DEFINES    +=  FITKInterfaceMeshGen_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKInterfaceMeshGen.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../  \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKInterfaceMeshGen/release/moc
    Release:RCC_DIR         = ../../generate/FITKInterfaceMeshGen/release/rcc
    Release:UI_DIR          = ../../generate/FITKInterfaceMeshGen/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKInterfaceMeshGen/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
 
    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKInterfaceMeshGen/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKInterfaceMeshGen/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKInterfaceMeshGen/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKInterfaceMeshGen/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \

    message("Windows FITKInterfaceMeshGen generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKInterfaceMeshGen/release/moc
    UI_DIR          = ../../generate/FITKInterfaceMeshGen/release/qui
    RCC_DIR         = ../../generate/FITKInterfaceMeshGen/release/rcc
    OBJECTS_DIR     = ../../generate/FITKInterfaceMeshGen/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKInterfaceModel \     
        -lFITKInterfaceGeometry \

    message("Linux FITKInterfaceMeshGen generated")
}

