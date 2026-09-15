TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS += \
    FITK_Kernel \
    FITK_Interface \
    FITK_Component \
    GUIWidget \
    GUIFrame \
    PickDataProvider \  
    GUIDialog \
    GraphDataProvider \
    OperatorsInterface \
    OperatorsGUI \
    OperatorsModel \
    PythonInterface \
    CADOCCApp 


# Denpends - Kernel
# FITKCore.depends = 
FITKAppFramework.depends = FITKCore
FITKPython.depends = FITKCore FITKAppFramework
FITKAdaptor.depends = FITKCore
FITKEasyParam.depends = FITKCore

# Denpends - Interface
FITKInterfaceModel.depends = FITKCore
FITKInterfaceIO.depends = FITKCore
FITKInterfaceGeometry.depends = FITKCore FITKAppFramework FITKInterfaceModel
FITKInterfaceGeoIOHDF5.depends = FITKCore FITKAdaptor FITKAppFramework FITKInterfaceIO FITKInterfaceGeometry

# Denpends - Component
FITKWidget.depends = FITKCore FITKAppFramework
FITKCompMessageWidget.depends = FITKCore FITKAppFramework
FITKGeoCompOCC.depends = FITKCore FITKAppFramework FITKInterfaceModel FITKInterfaceGeometry
FITKRenderWindowOCC.depends = FITKCore FITKAppFramework
FITKOCCGraphAdaptor.depends = FITKCore FITKAdaptor FITKInterfaceModel FITKInterfaceGeometry FITKGeoCompOCC FITKRenderWindowOCC
FITKGeoOCCIOHDF5.depends = FITKCore FITKAppFramework FITKAdaptor FITKInterfaceGeometry FITKInterfaceIO FITKInterfaceGeoIOHDF5 FITKGeoCompOCC

# Denpends - APP
GUIWidget.depends = FITKCore FITKAppFramework FITKInterfaceModel FITKInterfaceGeometry FITKGeoCompOCC FITKRenderWindowOCC FITKOCCGraphAdaptor
GUIFrame.depends = FITKCore FITKAppFramework FITKPython FITKInterfaceModel FITKInterfaceGeometry FITKWidget FITKCompMessageWidget GUIWidget
PickDataProvider.depends = FITKCore FITKAppFramework FITKInterfaceModel FITKInterfaceGeometry FITKWidget FITKGeoCompOCC FITKRenderWindowOCC FITKOCCGraphAdaptor GUIFrame
GUIDialog.depends = FITKCore FITKAppFramework FITKPython FITKEasyParam FITKInterfaceModel FITKInterfaceGeometry FITKWidget GUIWidget GUIFrame PickDataProvider
GraphDataProvider.depends = FITKCore FITKAppFramework FITKAdaptor FITKInterfaceGeometry FITKRenderWindowOCC FITKOCCGraphAdaptor
OperatorsInterface.depends = FITKCore FITKAppFramework
OperatorsGUI.depends = FITKCore FITKAppFramework FITKInterfaceModel FITKInterfaceGeometry FITKWidget FITKGeoCompOCC FITKRenderWindowOCC FITKOCCGraphAdaptor OperatorsInterface GUIWidget GUIFrame PickDataProvider GraphDataProvider
OperatorsModel.depends = FITKCore FITKAppFramework FITKInterfaceModel FITKInterfaceGeometry FITKInterfaceGeoIOHDF5 FITKRenderWindowOCC FITKGeoOCCIOHDF5 OperatorsInterface GUIWidget GUIFrame PickDataProvider GUIDialog
PythonInterface.depends = FITKCore FITKAppFramework FITKPython FITKInterfaceGeometry FITKGeoCompOCC OperatorsInterface GUIWidget GUIFrame OperatorsModel OperatorsGUI
CADOCCApp.depends = FITKCore FITKAppFramework FITKPython FITKInterfaceModel FITKInterfaceGeometry FITKInterfaceGeoIOHDF5 FITKCompMessageWidget FITKRenderWindowOCC FITKGeoCompOCC FITKGeoOCCIOHDF5 FITKWidget GUIWidget GUIFrame OperatorsModel OperatorsGUI PythonInterface