win32{

INCLUDEPATH += ../Tools/Win64/SogouWorkflow/include/ \

INCLUDEPATH += ../Tools/Win64/OpenSSL-Win64/include/ \
    
Debug:LIBS +=  -L../Tools/Win64/SogouWorkflow/libd \
                    -lworkflow    \
                -L../Tools/Win64/OpenSSL-Win64/lib/VC/x64/MTd \
                    -llibssl_static \
                    -llibcrypto_static \
                    -lWs2_32 \
                    -lCrypt32 \
                    -lMswsock \
                    
Release:LIBS += -L../Tools/Win64/SogouWorkflow/lib \
                    -lworkflow      \ 
                 -L../Tools/Win64/OpenSSL-Win64/lib/VC/x64/MTd \
                    -llibssl_static \
                    -llibcrypto_static \
                    -lWs2_32 \
                    -lCrypt32 \
                    -lMswsock \


}

unix{

INCLUDEPATH +=  ../../Tools/Linux64/qwt/include \

LIBS +=   -L../../Tools/Linux64/qwt/lib \
              -lqwt     \

}