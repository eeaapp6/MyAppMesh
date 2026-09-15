@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

echo ========================================
echo       Starting DLL Copy Process
echo ========================================

:: Get current script directory
set "CURRENT_DIR=%~dp0"
set "CURRENT_DIR=%CURRENT_DIR:~0,-1%"

:: Set target directories
set "OUTPUT_DIR=%CURRENT_DIR%\output"
set "BIN_DIR=%OUTPUT_DIR%\bin"
set "BIN_D_DIR=%OUTPUT_DIR%\bin_d"

:: Create output directories
echo Creating output directories...
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"
if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"
if not exist "%BIN_D_DIR%" mkdir "%BIN_D_DIR%"

echo Output directories created successfully
echo.

echo.
echo ========================================
echo   Starting Qt DLL File Copy
echo ========================================

:: Set Qt DLL path
set "QT_BIN_DIR=C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\bin"
echo Qt DLL path: %QT_BIN_DIR%

:: Check if Qt directory exists
if not exist "%QT_BIN_DIR%" (
    echo [ERROR] Qt directory does not exist: %QT_BIN_DIR%
) else (
    :: Copy Qt DLL to bin directory
    copy /y "%QT_BIN_DIR%\d3dcompiler_47.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\libEGL.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\libGLESv2.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\opengl32sw.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5Core.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5Gui.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5Multimedia.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5MultimediaWidgets.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5Network.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5OpenGL.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5PrintSupport.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5Qml.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5QmlModels.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5Quick.dll" "%BIN_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5QuickWidgets.dll" "%BIN_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5Sql.dll" "%BIN_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5Svg.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5Widgets.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5Xml.dll" "%BIN_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5XmlPatterns.dll" "%BIN_DIR%"

    :: Copy Qt DLL to bin_d directory
    copy /y "%QT_BIN_DIR%\d3dcompiler_47.dll" "%BIN_D_DIR%\"
    copy /y "%QT_BIN_DIR%\libEGLd.dll" "%BIN_D_DIR%\"
    copy /y "%QT_BIN_DIR%\libGLESv2d.dll" "%BIN_D_DIR%\"
    copy /y "%QT_BIN_DIR%\opengl32sw.dll" "%BIN_D_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5Cored.dll" "%BIN_D_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5Guid.dll" "%BIN_D_DIR%\"
    copy /y "%QT_BIN_DIR%\Qt5Multimediad.dll" "%BIN_D_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5MultimediaWidgetsd.dll" "%BIN_D_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5Networkd.dll" "%BIN_D_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5OpenGLd.dll" "%BIN_D_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5PrintSupportd.dll" "%BIN_D_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5Qmld.dll" "%BIN_D_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5QmlModelsd.dll" "%BIN_D_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5Quickd.dll" "%BIN_D_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5QuickWidgetsd.dll" "%BIN_D_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5Sqld.dll" "%BIN_D_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5Svgd.dll" "%BIN_D_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5Widgetsd.dll" "%BIN_D_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5Xmld.dll" "%BIN_D_DIR%"
    copy /y "%QT_BIN_DIR%\Qt5XmlPatternsd.dll" "%BIN_D_DIR%"
)

echo.
echo ========================================
echo   Starting Qt Plugin File Copy
echo ========================================

:: Set Qt plugins path
set "QT_PLUGINS_DIR=%QT_BIN_DIR%\..\plugins"
echo Qt plugins path: %QT_PLUGINS_DIR%

