SET "PATH=C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\bin\;%PATH%"

lupdate.exe FITKRenderWindowOCC.pro -no-obsolete -ts Resource\translations\Translate_FITKRenderWindowOCC_EN.ts
lupdate.exe FITKRenderWindowOCC.pro -no-obsolete -ts Resource\translations\Translate_FITKRenderWindowOCC_CN.ts

pause