TEMPLATE    =   lib
CONFIG      +=  c++11
CONFIG      +=  qt
TARGET      =   GUIPostDialog
QT          +=  core widgets gui
DEFINES     +=  GUIPostDialog_API

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./GUIPostDialog.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../../  \
                       ../../Tools/Win64/qwt/include \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/GUIPostDialog/release/moc
    Release:RCC_DIR         = ../../generate/GUIPostDialog/release/rcc
    Release:UI_DIR          = ../../generate/GUIPostDialog/release/qui
    Release:OBJECTS_DIR     = ../../generate/GUIPostDialog/release/obj
    Release:LIBS +=  \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKRadiossData \
        -lGUIWidget \
        -lFITKWidget \
        -lFITKPlotWindow \
        -lPickDataProvider \
        -L../../Tools/Win64/qwt/lib \
        -lqwt \


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/GUIPostDialog/debug/moc
    Debug:RCC_DIR         = ../../generate/GUIPostDialog/debug/rcc
    Debug:UI_DIR          = ../../generate/GUIPostDialog/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/GUIPostDialog/debug/obj
    Debug:LIBS +=  \
        -L../../output/bin_d  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKRadiossData \
        -lGUIWidget \
        -lFITKWidget \
        -lFITKPlotWindow \
        -lPickDataProvider \
        -L../../Tools/Win64/qwt/lib \
        -lqwt \


    message("Windows GUIPostDialog generated")
}

unix{
    INCLUDEPATH    +=  ./   \
                       ../../  \
                       ../../Tools/Linux64/qwt/include \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/GUIPostDialog/release/moc
    UI_DIR          = ../../generate/GUIPostDialog/release/qui
    RCC_DIR         = ../../generate/GUIPostDialog/release/rcc
    OBJECTS_DIR     = ../../generate/GUIPostDialog/release/obj
    LIBS += \
        -L../../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKRadiossData \
        -lGUIWidget \
        -lFITKWidget \
        -lFITKPlotWindow \
        -lPickDataProvider \
        -L../../Tools/Win64/qwt/lib \
        -lqwt \


    message("Linux GUIPostDialog generated")
}