:: Check if Qt plugins directory exists
if not exist "%QT_PLUGINS_DIR%" (
    echo [ERROR] Qt plugins directory does not exist: %QT_PLUGINS_DIR%
) else (
    :: Copy platforms plugin to bin directory
    echo.
    echo Copying platforms plugin to bin\platforms...
    if not exist "%BIN_DIR%\platforms" mkdir "%BIN_DIR%\platforms"
    xcopy "%QT_PLUGINS_DIR%\platforms" "%BIN_DIR%\platforms\" /e /i /y /q
    if %errorlevel% equ 0 (
        echo platforms plugin copied to bin successfully
    ) else (
        echo [WARNING] platforms plugin copy to bin may have issues
    )

    :: Copy platforms plugin to bin_d directory
    echo.
    echo Copying platforms plugin to bin_d\platforms...
    if not exist "%BIN_D_DIR%\platforms" mkdir "%BIN_D_DIR%\platforms"
    xcopy "%QT_PLUGINS_DIR%\platforms" "%BIN_D_DIR%\platforms\" /e /i /y /q
    if %errorlevel% equ 0 (
        echo platforms plugin copied to bin_d successfully
    ) else (
        echo [WARNING] platforms plugin copy to bin_d may have issues
    )

    :: Copy imageformats plugin to bin directory
    echo.
    echo Copying imageformats plugin to bin\imageformats...
    if not exist "%BIN_DIR%\imageformats" mkdir "%BIN_DIR%\imageformats"
    xcopy "%QT_PLUGINS_DIR%\imageformats" "%BIN_DIR%\imageformats\" /e /i /y /q
    if %errorlevel% equ 0 (
        echo imageformats plugin copied to bin successfully
    ) else (
        echo [WARNING] imageformats plugin copy to bin may have issues
    )

    :: Copy imageformats plugin to bin_d directory
    echo.
    echo Copying imageformats plugin to bin_d\imageformats...
    if not exist "%BIN_D_DIR%\imageformats" mkdir "%BIN_D_DIR%\imageformats"
    xcopy "%QT_PLUGINS_DIR%\imageformats" "%BIN_D_DIR%\imageformats\" /e /i /y /q
    if %errorlevel% equ 0 (
        echo imageformats plugin copied to bin_d successfully
    ) else (
        echo [WARNING] imageformats plugin copy to bin_d may have issues
    )
)

echo.
echo ========================================
echo   Starting OCC DLL File Copy
echo ========================================

:: Set OCC DLL paths
set "OCC_BIN_DIR=%CURRENT_DIR%\Tools\Win64\OCC\bin"
set "OCC_BIND_DIR=%CURRENT_DIR%\Tools\Win64\OCC\bind"

echo OCC bin path: %OCC_BIN_DIR%
echo OCC bind path: %OCC_BIND_DIR%

:: Check OCC bin directory
if exist "%OCC_BIN_DIR%" (
    echo.
    echo Copying OCC bin DLL to output\bin...
    xcopy "%OCC_BIN_DIR%\*.dll" "%BIN_DIR%\" /y /q
    if !errorlevel! equ 0 (
        echo OCC bin DLL copied successfully
    ) else (
        echo [WARNING] OCC bin DLL copy may have issues
    )
) else (
    echo [ERROR] OCC bin directory does not exist: %OCC_BIN_DIR%
)

:: Check OCC bind directory
if exist "%OCC_BIND_DIR%" (
    echo.
    echo Copying OCC bind DLL to output\bin_d...
    xcopy "%OCC_BIND_DIR%\*.dll" "%BIN_D_DIR%\" /y /q
    if !errorlevel! equ 0 (
        echo OCC bind DLL copied successfully
    ) else (
        echo [WARNING] OCC bind DLL copy may have issues
    )
) else (
    echo [ERROR] OCC bind directory does not exist: %OCC_BIND_DIR%
)

echo ========================================
echo   Starting dime DLL File Copy
echo ========================================

:: Set dime DLL paths
set "dime_BIN_DIR=%CURRENT_DIR%\Tools\Win64\dime\bin"
set "dime_BIND_DIR=%CURRENT_DIR%\Tools\Win64\dime\bind"

echo dime bin path: %dime_BIN_DIR%
echo dime bind path: %dime_BIND_DIR%

:: Check dime bin directory
if exist "%dime_BIN_DIR%" (
    echo.
    echo Copying dime bin DLL to output\bin...
    xcopy "%dime_BIN_DIR%\*.dll" "%BIN_DIR%\" /y /q
    if !errorlevel! equ 0 (
        echo dime bin DLL copied successfully
    ) else (
        echo [WARNING] dime bin DLL copy may have issues
    )
) else (
    echo [ERROR] dime bin directory does not exist: %dime_BIN_DIR%
)

