@if not defined NUMBER_OF_PROCESSORS (
    @echo Error: This script will not work with powershell.
    exit
)

@if "%1" == "" (
    @echo.
    @echo Error: Please pass your compiler type -mingw or -msvc as the first argument.
    @echo.
    exit
)

@echo The number of processors is %NUMBER_OF_PROCESSORS%.

@chcp 65001

@set LIBS_DIR=libs
@if not exist %LIBS_DIR% (
    @mkdir %LIBS_DIR%
)
cd %LIBS_DIR%

@echo nlohmann/json ----------------------------------------------------------------------
@if not exist nlohmann (
    mkdir nlohmann
)
cd nlohmann
@if not exist json.hpp (
    @echo Downloading now from GitHub...
    powershell Invoke-WebRequest https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp -OutFile json.hpp -UseBasicParsing
)
cd ..

@echo.
@echo wxWidgets --------------------------------------------------------------------------
@echo.
@set wxWidgets_VERSION=3.0.5
@rem set wxWidgets_VERSION=3.1.4-rc1

@if not exist wxWidgets (
    git clone https://github.com/wxWidgets/wxWidgets.git -b v%wxWidgets_VERSION% -j %NUMBER_OF_PROCESSORS% --depth=1
    cd wxWidgets
    git submodule update --init -j %NUMBER_OF_PROCESSORS%
    cd ..
) 
cd wxWidgets/build/msw

@if "%1" == "-msvc" (
    @if exist ../../lib/vc* (
        powershell rm -r "..\\..\\lib\\vc*"
    )

    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    nmake /f makefile.vc BUILD=release SHARED=0 UNICODE=1 TARGET_CPU=X64 RUNTIME_LIBS=static

    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
    nmake /f makefile.vc BUILD=release SHARED=0 UNICODE=1 TARGET_CPU=X86 RUNTIME_LIBS=static

) else (
    @if exist ../../lib/gcc* (
       powershell rm -r "..\\..\\lib\\gcc*"
    )

   @echo - Build for 64bit -------------------------------------------------------------
   mingw32-make -j %NUMBER_OF_PROCESSORS% -f makefile.gcc setup_h BUILD=release SHARED=0 UNICODE=1 CPPFLAGS="-m64 -w"
   mingw32-make -j %NUMBER_OF_PROCESSORS% -f makefile.gcc BUILD=release SHARED=0 UNICODE=1 CPPFLAGS="-m64 -w"
   @rem powershell mv "..\\..\\lib\\gcc_lib" "..\\..\\lib\\gcc_x64_lib"

   @rem powershell rm -r gcc_mswu*
   @rem @echo - Build for 32bit -------------------------------------------------------------
   @rem mingw32-make -j %NUMBER_OF_PROCESSORS% -f makefile.gcc setup_h BUILD=release SHARED=0 UNICODE=1 CPPFLAGS="-m32 -w"
   @rem mingw32-make -j %NUMBER_OF_PROCESSORS% -f makefile.gcc BUILD=release SHARED=0 UNICODE=1 CPPFLAGS="-m32 -w"
)

cd ../../../
cd ..

@rem If you use wxWidgets-3.1.x, it solves linker error.
@rem if "%1" == "-mingw" (
@rem    call copy_mingw_libs.bat
)

@echo done.
