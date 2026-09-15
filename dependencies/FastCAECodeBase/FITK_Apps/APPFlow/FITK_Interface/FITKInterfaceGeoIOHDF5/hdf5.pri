win32{

DEFINES    +=  H5_BUILT_AS_DYNAMIC_LIB

INCLUDEPATH     +=  ../../Tools/Win64/hdf5/include/  \


Debug:LIBS +=  -L../../Tools/Win64/hdf5/libd/  \
                  -lhdf5_D  \
                  -lhdf5_cpp_D  \
                  -lhdf5_hl_D  \
                  -lhdf5_hl_cpp_D  \
                  -lhdf5_tools_D  


Release:LIBS +=  -L../../Tools/Win64/hdf5/lib/  \
                  -lhdf5  \
                  -lhdf5_cpp  \
                  -lhdf5_hl  \
                  -lhdf5_hl_cpp  \
                  -lhdf5_tools  
}

unix{
INCLUDEPATH     +=  ../../Tools/Linux64/hdf5/include/  \


LIBS +=  -L../../Tools/Linux64/hdf5/lib/  \
                  -lhdf5  \
                  -lhdf5_cpp  \
                  -lhdf5_hl  \
                  -lhdf5_hl_cpp  
                  #-lhdf5_tools  
}