:: Check dime bind directory
if exist "%dime_BIND_DIR%" (
    echo.
    echo Copying dime bind DLL to output\bin_d...
    xcopy "%dime_BIND_DIR%\*.dll" "%BIN_D_DIR%\" /y /q
    if !errorlevel! equ 0 (
        echo dime bind DLL copied successfully
    ) else (
        echo [WARNING] dime bind DLL copy may have issues
    )
) else (
    echo [ERROR] dime bind directory does not exist: %dime_BIND_DIR%
)

echo.
echo ========================================
echo   Starting hdf5 DLL File Copy
echo ========================================

:: Set hdf5 DLL paths
set "hdf5_BIN_DIR=%CURRENT_DIR%\Tools\Win64\hdf5\bin"
set "hdf5_BIND_DIR=%CURRENT_DIR%\Tools\Win64\hdf5\bind"

echo hdf5 bin path: %hdf5_BIN_DIR%
echo hdf5 bind path: %hdf5_BIND_DIR%

:: Check hdf5 bin directory
if exist "%hdf5_BIN_DIR%" (
    echo.
    echo Copying hdf5 bin DLL to output\bin...
    xcopy "%hdf5_BIN_DIR%\*.dll" "%BIN_DIR%\" /y /q
    if !errorlevel! equ 0 (
        echo hdf5 bin DLL copied successfully
    ) else (
        echo [WARNING] hdf5 bin DLL copy may have issues
    )
) else (
    echo [ERROR] hdf5 bin directory does not exist: %hdf5_BIN_DIR%
)

:: Check hdf5 bind directory
if exist "%hdf5_BIND_DIR%" (
    echo.
    echo Copying hdf5 bind DLL to output\bin_d...
    xcopy "%hdf5_BIND_DIR%\*.dll" "%BIN_D_DIR%\" /y /q
    if !errorlevel! equ 0 (
        echo hdf5 bind DLL copied successfully
    ) else (
        echo [WARNING] hdf5 bind DLL copy may have issues
    )
) else (
    echo [ERROR] hdf5 bind directory does not exist: %hdf5_BIND_DIR%
)

echo.
echo ========================================
echo   Starting PythonQt DLL File Copy
echo ========================================

:: Set PythonQt DLL paths
set "PythonQt_BIN_DIR=%CURRENT_DIR%\Tools\Win64\PythonQt\bin"
set "PythonQt_BIND_DIR=%CURRENT_DIR%\Tools\Win64\PythonQt\bind"

echo PythonQt bin path: %PythonQt_BIN_DIR%
echo PythonQt bind path: %PythonQt_BIND_DIR%

:: Check PythonQt bin directory
if exist "%PythonQt_BIN_DIR%" (
    echo.
    echo Copying PythonQt bin DLL to output\bin...
    xcopy "%PythonQt_BIN_DIR%\*.dll" "%BIN_DIR%\" /y /q
    if !errorlevel! equ 0 (
        echo PythonQt bin DLL copied successfully
    ) else (
        echo [WARNING] PythonQt bin DLL copy may have issues
    )
) else (
    echo [ERROR] PythonQt bin directory does not exist: %PythonQt_BIN_DIR%
)

:: Check PythonQt bind directory
if exist "%PythonQt_BIND_DIR%" (
    echo.
    echo Copying PythonQt bind DLL to output\bin_d...
    xcopy "%PythonQt_BIND_DIR%\*.dll" "%BIN_D_DIR%\" /y /q
    if !errorlevel! equ 0 (
        echo PythonQt bind DLL copied successfully
    ) else (
        echo [WARNING] PythonQt bind DLL copy may have issues
    )
) else (
    echo [ERROR] PythonQt bind directory does not exist: %PythonQt_BIND_DIR%
)

echo ========================================
echo   Starting Python DLL File Copy
echo ========================================

:: Set Python paths
set "PYTHON_DIR=%CURRENT_DIR%\Tools\Win64\Python37"
set "PYTHON_DLLS_DIR=%PYTHON_DIR%\DLLs"
set "PYTHON_LIB_DIR=%PYTHON_DIR%\Lib"

