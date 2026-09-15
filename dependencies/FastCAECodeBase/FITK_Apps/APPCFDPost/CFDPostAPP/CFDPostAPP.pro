TEMPLATE    =   app
CONFIG      +=  c++11 qt
TARGET      =   CFDPostAPP
QT          +=  core gui widgets

INCLUDEPATH += ./ \
                ../ \
                ../../ \
                
include(CFDPostAPP.pri)
include(../vtk.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \
                       
    Release:CONFIG            +=  console
    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/CFDPostAPP/release/moc
    Release:RCC_DIR         = ../generate/CFDPostAPP/release/rcc
    Release:UI_DIR          = ../generate/CFDPostAPP/release/qui
    Release:OBJECTS_DIR     = ../generate/CFDPostAPP/release/obj
    Release:LIBS +=  \
        -L../output/bin  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKRenderWindowVTK \
        -lFITKWidget \
        -lFITKInterfaceCFDPost \
        -lFITKPlotWindow \
        -lFITKHttpPythonDriver \
        -lGUIFrame \
        -lGUIWidget \
        -lOperatorsModel \
        -lOperatorsGUI \
    
    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/CFDPostAPP/debug/moc
    Debug:RCC_DIR         = ../generate/CFDPostAPP/debug/rcc
    Debug:UI_DIR          = ../generate/CFDPostAPP/debug/qui
    Debug:OBJECTS_DIR     = ../generate/CFDPostAPP/debug/obj
    Debug:LIBS +=  \
        -L../output/bin_d  \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKRenderWindowVTK \
        -lFITKWidget \
        -lFITKInterfaceCFDPost \
        -lFITKPlotWindow \
        -lFITKHttpPythonDriver \
        -lGUIFrame \
        -lGUIWidget \
        -lOperatorsModel \
        -lOperatorsGUI \

    message("Windows CFDPostAPP generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/CFDPostAPP/release/moc
    UI_DIR          = ../generate/CFDPostAPP/release/qui
    RCC_DIR         = ../generate/CFDPostAPP/release/rcc
    OBJECTS_DIR     = ../generate/CFDPostAPP/release/obj
    LIBS += \
        -L../output/bin \
        -lFITKAppFramework \
        -lFITKCore \
        -lFITKInterfaceCFDPost \
        -lFITKRenderWindowVTK \
        -lFITKWidget \
        -lFITKEasyParam \
        -lFITKPlotWindow \
        -lFITKHttpPythonDriver \
        -lGUIFrame \
        -lGUIWidget \
        -lOperatorsModel \
        -lOperatorsInterface \
        -lOperatorsGUI \
       
    message("Linux CFDPostAPP generated")
}