TEMPLATE    =  app
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  AppRadioss
QT         +=  core widgets gui 

include(../vtk.pri)
include(./AppRadioss.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       ../Tools/Win64/SARibbon/include/SARibbon-2.0.1/  \
    Release:CONFIG            +=  console
    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/AppRadioss/release/moc
    Release:RCC_DIR         = ../generate/AppRadioss/release/rcc
    Release:UI_DIR          = ../generate/AppRadioss/release/qui
    Release:OBJECTS_DIR     = ../generate/AppRadioss/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \ 
        -lFITKInterfaceGeometry \
        -lFITKCompMessageWidget \
        -lFITKRenderWindowVTK \
        -lFITKGeoCompOCC \
        -lFITKRadiossData \
        -lFITKRadiossRadIO \
        -lFITKGmshExeDriver \
        -lFITKGmshMshIO \
        -lGUIFrame \
        -lGUIWidget \
        -lFITKWidget \
        -lOperatorsInterface \
        -lOperatorsModel \
        -lOperatorsGUI \
        -lFITKRadiossLSDynaTransfer \
        -lFITKPlotWindow \
        -lPickDataProvider \
        -lFITKRadiossDataIOHDF5 \
        -lFITKGeoOCCIOHDF5 \
        -lFITKNastranBDFIO \
        -lFITKAbaqusRadiossDataMapper \
        -lFITKMeshIO \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/AppRadioss/debug/moc
    Debug:RCC_DIR         = ../generate/AppRadioss/debug/rcc
    Debug:UI_DIR          = ../generate/AppRadioss/debug/qui
    Debug:OBJECTS_DIR     = ../generate/AppRadioss/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d  \
        -lFITKAppFramework \
        -lFITKCore \ 
        -lFITKInterfaceGeometry \
        -lFITKCompMessageWidget \
        -lFITKRenderWindowVTK \
        -lFITKGeoCompOCC \
        -lFITKRadiossData \
        -lFITKRadiossRadIO \
        -lFITKGmshExeDriver \
        -lFITKGmshMshIO \
        -lGUIFrame \
        -lGUIWidget \
        -lFITKWidget \
        -lOperatorsInterface \
        -lOperatorsModel \
        -lOperatorsGUI \
        -lFITKRadiossLSDynaTransfer \
        -lFITKPlotWindow \
        -lPickDataProvider \
        -lFITKRadiossDataIOHDF5 \
        -lFITKGeoOCCIOHDF5 \
        -lFITKNastranBDFIO \
        -lFITKAbaqusRadiossDataMapper \
        -lFITKMeshIO \

Debug:LIBS +=  -L$$PWD/../Tools/Win64/SARibbon/libd/  \ 
               -lSARibbonBard  \

Release:LIBS +=  -L$$PWD/../Tools/Win64/SARibbon/lib/  \
               -lSARibbonBar  \
               
               
    message("Windows AppRadioss generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \
                        ../Tools/Linux64/SARibbon/include/  \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/AppRadioss/release/moc
    UI_DIR          = ../generate/AppRadioss/release/qui
    RCC_DIR         = ../generate/AppRadioss/release/rcc
    OBJECTS_DIR     = ../generate/AppRadioss/release/obj
    LIBS += \
        -L../output/bin \
        -lFITKAppFramework \
        -lFITKCore \ 
        -lFITKInterfaceGeometry \
        -lFITKCompMessageWidget \
        -lFITKRenderWindowVTK \
        -lFITKGeoCompOCC \
        -lFITKRadiossData \
        -lFITKRadiossRadIO \
        -lFITKGmshExeDriver \
        -lFITKGmshMshIO \
        -L$$PWD/../Tools/Linux64/SARibbon/lib/  \
        -lSARibbonBar \
        -lGUIFrame \
        -lGUIWidget \
        -lFITKWidget \
        -lOperatorsInterface \
        -lOperatorsModel \
        -lOperatorsGUI \
        -lFITKRadiossLSDynaTransfer \
        -lFITKPlotWindow \
        -lPickDataProvider \
        -lFITKRadiossDataIOHDF5 \
        -lFITKGeoOCCIOHDF5 \
        -lFITKNastranBDFIO \
        -lFITKAbaqusRadiossDataMapper \
        -lFITKMeshIO \

        
    message("Linux AppRadioss generated")
}

