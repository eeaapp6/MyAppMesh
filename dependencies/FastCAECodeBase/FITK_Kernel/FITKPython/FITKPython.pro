TEMPLATE    =  lib
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  FITKPython
QT         +=  core widgets gui
DEFINES    +=  FITKPython_API

win32{
    QMAKE_CXXFLAGS += /MP
}

unix:!mac{ QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/\'" }

include(./FITKPython.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../Tools/Win64/PythonQt/include \
                       ../../Tools/Win64/Python37/include \

    Release:DESTDIR         = ../../output/bin
    Release:MOC_DIR         = ../../generate/FITKPython/release/moc
    Release:RCC_DIR         = ../../generate/FITKPython/release/rcc
    Release:UI_DIR          = ../../generate/FITKPython/release/qui
    Release:OBJECTS_DIR     = ../../generate/FITKPython/release/obj
    Release:LIBS +=  \
        -L../../Tools/Win64/PythonQt/lib \
        -lPythonQt-Qt5-Python3.7 \
        -lPythonQt_QtAll-Qt5-Python3.7 \
        -L../../Tools/Win64/Python37/libs \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \
        
   


    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../../output/bin_d
    Debug:MOC_DIR         = ../../generate/FITKPython/debug/moc
    Debug:RCC_DIR         = ../../generate/FITKPython/debug/rcc
    Debug:UI_DIR          = ../../generate/FITKPython/debug/qui
    Debug:OBJECTS_DIR     = ../../generate/FITKPython/debug/obj
    Debug:LIBS +=  \
        -L../../Tools/Win64/PythonQt/libd \
        -lPythonQt-Qt5-Python3.7_d \
        -lPythonQt_QtAll-Qt5-Python3.7_d \
        -L../../Tools/Win64/Python37/libs \
        -L../../output/bin_d  \
        -lFITKCore \
        -lFITKAppFramework \
     


    message("Windows FITKPython generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../Tools/Linux64/PythonQt/include \
                        ../../Tools/Linux64/Python37/include/python3.7m \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../../output/bin
    MOC_DIR         = ../../generate/FITKPython/release/moc
    UI_DIR          = ../../generate/FITKPython/release/qui
    RCC_DIR         = ../../generate/FITKPython/release/rcc
    OBJECTS_DIR     = ../../generate/FITKPython/release/obj
    LIBS += \
        -L../../Tools/Linux64/PythonQt/lib \
        -lPythonQt-Qt5-Python \
        -lPythonQt_QtAll-Qt5-Python \
        -L../../output/bin  \
        -lFITKCore \
        -lFITKAppFramework \
        

    message("Linux FITKPython generated")
}

