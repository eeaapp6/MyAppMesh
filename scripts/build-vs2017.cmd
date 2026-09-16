@echo off
setlocal

set "VCVARS=%VS2017_VCVARS%"
if defined VCVARS if exist "%VCVARS%" goto have_vcvars

for /f "delims=" %%I in ('where vswhere.exe 2^>nul') do if not defined VSWHERE set "VSWHERE=%%I"
if defined VSWHERE for /f "usebackq delims=" %%I in (`"%VSWHERE%" -version [15.0^,16.0^) -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS2017_ROOT=%%I"
if defined VS2017_ROOT set "VCVARS=%VS2017_ROOT%\VC\Auxiliary\Build\vcvarsall.bat"
if exist "%VCVARS%" goto have_vcvars

echo Visual Studio 2017 was not discovered.
echo Set VS2017_VCVARS to the vcvarsall.bat file and retry.
exit /b 1

:have_vcvars

if not defined WINDOWS_SDK_VERSION set "WINDOWS_SDK_VERSION=10.0.17763.0"
call "%VCVARS%" x64 %WINDOWS_SDK_VERSION%
if errorlevel 1 exit /b %errorlevel%

if not defined QT_ROOT for /f "delims=" %%I in ('qmake -query QT_INSTALL_PREFIX 2^>nul') do set "QT_ROOT=%%I"
if not defined QT_ROOT (
    echo Qt was not discovered.
    echo Add qmake to PATH or set QT_ROOT to the Qt 5.14.2 msvc2017_64 directory.
    exit /b 1
)
if not defined CMAKE_EXE set "CMAKE_EXE=cmake"
if not defined CTEST_EXE set "CTEST_EXE=ctest"
set "REPO_ROOT=%~dp0.."
set "BUILD_DIR=%REPO_ROOT%\build\vs2017-x64-debug"
set "TOOLS_DIR=%REPO_ROOT%\dependencies\FastCAECodeBase\Tools"
set "FITK_OUTPUT_DIR=%REPO_ROOT%\output"

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
