TEMPLATE    =  app
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FlowApp
QT         +=  core widgets gui 

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./vtk.pri)
include(./FlowApp.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       
    Release:CONFIG            +=  console
    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/FlowApp/release/moc
    Release:RCC_DIR         = ../generate/FlowApp/release/rcc
    Release:UI_DIR          = ../generate/FlowApp/release/qui
    Release:OBJECTS_DIR     = ../generate/FlowApp/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKCompMessageWidget \
        -lFITKRenderWindowVTK \
        -lFITKInterfaceFlowOF \
        -lFITKInterfaceFlowPHengLEI \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        -lOperatorsModel \
        -lOperatorsGUI \
        -lGUIFrame \
        -lGUIWidget \
        -lFITKWidget \
        -lFITKOFMeshIO \
        -lFITKFlowOFIOHDF5 \
        -lFITKCGNSIO \
        -lFITKPlotWindow

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/FlowApp/debug/moc
    Debug:RCC_DIR         = ../generate/FlowApp/debug/rcc
    Debug:UI_DIR          = ../generate/FlowApp/debug/qui
    Debug:OBJECTS_DIR     = ../generate/FlowApp/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKCompMessageWidget \
        -lFITKRenderWindowVTK \
        -lFITKInterfaceFlowOF \
        -lFITKInterfaceFlowPHengLEI \
        -lFITKInterfaceModel \ 
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        -lOperatorsModel \
        -lFITKInterfaceMesh \
        -lOperatorsGUI \
        -lGUIFrame \
        -lGUIWidget \
        -lFITKWidget \
        -lFITKOFMeshIO \
        -lFITKFlowOFIOHDF5 \
        -lFITKCGNSIO \
        -lFITKPlotWindow

Debug:LIBS +=  -L$$PWD/../Tools/Win64/SARibbon/libd/  \                                  
               -lSARibbonBard  \

Release:LIBS +=  -L$$PWD/../Tools/Win64/SARibbon/lib/  \
               -lSARibbonBar  \

    message("Windows FlowApp generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/FlowApp/release/moc
    UI_DIR          = ../generate/FlowApp/release/qui
    RCC_DIR         = ../generate/FlowApp/release/rcc
    OBJECTS_DIR     = ../generate/FlowApp/release/obj
    LIBS += \
        -L../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKCompMessageWidget \
        -lFITKRenderWindowVTK \
        -lFITKInterfaceFlowOF \
        -lFITKInterfaceFlowPHengLEI \
        -lFITKInterfaceModel \
        -lFITKInterfaceMesh \
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        -lOperatorsModel \
        -lOperatorsGUI \
        -lOperatorsInterface \
        -lGUIFrame \
        -lGUIWidget \
        -lFITKWidget \
        -L$$PWD/../Tools/Linux64/SARibbon/lib/  \
        -lSARibbonBar \
        -lFITKOFMeshIO \
        -lFITKFlowOFIOHDF5 \
        -lFITKCGNSIO \
        -lFITKPlotWindow

    message("Linux FlowApp generated")
}

