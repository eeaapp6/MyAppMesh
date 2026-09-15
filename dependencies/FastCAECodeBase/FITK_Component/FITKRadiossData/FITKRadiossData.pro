TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKRadiossData
QT         +=  core widgets gui
DEFINES    +=  FITKRadiossData_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKRadiossData.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKRadiossData/release/moc
    Release:RCC_DIR         = ../../generate/FITKRadiossData/release/rcc
    Release:UI_DIR          = ../../generate/FITKRadiossData/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKRadiossData/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceModel \
        -lFITKInterfacePhysics \
        -lFITKInterfaceStructuralPost \
        -lFITKInterfaceGeometry \
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKRadiossData/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKRadiossData/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKRadiossData/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKRadiossData/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d  \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceModel \
        -lFITKInterfacePhysics \
        -lFITKInterfaceStructuralPost \
        -lFITKInterfaceGeometry \


    message("Windows FITKRadiossData generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKRadiossData/release/moc
    UI_DIR          = ../../generate/FITKRadiossData/release/qui
    RCC_DIR         = ../../generate/FITKRadiossData/release/rcc
    OBJECTS_DIR     = ../../generate/FITKRadiossData/release/obj
    LIBS += \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceModel \
        -lFITKInterfacePhysics \
        -lFITKInterfaceStructuralPost \
        -lFITKInterfaceGeometry \
       

    message("Linux FITKRadiossData generated")
}

