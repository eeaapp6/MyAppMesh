@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "REPO_ROOT=%~dp0.."
set "BUILD_DIR=%REPO_ROOT%\build\vs2017-x64-debug"
set "TOOLS_DIR=%REPO_ROOT%\dependencies\FastCAECodeBase\Tools"
set "FITK_OUTPUT_DIR=%REPO_ROOT%\output"
if not defined WINDOWS_SDK_VERSION set "WINDOWS_SDK_VERSION=10.0.17763.0"

set "VCVARS=%VS2017_VCVARS%"
if defined VCVARS (
    echo Checking VS2017_VCVARS: !VCVARS!
    if exist "!VCVARS!" goto have_vcvars
    echo VS2017_VCVARS does not exist: !VCVARS!
    set "VCVARS="
)

set "VSWHERE="
for /f "delims=" %%I in ('where vswhere.exe 2^>nul') do if not defined VSWHERE set "VSWHERE=%%I"
if not defined VSWHERE if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if defined VSWHERE (
    echo Checking Visual Studio with: !VSWHERE!
    for /f "usebackq delims=" %%I in (`"!VSWHERE!" -version [15.0^,16.0^) -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do if not defined VS2017_ROOT set "VS2017_ROOT=%%I"
    if defined VS2017_ROOT set "VCVARS=!VS2017_ROOT!\VC\Auxiliary\Build\vcvarsall.bat"
    if defined VCVARS if exist "!VCVARS!" goto have_vcvars
)

if not defined VCVARS if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
if not defined VCVARS if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvarsall.bat"
if not defined VCVARS if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat"
if not defined VCVARS if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"
if defined VCVARS if exist "%VCVARS%" goto have_vcvars

echo Visual Studio 2017 with VC.Tools.x86.x64 was not discovered.
echo Checked: VS2017_VCVARS, PATH vswhere.exe, %ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe
echo Checked editions: Community, BuildTools, Professional, Enterprise under C:\Program Files (x86)\Microsoft Visual Studio\2017.
echo Install the VS2017 v141 C++ x64 tools or set VS2017_VCVARS to vcvarsall.bat.
exit /b 1

:have_vcvars
echo Using VS2017 environment: %VCVARS%
call "%VCVARS%" x64 %WINDOWS_SDK_VERSION%
if errorlevel 1 exit /b %errorlevel%
where cl >nul 2>&1 || (echo MSVC x64 compiler was not initialized. & exit /b 1)
if not exist "%WindowsSdkDir%Include\%WINDOWS_SDK_VERSION%" (
    echo Windows SDK !WINDOWS_SDK_VERSION! was not initialized from !VCVARS!.
    exit /b 1
)

set "QMAKE_EXE="
if defined QT_ROOT (
    echo Checking QT_ROOT: !QT_ROOT!
    if exist "!QT_ROOT!\bin\qmake.exe" set "QMAKE_EXE=!QT_ROOT!\bin\qmake.exe"
)
if not defined QMAKE_EXE for /f "delims=" %%I in ('where qmake.exe 2^>nul') do if not defined QMAKE_EXE set "QMAKE_EXE=%%I"
if not defined QMAKE_EXE if exist "C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\bin\qmake.exe" set "QMAKE_EXE=C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\bin\qmake.exe"
if not defined QMAKE_EXE (
    echo Qt was not discovered.
    echo Checked: QT_ROOT\bin\qmake.exe, PATH qmake.exe, C:\Qt\Qt5.14.2\5.14.2\msvc2017_64\bin\qmake.exe
    echo Install Qt 5.14.2 msvc2017_64 or set QT_ROOT.
    exit /b 1
)
for /f "usebackq delims=" %%I in (`"%QMAKE_EXE%" -query QT_INSTALL_PREFIX`) do set "QT_ROOT=%%I"
for /f "usebackq delims=" %%I in (`"%QMAKE_EXE%" -query QT_VERSION`) do set "QT_VERSION=%%I"
if not "!QT_VERSION!"=="5.14.2" (
    echo Unsupported Qt version !QT_VERSION! from !QMAKE_EXE!; expected 5.14.2.
    exit /b 1
)
echo %QT_ROOT% | findstr /i /c:"msvc2017_64" >nul || (
    echo Unsupported Qt target at !QT_ROOT!; expected msvc2017_64.
    exit /b 1
)
echo Using Qt %QT_VERSION%: %QT_ROOT%

if defined CMAKE_EXE (
    if not exist "!CMAKE_EXE!" (echo CMAKE_EXE does not exist: !CMAKE_EXE! & exit /b 1)
) else if exist "C:\Qt\QT_WK\Tools\CMake_64\bin\cmake.exe" (
    set "CMAKE_EXE=C:\Qt\QT_WK\Tools\CMake_64\bin\cmake.exe"
) else (
    for /f "delims=" %%I in ('where cmake.exe 2^>nul') do if not defined CMAKE_EXE set "CMAKE_EXE=%%I"
)
if not defined CMAKE_EXE (
    echo CMake was not discovered. Checked CMAKE_EXE, C:\Qt\QT_WK\Tools\CMake_64\bin\cmake.exe and PATH.
    exit /b 1
)
if defined CTEST_EXE (
    if not exist "!CTEST_EXE!" (echo CTEST_EXE does not exist: !CTEST_EXE! & exit /b 1)
) else if exist "C:\Qt\QT_WK\Tools\CMake_64\bin\ctest.exe" (
    set "CTEST_EXE=C:\Qt\QT_WK\Tools\CMake_64\bin\ctest.exe"
) else (
    for /f "delims=" %%I in ('where ctest.exe 2^>nul') do if not defined CTEST_EXE set "CTEST_EXE=%%I"
)
if not defined CTEST_EXE (
    echo CTest was not discovered. Checked CTEST_EXE, C:\Qt\QT_WK\Tools\CMake_64\bin\ctest.exe and PATH.
    exit /b 1
)
echo Using CMake: %CMAKE_EXE%
echo Using CTest: %CTEST_EXE%

"%CMAKE_EXE%" -S "%REPO_ROOT%" -B "%BUILD_DIR%" -G "Visual Studio 15 2017" -A x64 -T v141 -DCMAKE_SYSTEM_VERSION=%WINDOWS_SDK_VERSION% -DCMAKE_CONFIGURATION_TYPES=Debug -DTOOLS_DIR="%TOOLS_DIR%" -DFITK_OUTPUT_DIR="%FITK_OUTPUT_DIR%" -DQt5_DIR="%QT_ROOT%\lib\cmake\Qt5"
if errorlevel 1 exit /b %errorlevel%

"%CMAKE_EXE%" --build "%BUILD_DIR%" --config Debug
if errorlevel 1 exit /b %errorlevel%
"%CTEST_EXE%" --test-dir "%BUILD_DIR%" -C Debug --output-on-failure
if errorlevel 1 exit /b %errorlevel%

> "%BUILD_DIR%\release-negative-validation.log" echo APPMesh build policy: Debug only; the generated project contains no Release configuration.
"%CMAKE_EXE%" --build "%BUILD_DIR%" --config Release >> "%BUILD_DIR%\release-negative-validation.log" 2>&1
set "RELEASE_EXIT=%ERRORLEVEL%"
type "%BUILD_DIR%\release-negative-validation.log"
if "%RELEASE_EXIT%"=="0" (
    echo ERROR: Release build unexpectedly succeeded; APPMesh must remain Debug-only.
    exit /b 1
)

echo APPMesh Debug-only negative validation passed: Release rejected with exit code %RELEASE_EXIT%.
exit /b 0
