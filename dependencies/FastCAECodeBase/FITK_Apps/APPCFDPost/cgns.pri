win32{

INCLUDEPATH +=  $$PWD/Tools/Win64/cgns/include  \

Debug:LIBS += -L$$PWD/Tools/Win64/cgns/libd/ \
-lcgns \    
-lcgnsdll \
-llibhdf5_D \
-llibhdf5_cpp_D \
-llibhdf5_hl_D \
-llibhdf5_hl_cpp_D \
-llibhdf5_tools_D \

Release:LIBS += -L$$PWD/Tools/Win64/cgns/lib/ \
-lcgns \    
-lcgnsdll \
-llibhdf5 \
-llibhdf5_cpp \
-llibhdf5_hl \
-llibhdf5_hl_cpp \
-llibhdf5_tools \

}

unix{

INCLUDEPATH += ../../cgns/include

LIBS +=  -L../../cgns/lib \
-lcgns \  
-lhdf5 \
-lhdf5_cpp \
-lhdf5_hl \
-lhdf5_hl_cpp \
-lhdf5_tools

}
