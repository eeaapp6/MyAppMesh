TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS += \
    FITKCore \ 
    FITKAppFramework \
    FITKPython \
    FITKAdaptor \
    FITKEasyParam
    
  
# Denpends - Kernel
# FITKCore.depends = 
FITKAppFramework.depends = FITKCore
FITKPython.depends = FITKCore FITKAppFramework
FITKAdaptor.depends = FITKCore
FITKEasyParam.depends = FITKCore