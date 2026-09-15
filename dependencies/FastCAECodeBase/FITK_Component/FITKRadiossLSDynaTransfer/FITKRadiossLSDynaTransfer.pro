TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKRadiossLSDynaTransfer
QT         +=  core widgets gui
DEFINES    +=  FITKRadiossLSDynaTransfer_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKRadiossLSDynaTransfer.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../  \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKRadiossLSDynaTransfer/release/moc
    Release:RCC_DIR         = ../../generate/FITKRadiossLSDynaTransfer/release/rcc
    Release:UI_DIR          = ../../generate/FITKRadiossLSDynaTransfer/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKRadiossLSDynaTransfer/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKInterfaceIO \
        -lFITKRadiossData \
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKRadiossLSDynaTransfer/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKRadiossLSDynaTransfer/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKRadiossLSDynaTransfer/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKRadiossLSDynaTransfer/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKInterfaceIO \
        -lFITKRadiossData \
     


    message("Windows FITKRadiossLSDynaTransfer generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../  \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKRadiossLSDynaTransfer/release/moc
    UI_DIR          = ../../generate/FITKRadiossLSDynaTransfer/release/qui
    RCC_DIR         = ../../generate/FITKRadiossLSDynaTransfer/release/rcc
    OBJECTS_DIR     = ../../generate/FITKRadiossLSDynaTransfer/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKInterfaceIO \
        -lFITKRadiossData \
       

    message("Linux FITKRadiossLSDynaTransfer generated")
}

