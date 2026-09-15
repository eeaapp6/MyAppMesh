TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKGeoCompOCC
QT         +=  core widgets gui
DEFINES    +=  FITKGeoCompOCC_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKGeoCompOCC.pri)
include(./occ.pri)
include(./dime.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKGeoCompOCC/release/moc
    Release:RCC_DIR         = ../../generate/FITKGeoCompOCC/release/rcc
    Release:UI_DIR          = ../../generate/FITKGeoCompOCC/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKGeoCompOCC/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKGeoCompOCC/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKGeoCompOCC/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKGeoCompOCC/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKGeoCompOCC/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
     


    message("Windows FITKGeoCompOCC generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKGeoCompOCC/release/moc
    UI_DIR          = ../../generate/FITKGeoCompOCC/release/qui
    RCC_DIR         = ../../generate/FITKGeoCompOCC/release/rcc
    OBJECTS_DIR     = ../../generate/FITKGeoCompOCC/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKCore \
        -lFITKAppFramework \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
       

    message("Linux FITKGeoCompOCC generated")
}


TRANSLATIONS = \
    ./Resource/translations/Translate_FITKGeoCompOCC_EN.ts \
    ./Resource/translations/Translate_FITKGeoCompOCC_CN.ts \