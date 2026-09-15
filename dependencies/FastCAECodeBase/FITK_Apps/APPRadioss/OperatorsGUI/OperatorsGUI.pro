TEMPLATE    =   lib
CONFIG      +=  c++11
CONFIG      +=  qt
TARGET      =   OperatorsGUI
QT          +=  core widgets gui
DEFINES     +=  OperatorsGUI_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./OperatorsGUI.pri)
include(../vtk.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../FITK_Kernal  \
                       ../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \

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
        -lGUIFrame \
        -lGUIWidget \
        -lFITKRenderWindowVTK \
        -lFITKRadiossData \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lOperatorsInterface \
        -lGraphDataProvider \
        -lGraphDataVTKAdaptor \
        -lPickDataProvider \
        -lGUIToolDialog \
        -lFITKInterfaceStructuralPost \
        -lGUIPostDialog \



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
        -lGUIFrame \
        -lGUIWidget \
        -lFITKRenderWindowVTK \
        -lFITKRadiossData \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lOperatorsInterface \
        -lGraphDataProvider \
        -lGraphDataVTKAdaptor \
        -lPickDataProvider \
        -lGUIToolDialog \
        -lFITKInterfaceStructuralPost \
        -lGUIPostDialog \


Debug:LIBS +=  -L$$PWD/../Tools/Win64/SARibbon/libd/  \                                  
               -lSARibbonBard  \

Release:LIBS +=  -L$$PWD/../Tools/Win64/SARibbon/lib/  \
               -lSARibbonBar  \

    message("Windows OperatorsGUI generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../FITK_Kernal \
                        ../Tools/Linux64/SARibbon/include/  \

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
        -lGUIFrame \
        -lGUIWidget \
        -lFITKRenderWindowVTK \
        -lFITKRadiossData \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceModel \
        -lOperatorsInterface \
        -lGraphDataProvider \
        -lGraphDataVTKAdaptor \
        -lPickDataProvider \
        -lFITKInterfaceStructuralPost \
        -L$$PWD/../Tools/Linux64/SARibbon/lib/  \
        -lSARibbonBar \
        -lGUIToolDialog \
        -lGUIPostDialog \

    message("Linux OperatorsGUI generated")
}
