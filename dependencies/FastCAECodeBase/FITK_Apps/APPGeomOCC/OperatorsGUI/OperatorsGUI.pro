TEMPLATE    =   lib
CONFIG      +=  c++11
CONFIG      +=  qt
TARGET      =   OperatorsGUI
QT          +=  core widgets gui
DEFINES     +=  OperatorsGUI_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./occ.pri)
include(./OperatorsGUI.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../FITK_Kernal  \

    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/OperatorsGUI/release/moc
    Release:RCC_DIR         = ../generate/OperatorsGUI/release/rcc
    Release:UI_DIR          = ../generate/OperatorsGUI/release/qui
    Release:OBJECTS_DIR     = ../generate/OperatorsGUI/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKWidget \
        -lOperatorsInterface \
        -lFITKOCCGraphAdaptor \
        -lFITKRenderWindowOCC \
        -lGraphDataProvider \
        -lGUIFrame \
        -lGUIWidget \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKGeoCompOCC \
        -lPickDataProvider

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/OperatorsGUI/debug/moc
    Debug:RCC_DIR         = ../generate/OperatorsGUI/debug/rcc
    Debug:UI_DIR          = ../generate/OperatorsGUI/debug/qui
    Debug:OBJECTS_DIR     = ../generate/OperatorsGUI/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKWidget \
        -lOperatorsInterface \
        -lFITKOCCGraphAdaptor \
        -lFITKRenderWindowOCC \
        -lGraphDataProvider \
        -lGUIFrame \
        -lGUIWidget \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKGeoCompOCC \
        -lPickDataProvider


    message("Windows OperatorsGUI generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../FITK_Kernal \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/OperatorsGUI/release/moc
    UI_DIR          = ../generate/OperatorsGUI/release/qui
    RCC_DIR         = ../generate/OperatorsGUI/release/rcc
    OBJECTS_DIR     = ../generate/OperatorsGUI/release/obj
    LIBS += \
        -L../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKWidget \
        -lOperatorsInterface \
        -lFITKOCCGraphAdaptor \
        -lFITKRenderWindowOCC \
        -lGraphDataProvider \
        -lGUIFrame \
        -lGUIWidget \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lFITKGeoCompOCC \
        -lPickDataProvider
        
        
    message("Linux OperatorsGUI generated")
}

TRANSLATIONS = \
    ../Resource/translations/Translate_OperatorsGUI_EN.ts \
    ../Resource/translations/Translate_OperatorsGUI_CN.ts \