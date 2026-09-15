TEMPLATE = subdirs

SUBDIRS += \
    $$PWD/FITKRadiossData \
    $$PWD/FITKRadiossRadIO \
    $$PWD/FITKCompMessageWidget \
    $$PWD/FITKWidget \
    $$PWD/FITKRenderWindowVTK \
    $$PWD/FITKGeoCompOCC \
    $$PWD/FITKPlotWindow \
    $$PWD/FITKGmshExeDriver \
    $$PWD/FITKGmshMshIO \
    $$PWD/FITKRadiossLSDynaTransfer \
    $$PWD/FITKGeoOCCIOHDF5 \
    $$PWD/FITKRadiossDataIOHDF5 \
    $$PWD/FITKMeshIO \
#    $$PWD/FITKNastranBDFIO \
    
CONFIG += ordered
