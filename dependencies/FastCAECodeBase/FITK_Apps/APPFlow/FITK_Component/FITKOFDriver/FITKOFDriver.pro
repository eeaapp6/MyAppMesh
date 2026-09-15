TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKOFDriver
QT         +=  core widgets gui
DEFINES    +=  FITKOFDRIVER_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKOFDriver.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKOFDriver/release/moc
    Release:RCC_DIR         = ../../generate/FITKOFDriver/release/rcc
    Release:UI_DIR          = ../../generate/FITKOFDriver/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKOFDriver/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKOFDriver/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKOFDriver/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKOFDriver/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKOFDriver/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKAppFramework \


    message("Windows FITKOFDriver generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../  \                   
            
    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKOFDriver/release/moc
    UI_DIR          = ../../generate/FITKOFDriver/release/qui
    RCC_DIR         = ../../generate/FITKOFDriver/release/rcc
    OBJECTS_DIR     = ../../generate/FITKOFDriver/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAppFramework \

    message("Linux FITKOFDriver generated")
}
