TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKInterfacePhysics
QT         +=  core widgets gui
DEFINES    +=  FITKInterfacePhysics_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKInterfacePhysics.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKInterfacePhysics/release/moc
    Release:RCC_DIR         = ../../generate/FITKInterfacePhysics/release/rcc
    Release:UI_DIR          = ../../generate/FITKInterfacePhysics/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKInterfacePhysics/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKInterfaceModel \
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKInterfacePhysics/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKInterfacePhysics/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKInterfacePhysics/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKInterfacePhysics/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKInterfaceModel \
     


    message("Windows FITKInterfacePhysics generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKInterfacePhysics/release/moc
    UI_DIR          = ../../generate/FITKInterfacePhysics/release/qui
    RCC_DIR         = ../../generate/FITKInterfacePhysics/release/rcc
    OBJECTS_DIR     = ../../generate/FITKInterfacePhysics/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKInterfaceModel \

    message("Linux FITKInterfacePhysics generated")
}

