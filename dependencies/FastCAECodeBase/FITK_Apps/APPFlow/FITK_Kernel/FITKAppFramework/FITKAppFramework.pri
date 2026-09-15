HEADERS += \ 
    $$PWD/FITKAppFrameworkAPI.h \
    $$PWD/FITKAppFramework.h \
    $$PWD/FITKAbstractMainWinGenerator.h \
    $$PWD/FITKGlobalData.h \
    $$PWD/FITKAbstractGlobalDataFactory.h \
    $$PWD/FITKComponents.h \
    $$PWD/FITKComponentFactory.h \
    $$PWD/FITKComponentInterface.h \
    $$PWD/FITKSignalTransfer.h \
    $$PWD/FITKMessage.h \
    $$PWD/FITKGlobalEventFilter.h \
    $$PWD/FITKKeyMouseStates.h \
    $$PWD/FITKCommandLineHandler.h \
    $$PWD/FITKAppSettings.h \
    $$PWD/FITKRunTimeSetting.h \
    $$PWD/FITKAbstractEventFilter.h \
    $$PWD/FITKCopyrightDialog.h \
    $$PWD/FITKAbstractProgramDriver.h \
    $$PWD/FITKProgramDriverFactory.h \
    $$PWD/FITKProgramTaskManager.h \
    $$PWD/FITKAbstractPythonRegister.h \
    $$PWD/FITKAbstractCommandRunner.h \
    $$PWD/FITKLinuxShellDriver.h \
    $$PWD/FITKExecProgramDriver.h \
    $$PWD/FITKWorkBenchHandler.h \
    $$PWD/FITKAbstractAutoSaver.h \
    $$PWD/FITKAbstractSysChecker.h \
    $$PWD/FITKAbstractAppInitializer.h \
    $$PWD/FITKAppScriptHelper.h \
    $$PWD/FITKPluginManager.h \
    $$PWD/FITKAbstractPlugin.h \
    $$PWD/FITKExceptionFilter.h \
    $$PWD/FITKAppHistoryFiles.h \
    $$PWD/FITKAbstractLicenseChecker.h \

SOURCES += \
    $$PWD/FITKAppFramework.cpp \
    $$PWD/FITKAbstractMainWinGenerator.cpp \
    $$PWD/FITKGlobalData.cpp \
    $$PWD/FITKAbstractGlobalDataFactory.cpp \
    $$PWD/FITKComponents.cpp \
    $$PWD/FITKComponentFactory.cpp \
    $$PWD/FITKComponentInterface.cpp \
    $$PWD/FITKSignalTransfer.cpp \
    $$PWD/FITKMessage.cpp \
    $$PWD/FITKGlobalEventFilter.cpp \
    $$PWD/FITKKeyMouseStates.cpp \
    $$PWD/FITKCommandLineHandler.cpp \
    $$PWD/FITKAppSettings.cpp \
    $$PWD/FITKRunTimeSetting.cpp \
    $$PWD/FITKAbstractEventFilter.cpp \
    $$PWD/FITKCopyrightDialog.cpp \
    $$PWD/FITKAbstractProgramDriver.cpp \
    $$PWD/FITKProgramDriverFactory.cpp \
    $$PWD/FITKProgramTaskManager.cpp \
    $$PWD/FITKAbstractPythonRegister.cpp \
    $$PWD/FITKAbstractCommandRunner.cpp \
    $$PWD/FITKLinuxShellDriver.cpp \
    $$PWD/FITKExecProgramDriver.cpp \
    $$PWD/FITKWorkBenchHandler.cpp \
    $$PWD/FITKAbstractAutoSaver.cpp \
    $$PWD/FITKAbstractSysChecker.cpp \
    $$PWD/FITKAbstractAppInitializer.cpp \
    $$PWD/FITKAppScriptHelper.cpp \
    $$PWD/FITKPluginManager.cpp \
    $$PWD/FITKAbstractPlugin.cpp \
    $$PWD/FITKExceptionFilter.cpp \
    $$PWD/FITKAppHistoryFiles.cpp \
    $$PWD/FITKAbstractLicenseChecker.cpp \


FORMS += \
    $$PWD/FITKCopyrightDialog.ui \
 
 
RESOURCES += License.qrc

#RC_FILE += ../qrc/qianfan.rc
DISTFILES += \
    $$PWD/License.txt \
    