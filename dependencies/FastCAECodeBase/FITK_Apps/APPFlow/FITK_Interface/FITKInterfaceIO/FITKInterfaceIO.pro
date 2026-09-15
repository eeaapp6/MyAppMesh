TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKInterfaceIO
QT         +=  core widgets gui xml
DEFINES    +=  FITKInterfaceIO_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKInterfaceIO.pri)
include(./hdf5.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       ../../Tools/Win64/rapidxml \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKInterfaceIO/release/moc
    Release:RCC_DIR         = ../../generate/FITKInterfaceIO/release/rcc
    Release:UI_DIR          = ../../generate/FITKInterfaceIO/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKInterfaceIO/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
#        -lFITKAppFramework \
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKInterfaceIO/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKInterfaceIO/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKInterfaceIO/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKInterfaceIO/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
#        -lFITKAppFramework \


    message("Windows FITKInterfaceIO generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \
                        ../../Tools/Linux64/rapidxml 

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKInterfaceIO/release/moc
    UI_DIR          = ../../generate/FITKInterfaceIO/release/qui
    RCC_DIR         = ../../generate/FITKInterfaceIO/release/rcc
    OBJECTS_DIR     = ../../generate/FITKInterfaceIO/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
#        -lFITKAppFramework \

    message("Linux FITKInterfaceIO generated")
}

