TEMPLATE = subdirs

SUBDIRS += \
    $$PWD/AppRadioss \
    $$PWD/FITK_Kernel \ 
    $$PWD/FITK_Interface \
    $$PWD/FITK_Component \
    $$PWD/GUIFrame \
    $$PWD/GUIWidget \
    $$PWD/GUIDialog \
    $$PWD/OperatorsInterface \
    $$PWD/OperatorsModel \
    $$PWD/OperatorsGUI \
    $$PWD/GraphDataProvider \
    $$PWD/GraphDataVTKAdaptor \
    $$PWD/PickDataProvider \

    $$PWD/SimpleHttpClient \


CONFIG += ordered
