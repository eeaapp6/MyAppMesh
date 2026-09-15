TEMPLATE = subdirs

SUBDIRS += \
    $$PWD/FITK_Component \ 
    $$PWD/FITK_Interface \ 
    $$PWD/FITK_Kernel \ 
    $$PWD/GUIFrame/GUIFrame.pro \
    $$PWD/GUIWidget/GUIWidget.pro \
    $$PWD/GUIDialog/GUIDialog.pro \
    $$PWD/PostGraphAdaptor/PostGraphAdaptor.pro \
    $$PWD/OperatorsGUI/OperatorsGUI.pro \
    $$PWD/OperatorsInterface/OperatorsInterface.pro \
    $$PWD/OperatorsModel/OperatorsModel.pro \
    $$PWD/CFDPostAPP/CFDPostAPP.pro \

CONFIG += ordered
