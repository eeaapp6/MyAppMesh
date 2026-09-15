TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS += \
    FITKInterfaceModel \ 
    FITKInterfaceIO \
    FITKInterfaceGeometry \
    FITKInterfaceGeoIOHDF5 


# Denpends - Interface
FITKInterfaceModel.depends = FITKCore
FITKInterfaceIO.depends = FITKCore
FITKInterfaceGeometry.depends = FITKCore FITKAppFramework FITKInterfaceModel
FITKInterfaceGeoIOHDF5.depends = FITKCore FITKAdaptor FITKAppFramework FITKInterfaceIO FITKInterfaceGeometry