HEADERS += \ 
    $$PWD/ApplicationCAD.h \
    $$PWD/MainWindowGenerator.h \
    $$PWD/GlobalDataFactory.h \
    $$PWD/ComponentFactory.h \ 
    $$PWD/CommandLineHandler.h \
    $$PWD/SignalProcessor.h \
    $$PWD/WorkBenchHandler.h \
    $$PWD/PythonRegister.h \
    $$PWD/CADOCCSettings.h \

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/ApplicationCAD.cpp \
    $$PWD/MainWindowGenerator.cpp \
    $$PWD/GlobalDataFactory.cpp \
    $$PWD/ComponentFactory.cpp \ 
    $$PWD/CommandLineHandler.cpp \
    $$PWD/SignalProcessor.cpp \
    $$PWD/WorkBenchHandler.cpp \
    $$PWD/PythonRegister.cpp \
    $$PWD/CADOCCSettings.cpp \

RESOURCES += \
    $$PWD/../Resource/resources.qrc \
    $$PWD/../Resource/translations.qrc 
    
RC_ICONS = icon.ico 