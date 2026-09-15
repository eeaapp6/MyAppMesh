SET "PATH=C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\bin\;%PATH%"

lupdate.exe GUIDialog -no-obsolete -ts Resource\translations\Translate_GUIDialog_EN.ts
lupdate.exe GUIDialog -no-obsolete -ts Resource\translations\Translate_GUIDialog_CN.ts
lupdate.exe GUIFrame -no-obsolete -ts Resource\translations\Translate_GUIFrame_EN.ts
lupdate.exe GUIFrame -no-obsolete -ts Resource\translations\Translate_GUIFrame_CN.ts
lupdate.exe GUIWidget -no-obsolete -ts Resource\translations\Translate_GUIWidget_EN.ts
lupdate.exe GUIWidget -no-obsolete -ts Resource\translations\Translate_GUIWidget_CN.ts
lupdate.exe OperatorsModel -no-obsolete -ts Resource\translations\Translate_OperatorsModel_EN.ts
lupdate.exe OperatorsModel -no-obsolete -ts Resource\translations\Translate_OperatorsModel_CN.ts
lupdate.exe OperatorsGUI -no-obsolete -ts Resource\translations\Translate_OperatorsGUI_EN.ts
lupdate.exe OperatorsGUI -no-obsolete -ts Resource\translations\Translate_OperatorsGUI_CN.ts

pause