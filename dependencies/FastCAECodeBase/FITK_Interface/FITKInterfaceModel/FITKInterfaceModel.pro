
TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKInterfaceModel
QT         +=  core widgets gui
DEFINES    +=  FITKInterfaceModel_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKInterfaceModel.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKInterfaceModel/release/moc
    Release:RCC_DIR         = ../../generate/FITKInterfaceModel/release/rcc
    Release:UI_DIR          = ../../generate/FITKInterfaceModel/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKInterfaceModel/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \



    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKInterfaceModel/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKInterfaceModel/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKInterfaceModel/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKInterfaceModel/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \


    message("Windows FITKInterfaceModel generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKInterfaceModel/release/moc
    UI_DIR          = ../../generate/FITKInterfaceModel/release/qui
    RCC_DIR         = ../../generate/FITKInterfaceModel/release/rcc
    OBJECTS_DIR     = ../../generate/FITKInterfaceModel/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \       

    message("Linux FITKInterfaceModel generated")
}

