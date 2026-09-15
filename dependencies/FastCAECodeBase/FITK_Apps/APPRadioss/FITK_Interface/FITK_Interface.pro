TEMPLATE = subdirs

SUBDIRS += \ 
    $$PWD/FITKInterfaceModel \ 
    $$PWD/FITKInterfacePhysics \
    $$PWD/FITKInterfaceGeometry \ 
    $$PWD/FITKInterfaceMesh \
    $$PWD/FITKVTKAlgorithm \
    $$PWD/FITKInterfaceIO \
    $$PWD/FITKInterfaceMeshGen \
    $$PWD/FITKInterfaceStructuralPost \
    $$PWD/FITKInterfaceGeoIOHDF5 \
    
CONFIG += ordered
