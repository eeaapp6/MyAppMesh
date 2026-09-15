win32{

INCLUDEPATH += $$PWD\Tools\Win64\qwt\include/ \

Debug:LIBS +=  -L$$PWD\Tools\Win64\qwt\libd \
    -lqwtd \
    -lqwtpolard \

Release:LIBS += -L$$PWD\Tools\Win64\qwt\lib \
    -lqwt \
    -lqwtpolar \
}

unix{

INCLUDEPATH +=  ../../Qwt/include/ \

LIBS +=  -L../../Qwt/lib \
     -lqwt \
     -lqwtpolar \
}
