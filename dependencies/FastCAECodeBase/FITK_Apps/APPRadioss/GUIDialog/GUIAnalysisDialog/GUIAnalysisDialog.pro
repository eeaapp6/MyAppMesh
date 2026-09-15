TEMPLATE    =   lib
CONFIG      +=  c++11
CONFIG      +=  qt
TARGET      =   GUIAnalysisDialog
QT          +=  core widgets gui
DEFINES     +=  GUIAnalysisDialog_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./GUIAnalysisDialog.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../../ \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/GUIAnalysisDialog/release/moc
    Release:RCC_DIR         = ../../generate/GUIAnalysisDialog/release/rcc
    Release:UI_DIR          = ../../generate/GUIAnalysisDialog/release/qui
    Release:OBJECTS_DIR     = ../../generate/GUIAnalysisDialog/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKInterfacePhysics \
        -lFITKRadiossData \
        -lFITKPlotWindow \
        -lFITKWidget \
        -lFITKInterfaceModel \
        -lGUIWidget \
        -lPickDataProvider \
        -lGraphDataVTKAdaptor \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/GUIAnalysisDialog/debug/moc
    Debug:RCC_DIR         = ../../generate/GUIAnalysisDialog/debug/rcc
    Debug:UI_DIR          = ../../generate/GUIAnalysisDialog/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/GUIAnalysisDialog/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKInterfacePhysics \
        -lFITKRadiossData \
        -lFITKPlotWindow \
        -lFITKWidget \
        -lFITKInterfaceModel \
        -lGUIWidget \
        -lPickDataProvider \
        -lGraphDataVTKAdaptor \

    message("Windows GUIAnalysisDialog generated")
}

unix{
    INCLUDEPATH    +=  ./   \
                       ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/GUIAnalysisDialog/release/moc
    UI_DIR          = ../../generate/GUIAnalysisDialog/release/qui
    RCC_DIR         = ../../generate/GUIAnalysisDialog/release/rcc
    OBJECTS_DIR     = ../../generate/GUIAnalysisDialog/release/obj
    LIBS += \
        -L../../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKInterfacePhysics \
        -lFITKRadiossData \
        -lFITKPlotWindow \
        -lFITKWidget \
        -lFITKInterfaceModel \
        -lGUIWidget \
        -lPickDataProvider \
        -lGraphDataVTKAdaptor \

    message("Linux GUIAnalysisDialog generated")
}
