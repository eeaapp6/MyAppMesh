
call "E:\vs2017\VC\Auxiliary\Build\vcvarsall.bat" x64 10.0.17763.0

SET "E:\QT\5.14.2\msvc2017_64\bin\;E:\vs2017\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64;%PATH%"

qmake -r -tp vc FlowApp.pro

pause
