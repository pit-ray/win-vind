@REM  from https://github.com/pit-ray/win-vind/issues/191#issuecomment-1671656805

call "c:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat"
if %errorlevel% neq 0 exit /b %errorlevel%
cmake -B build_arm64 -DCMAKE_BUILD_TYPE=Release -DwxWidgets_LIB_DIR=libs/wxWidgets/lib/vc_arm64_lib -G "Visual Studio 16 2019" -A Arm64 .
if %errorlevel% neq 0 exit /b %errorlevel%
cmake --build build_arm64 -DwxWidgets_LIBRARIES=libs/wxWidgets/lib/vc_arm64_lib --config Release
if %errorlevel% neq 0 exit /b %errorlevel%
