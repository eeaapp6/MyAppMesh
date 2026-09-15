SET "PATH=C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\bin\;%PATH%"

lupdate.exe FITKCompMessageWidget.pro -no-obsolete -ts Resource\translations\Translate_FITKCompMessageWidget_EN.ts
lupdate.exe FITKCompMessageWidget.pro -no-obsolete -ts Resource\translations\Translate_FITKCompMessageWidget_CN.ts

pause