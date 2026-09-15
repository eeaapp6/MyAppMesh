TEMPLATE    =  app
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  CADOCCApp
QT         +=  core widgets gui 

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./occ.pri)
include(./hdf5.pri)
include(./CADOCCApp.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \

    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/CADOCCApp/release/moc
    Release:RCC_DIR         = ../generate/CADOCCApp/release/rcc
    Release:UI_DIR          = ../generate/CADOCCApp/release/qui
    Release:OBJECTS_DIR     = ../generate/CADOCCApp/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKCompMessageWidget \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        -lFITKRenderWindowOCC \
        -lOperatorsGUI \
        -lOperatorsModel \
        -lFITKWidget \
        -lGUIWidget \
        -lGUIFrame \
        -lFITKInterfaceGeoIOHDF5 \
        -lFITKGeoOCCIOHDF5 \
        -lFITKPython \
        -lFITKHttpPythonDriver \
        -lPythonInterface \
        
    
    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/CADOCCApp/debug/moc
    Debug:RCC_DIR         = ../generate/CADOCCApp/debug/rcc
    Debug:UI_DIR          = ../generate/CADOCCApp/debug/qui
    Debug:OBJECTS_DIR     = ../generate/CADOCCApp/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKCompMessageWidget \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        -lFITKRenderWindowOCC \
        -lOperatorsGUI \
        -lOperatorsModel \
        -lFITKWidget \
        -lGUIWidget \
        -lGUIFrame \
        -lFITKInterfaceGeoIOHDF5 \
        -lFITKGeoOCCIOHDF5 \
        -lFITKPython \
        -lFITKHttpPythonDriver \
        -lPythonInterface \


    message("Windows CADOCCApp generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/CADOCCApp/release/moc
    UI_DIR          = ../generate/CADOCCApp/release/qui
    RCC_DIR         = ../generate/CADOCCApp/release/rcc
    OBJECTS_DIR     = ../generate/CADOCCApp/release/obj
    LIBS += \
        -L../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKCompMessageWidget \
        -lFITKInterfaceModel \
        -lFITKInterfaceGeometry \
        -lFITKInterfaceGeoIOHDF5 \
        -lFITKGeoCompOCC \
        -lFITKRenderWindowOCC \
        -lFITKOCCGraphAdaptor \
        -lFITKGeoOCCIOHDF5 \
        -lFITKAdaptor \
        -lFITKWidget \
        -lPickDataProvider \
        -lGraphDataProvider \
        -lOperatorsInterface \
        -lOperatorsGUI \
        -lOperatorsModel \
        -lGUIDialog \
        -lGUIWidget \
        -lGUIFrame \
        -lFITKPython \
        -lFITKHttpPythonDriver \
        -lPythonInterface \

    message("Linux CADOCCApp generated")
}

