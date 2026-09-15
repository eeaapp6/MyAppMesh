TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKVTKAlgorithm
QT         +=  core widgets gui
DEFINES    +=  FITKVTKALGORITHM_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./vtk.pri)
include(./FITKVTKAlgorithm.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKVTKAlgorithm/release/moc
    Release:RCC_DIR         = ../../generate/FITKVTKAlgorithm/release/rcc
    Release:UI_DIR          = ../../generate/FITKVTKAlgorithm/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKVTKAlgorithm/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKVTKAlgorithm/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKVTKAlgorithm/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKVTKAlgorithm/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKVTKAlgorithm/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \

     


    message("Windows FITKVTKAlgorithm generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKVTKAlgorithm/release/moc
    UI_DIR          = ../../generate/FITKVTKAlgorithm/release/qui
    RCC_DIR         = ../../generate/FITKVTKAlgorithm/release/rcc
    OBJECTS_DIR     = ../../generate/FITKVTKAlgorithm/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
       

    message("Linux FITKVTKAlgorithm generated")
}

