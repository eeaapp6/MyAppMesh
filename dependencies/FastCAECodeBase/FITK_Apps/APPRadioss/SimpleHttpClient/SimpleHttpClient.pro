TEMPLATE    =  app
CONFIG     +=  c++11
CONFIG     +=  qt
TARGET      =  SimpleHttpClient
QT         +=  core widgets gui

include(./SimpleHttpClient.pri)
include(./SogouWorkflow.pri)
include(../vtk.pri)

win32{

    INCLUDEPATH    +=  ./   \
                       ../  \
                       ../../ \ 

    Release:CONFIG            +=  console
    Release:DESTDIR         = ../output/bin
    Release:MOC_DIR         = ../generate/SimpleHttpClient/release/moc
    Release:RCC_DIR         = ../generate/SimpleHttpClient/release/rcc
    Release:UI_DIR          = ../generate/SimpleHttpClient/release/qui
    Release:OBJECTS_DIR     = ../generate/SimpleHttpClient/release/obj
    Release:LIBS +=  \
      
      

    Debug:CONFIG            +=  console
    Debug:DESTDIR         = ../output/bin_d
    Debug:MOC_DIR         = ../generate/SimpleHttpClient/debug/moc
    Debug:RCC_DIR         = ../generate/SimpleHttpClient/debug/rcc
    Debug:UI_DIR          = ../generate/SimpleHttpClient/debug/qui
    Debug:OBJECTS_DIR     = ../generate/SimpleHttpClient/debug/obj
    Debug:LIBS +=  \
        
    message("Windows SimpleHttpClient generated")
}

unix{

    INCLUDEPATH    +=   ./  \
                        ../ \
                        ../../ \

    CONFIG          += console
    CONFIG          += plugin
    DESTDIR         = ../output/bin
    MOC_DIR         = ../generate/SimpleHttpClient/release/moc
    UI_DIR          = ../generate/SimpleHttpClient/release/qui
    RCC_DIR         = ../generate/SimpleHttpClient/release/rcc
    OBJECTS_DIR     = ../generate/SimpleHttpClient/release/obj
    LIBS += \
        -L../output/bin \
      
        
    message("Linux SimpleHttpClient generated")
}

