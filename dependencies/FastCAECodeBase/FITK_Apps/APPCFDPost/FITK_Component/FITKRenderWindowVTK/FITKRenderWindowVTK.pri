HEADERS += \ 
    $$PWD/FITKRenderWindowVTKAPI.h \
    $$PWD/FITKGraph3DWindowVTK.h \
    $$PWD/FITKGraph3DWindowInterface.h \
    $$PWD/FITKGraphRender.h \
    $$PWD/FITKGraphOperator.h \
    $$PWD/FITKGraphObjectVTK.h \
    $$PWD/FITKGraphInteractionStyle.h \
    $$PWD/FITKGraphAreaPicker.h \
    $$PWD/FITKBoundaryActor.h \
    $$PWD/FITKLegendScaleActor.h \
    $$PWD/FITKGraph3DClipDialog.h \
    $$PWD/FITKGraph3DWidgetPlane.h \
    $$PWD/FITKGraph3DWidgetPlaneCallBack.h \
    
SOURCES += \
    $$PWD/FITKGraph3DWindowVTK.cpp \
    $$PWD/FITKGraph3DWindowInterface.cpp \
    $$PWD/FITKGraphRender.cpp \
    $$PWD/FITKGraphOperator.cpp \
    $$PWD/FITKGraphObjectVTK.cpp \
    $$PWD/FITKGraphInteractionStyle.cpp \
    $$PWD/FITKGraphAreaPicker.cpp \
    $$PWD/FITKBoundaryActor.cpp \
    $$PWD/FITKLegendScaleActor.cpp \
    $$PWD/FITKGraph3DClipDialog.cpp \
    $$PWD/FITKGraph3DWidgetPlane.cpp \
    $$PWD/FITKGraph3DWidgetPlaneCallBack.cpp \
    
FORMS += \
    $$PWD/FITKGraph3DWindowVTK.ui \
    $$PWD/FITKGraph3DClipDialog.ui \
 
RESOURCES += ./FITKIcons.qrc \
             ./OpenGL.qrc \

#RC_FILE += ../qrc/qianfan.rc


DISTFILES += \
    $$PWD/License.txt \
    