@echo off
setlocal

set "VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
if exist "%VCVARS%" goto have_vcvars
echo Visual Studio 2017 vcvarsall.bat was not found: %VCVARS%
exit /b 1

:have_vcvars

call "%VCVARS%" x64 10.0.17763.0
if errorlevel 1 exit /b %errorlevel%

if not defined QT_ROOT set "QT_ROOT=C:\Qt\Qt5.14.2\5.14.2\msvc2017_64"
if not defined CMAKE_EXE set "CMAKE_EXE=C:\Qt\QT_WK\Tools\CMake_64\bin\cmake.exe"
if not exist "%CMAKE_EXE%" set "CMAKE_EXE=cmake"
if not defined CTEST_EXE set "CTEST_EXE=C:\Qt\QT_WK\Tools\CMake_64\bin\ctest.exe"
if not exist "%CTEST_EXE%" set "CTEST_EXE=ctest"
set "REPO_ROOT=%~dp0.."
set "BUILD_DIR=%REPO_ROOT%\build\vs2017-x64"
set "TOOLS_DIR=%REPO_ROOT%\dependencies\FastCAECodeBase\Tools"

"%CMAKE_EXE%" -S "%REPO_ROOT%" -B "%BUILD_DIR%" -G "Visual Studio 15 2017" -A x64 -T v141 -DCMAKE_SYSTEM_VERSION=10.0.17763.0 -DTOOLS_DIR="%TOOLS_DIR%" -DQt5_DIR="%QT_ROOT%\lib\cmake\Qt5"
if errorlevel 1 exit /b %errorlevel%

"%CMAKE_EXE%" --build "%BUILD_DIR%" --config Debug
if errorlevel 1 exit /b %errorlevel%
"%CTEST_EXE%" --test-dir "%BUILD_DIR%" -C Debug --output-on-failure
if errorlevel 1 exit /b %errorlevel%

"%CMAKE_EXE%" --build "%BUILD_DIR%" --config Release
if errorlevel 1 exit /b %errorlevel%
"%CTEST_EXE%" --test-dir "%BUILD_DIR%" -C Release --output-on-failure
exit /b %errorlevel%