echo Python directory: %PYTHON_DIR%
echo Python DLLs directory: %PYTHON_DLLS_DIR%
echo Python Lib directory: %PYTHON_LIB_DIR%

:: Check Python directory
if exist "%PYTHON_DIR%" (
    echo.
    echo Copying Python DLLs to output\bin...
    copy /y "%PYTHON_DIR%\python3.dll" "%BIN_DIR%\" 2>nul
    copy /y "%PYTHON_DIR%\python37.dll" "%BIN_DIR%\" 2>nul
    copy /y "%PYTHON_DIR%\vcruntime140.dll" "%BIN_DIR%\" 2>nul
    if !errorlevel! equ 0 (
        echo Python DLLs copied to bin successfully
    ) else (
        echo [WARNING] Python DLLs copy to bin may have issues
    )

    echo.
    echo Copying Python DLLs to output\bin_d...
    copy /y "%PYTHON_DIR%\python3.dll" "%BIN_D_DIR%\" 2>nul
    copy /y "%PYTHON_DIR%\python37.dll" "%BIN_D_DIR%\" 2>nul
    copy /y "%PYTHON_DIR%\vcruntime140.dll" "%BIN_D_DIR%\" 2>nul
    if !errorlevel! equ 0 (
        echo Python DLLs copied to bin_d successfully
    ) else (
        echo [WARNING] Python DLLs copy to bin_d may have issues
    )

    :: Copy Python DLLs folder
    if exist "%PYTHON_DLLS_DIR%" (
        echo.
        echo Copying Python DLLs folder to output\bin\DLLs...
        if not exist "%BIN_DIR%\DLLs" mkdir "%BIN_DIR%\DLLs"
        xcopy "%PYTHON_DLLS_DIR%\*" "%BIN_DIR%\DLLs\" /e /i /y /q
        if !errorlevel! equ 0 (
            echo Python DLLs folder copied to bin successfully
        ) else (
            echo [WARNING] Python DLLs folder copy to bin may have issues
        )

        echo.
        echo Copying Python DLLs folder to output\bin_d\DLLs...
        if not exist "%BIN_D_DIR%\DLLs" mkdir "%BIN_D_DIR%\DLLs"
        xcopy "%PYTHON_DLLS_DIR%\*" "%BIN_D_DIR%\DLLs\" /e /i /y /q
        if !errorlevel! equ 0 (
            echo Python DLLs folder copied to bin_d successfully
        ) else (
            echo [WARNING] Python DLLs folder copy to bin_d may have issues
        )
    ) else (
        echo [ERROR] Python DLLs directory does not exist: %PYTHON_DLLS_DIR%
    )

    :: Copy Python Lib folder
    if exist "%PYTHON_LIB_DIR%" (
        echo.
        echo Copying Python Lib folder to output\bin\Lib...
        if not exist "%BIN_DIR%\Lib" mkdir "%BIN_DIR%\Lib"
        xcopy "%PYTHON_LIB_DIR%\*" "%BIN_DIR%\Lib\" /e /i /y /q
        if !errorlevel! equ 0 (
            echo Python Lib folder copied to bin successfully
        ) else (
            echo [WARNING] Python Lib folder copy to bin may have issues
        )

        echo.
        echo Copying Python Lib folder to output\bin_d\Lib...
        if not exist "%BIN_D_DIR%\Lib" mkdir "%BIN_D_DIR%\Lib"
        xcopy "%PYTHON_LIB_DIR%\*" "%BIN_D_DIR%\Lib\" /e /i /y /q
        if !errorlevel! equ 0 (
            echo Python Lib folder copied to bin_d successfully
        ) else (
            echo [WARNING] Python Lib folder copy to bin_d may have issues
        )
    ) else (
        echo [ERROR] Python Lib directory does not exist: %PYTHON_LIB_DIR%
    )
) else (
    echo [ERROR] Python directory does not exist: %PYTHON_DIR%
)

echo.
echo ========================================
echo   Copy Process Completed!
echo Output directory: %OUTPUT_DIR%
echo   - bin: %BIN_DIR%
echo   - bin_d: %BIN_D_DIR%
echo ========================================
pause