# clean the wxWidgets

cd libs
if %errorlevel% neq 0 exit /b %errorlevel%

rmdir /s wxWidgets
if %errorlevel% neq 0 exit /b %errorlevel%

# get fresh copies
git clone https://github.com/wxWidgets/wxWidgets.git -b v3.1.5 -j %NUMBER_OF_PROCESSORS% --depth=1
if %errorlevel% neq 0 exit /b %errorlevel%

cd wxWidgets
if %errorlevel% neq 0 exit /b %errorlevel%
git submodule update --init -j %NUMBER_OF_PROCESSORS%
if %errorlevel% neq 0 exit /b %errorlevel%
