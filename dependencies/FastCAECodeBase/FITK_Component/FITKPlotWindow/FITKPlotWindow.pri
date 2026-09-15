HEADERS += \ 
    $$PWD/FITKPlotWindowAPI.h \
    $$PWD/FITKXYPlotWindow.h \
    $$PWD/FITKXYPlotCurveStyle.h \
    $$PWD/FITKBarChartWindow.h \
    $$PWD/FITKPlotWindowsInterface.h \
    $$PWD/FITKXYPlotProperty.h \
    $$PWD/FITKXYCurveDrawManager.h \
    $$PWD/FITKBarChartProperty.h \
    $$PWD/FITKBarChartDrawManager.h \
    $$PWD/FITKPolarPlotWindow.h \
    $$PWD/FITKPolarPlotProperty.h \
    $$PWD/FITKPolarCurveDrawManager.h \
    $$PWD/FITKSpectrogramManager.h \
    $$PWD/FITKSpectrogramProperty.h \
    $$PWD/FITKSpectrogramWindow.h \

SOURCES += \
    $$PWD/FITKXYPlotWindow.cpp \
    $$PWD/FITKXYPlotCurveStyle.cpp \
    $$PWD/FITKBarChartWindow.cpp \
    $$PWD/FITKPlotWindowsInterface.cpp \
    $$PWD/FITKXYPlotProperty.cpp \
    $$PWD/FITKXYCurveDrawManager.cpp \
    $$PWD/FITKBarChartProperty.cpp \
    $$PWD/FITKBarChartDrawManager.cpp \
    $$PWD/FITKPolarPlotWindow.cpp \
    $$PWD/FITKPolarPlotProperty.cpp \
    $$PWD/FITKPolarCurveDrawManager.cpp \
    $$PWD/FITKSpectrogramManager.cpp \
    $$PWD/FITKSpectrogramProperty.cpp \
    $$PWD/FITKSpectrogramWindow.cpp \

FORMS+=\
    $$PWD/FITKXYPlotWindow.ui \
    $$PWD/FITKXYPlotCurveStyle.ui \
    $$PWD/FITKBarChartWindow.ui \
    $$PWD/FITKPolarPlotWindow.ui \
    $$PWD/FITKSpectrogramWindow.ui \

 
#RESOURCES += ../qrc/qianfan.qrc

#RC_FILE += ../qrc/qianfan.rc

TRANSLATIONS += \
    $$PWD/ZH_CN/FITKPlotWindow_ZH_CN.ts \

DISTFILES += \
    $$PWD/License.txt \
    
 