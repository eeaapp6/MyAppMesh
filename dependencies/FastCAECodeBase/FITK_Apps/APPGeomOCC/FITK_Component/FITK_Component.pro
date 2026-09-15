TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS += \
    FITKWidget \
    FITKCompMessageWidget \
    FITKGeoCompOCC \
    FITKRenderWindowOCC \
    FITKOCCGraphAdaptor \
    FITKGeoOCCIOHDF5


# Denpends - Component
FITKWidget.depends = FITKCore FITKAppFramework
FITKCompMessageWidget.depends = FITKCore FITKAppFramework
FITKGeoCompOCC.depends = FITKCore FITKAppFramework FITKInterfaceModel FITKInterfaceGeometry
FITKRenderWindowOCC.depends = FITKCore FITKAppFramework
FITKOCCGraphAdaptor.depends = FITKCore FITKAdaptor FITKInterfaceModel FITKInterfaceGeometry FITKGeoCompOCC FITKRenderWindowOCC
FITKGeoOCCIOHDF5.depends = FITKCore FITKAppFramework FITKAdaptor FITKInterfaceGeometry FITKInterfaceIO FITKInterfaceGeoIOHDF5 FITKGeoCompOCC