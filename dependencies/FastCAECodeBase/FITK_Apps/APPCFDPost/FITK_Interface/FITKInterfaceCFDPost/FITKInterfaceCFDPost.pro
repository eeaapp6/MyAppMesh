TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKInterfaceCFDPost
QT         +=  core widgets gui
DEFINES    +=  FITKInterfaceCFDPost_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKInterfaceCFDPost.pri)
include(./vtk.pri)
include(./cgns.pri)

win32{

    INCLUDEPATH    +=  ./ \
                       ../ \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKInterfaceCFDPost/release/moc
    Release:RCC_DIR         = ../../generate/FITKInterfaceCFDPost/release/rcc
    Release:UI_DIR          = ../../generate/FITKInterfaceCFDPost/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKInterfaceCFDPost/release/obj
    Release:LIBS +=  \
         -L../../output/bin  \
         -lFITKCore \
         -lFITKAppFramework \
         -lFITKVTKAlgorithm \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKInterfaceCFDPost/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKInterfaceCFDPost/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKInterfaceCFDPost/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKInterfaceCFDPost/debug/obj
    Debug:LIBS +=  \
         -L../../output/bin_d \
         -lFITKCore \
         -lFITKAppFramework \
         -lFITKVTKAlgorithm \

    message("Windows FITKInterfaceCFDPost generated")
}

unix{

    INCLUDEPATH    +=   ./ \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKInterfaceCFDPost/release/moc
    UI_DIR          = ../../generate/FITKInterfaceCFDPost/release/qui
    RCC_DIR         = ../../generate/FITKInterfaceCFDPost/release/rcc
    OBJECTS_DIR     = ../../generate/FITKInterfaceCFDPost/release/obj
    LIBS += \
         -L../../output/bin \
         -lFITKCore \
         -lFITKAppFramework \
         -lFITKVTKAlgorithm \
       
    message("Linux FITKInterfaceCFDPost generated")
}

