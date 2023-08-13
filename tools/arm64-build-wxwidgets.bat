# taken from https://github.com/pit-ray/win-vind/issues/191#issuecomment-1671656805

# Assume wxwidgets source already available. For fresh copy use
#    .\tools\arm65-clean-checkout-wxwidgets.bat

cd libs
if %errorlevel% neq 0 exit /b %errorlevel%

cd wxWidgets
if %errorlevel% neq 0 exit /b %errorlevel%
cd build/msw
if %errorlevel% neq 0 exit /b %errorlevel%
rem call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsamd64_arm64.bat"
rem if %errorlevel% neq 0 exit /b %errorlevel%
call "c:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat"
if %errorlevel% neq 0 exit /b %errorlevel%
nmake /f makefile.vc BUILD=release SHARED=0 UNICODE=1 TARGET_CPU=ARM64 RUNTIME_LIBS=static
if %errorlevel% neq 0 exit /b %errorlevel%
cd ../../../
if %errorlevel% neq 0 exit /b %errorlevel%
