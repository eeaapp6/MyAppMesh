HEADERS += \ 
    $$PWD/FITKPythonAPI.h \
    $$PWD/FITKPythonInterface.h \
    $$PWD/PythonAgent.h \
    $$PWD/FITKPythonQtScriptingConsole.h \

SOURCES += \
    $$PWD/FITKPythonInterface.cpp \
    $$PWD/PythonAgent.cpp \
    $$PWD/FITKPythonQtScriptingConsole.cpp \

FORMS += \
#    $$PWD/PluginManageDialog.ui \
 
 
RESOURCES += ./PythonCode.qrc

#RC_FILE += ../qrc/qianfan.rc

DISTFILES += \
    $$PWD/License.txt \