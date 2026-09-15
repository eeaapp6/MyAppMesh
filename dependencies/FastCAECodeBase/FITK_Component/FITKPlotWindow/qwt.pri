
win32{

INCLUDEPATH += ../../Tools/Win64/qwt/include \
    
Debug:LIBS +=  -L../../Tools/Win64/qwt/libd \
                    -lqwtd    \             
                    
Release:LIBS += -L../../Tools/Win64/qwt/lib \
                    -lqwt      \              

}

unix{

INCLUDEPATH +=  ../../Tools/Linux64/qwt/include \

LIBS +=   -L../../Tools/Linux64/qwt/lib \
              -lqwt     \

}
