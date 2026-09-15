TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  PythonInterface
QT         +=  core widgets gui
DEFINES    +=  PythonInterface_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./PythonInterface.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       $$PWD/../Tools/Win64/PythonQt/include \
                       $$PWD/../Tools/Win64/Python37/include \


    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/PythonInterface/release/moc
    Release:RCC_DIR         = ../generate/PythonInterface/release/rcc
    Release:UI_DIR          = ../generate/PythonInterface/release/qui
    Release:OBJECTS_DIR     = ../generate/PythonInterface/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKCore \
        -lFITKPython \
        -lOperatorsInterface \
        -lOperatorsModel \
        -lOperatorsGUI \
        -lFITKAppFramework \
        -lGUIFrame \
        -lGUIWidget \
        -lFITKHttpPythonDriver \
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        -L../Tools/Win64/PythonQt/lib \
        -lPythonQt-Qt5-Python3.7 \
        -lPythonQt_QtAll-Qt5-Python3.7 \
        -L../Tools/Win64/Python37/libs \

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/PythonInterface/debug/moc
    Debug:RCC_DIR         = ../generate/PythonInterface/debug/rcc
    Debug:UI_DIR          = ../generate/PythonInterface/debug/qui
    Debug:OBJECTS_DIR     = ../generate/PythonInterface/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d \
        -lFITKCore \
        -lFITKPython \
        -lOperatorsInterface \
        -lOperatorsModel \
        -lOperatorsGUI \
        -lFITKAppFramework \
        -lGUIFrame \
        -lGUIWidget \
        -lFITKHttpPythonDriver \
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        -L../Tools/Win64/PythonQt/libd \
        -lPythonQt-Qt5-Python3.7_d \
        -lPythonQt_QtAll-Qt5-Python3.7_d \
        -L../Tools/Win64/Python37/libs \

    message("Windows PythonInterface generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        $$PWD/../Tools/Linux64/PythonQt/include \
                        $$PWD/../Tools/Linux64/Python37/include \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/PythonInterface/release/moc
    UI_DIR          = ../generate/PythonInterface/release/qui
    RCC_DIR         = ../generate/PythonInterface/release/rcc
    OBJECTS_DIR     = ../generate/PythonInterface/release/obj
    LIBS += \
        -L../output/bin \ 
        -lFITKCore \
        -lFITKPython \
        -lOperatorsInterface \
        -lOperatorsModel \
        -lOperatorsGUI \
        -lFITKAppFramework \
        -lGUIFrame \
        -lGUIWidget \
        -lFITKHttpPythonDriver \
        -lFITKInterfaceGeometry \
        -lFITKGeoCompOCC \
        -L../Tools/Linux64/PythonQt/lib \
        -lPythonQt-Qt5-Python3.7 \
        -lPythonQt_QtAll-Qt5-Python3.7 \
        -L../Tools/Linux64/Python37/libs \

    message("Linux PythonInterface generated")
}

