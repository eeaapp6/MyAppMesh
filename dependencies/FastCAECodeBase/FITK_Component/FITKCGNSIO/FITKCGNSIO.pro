TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKCGNSIO
QT         +=  core  widgets gui
DEFINES    +=  FITKCGNSIO_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKCGNSIO.pri)
include(./cgns.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKCGNSIO/release/moc
    Release:RCC_DIR         = ../../generate/FITKCGNSIO/release/rcc
    Release:UI_DIR          = ../../generate/FITKCGNSIO/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKCGNSIO/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \
        -lFITKAppFramework \


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKCGNSIO/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKCGNSIO/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKCGNSIO/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKCGNSIO/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \
        -lFITKAppFramework \




    message("Windows FITKCGNSIO generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKCGNSIO/release/moc
    UI_DIR          = ../../generate/FITKCGNSIO/release/qui
    RCC_DIR         = ../../generate/FITKCGNSIO/release/rcc
    OBJECTS_DIR     = ../../generate/FITKCGNSIO/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKInterfaceIO \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \
        -lFITKAppFramework \


    message("Linux FITKCGNSIO generated")
}

