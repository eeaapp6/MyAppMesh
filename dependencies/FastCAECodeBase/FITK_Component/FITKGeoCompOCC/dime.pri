win32{

DEFINES    +=   DIME_DLL

INCLUDEPATH +=  ../../Tools/Win64/dime/include \
    
Debug:LIBS +=  -L../../Tools/Win64/dime/libd \
                    -ldime0d  \        
 

Release:LIBS +=  -L../../Tools/Win64/dime/lib  \
                      -ldime0  \        
                      
}

unix{

INCLUDEPATH +=  ../../Tools/Linux64/dime/include \

LIBS +=  -L../../Tools/Linux64/dime/lib  \
                      -ldime  \        
                      
}
