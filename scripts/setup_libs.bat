@chcp 65001
@echo Usage: setup_libs.bat [-msvc/-mingw] [32/64] [-update (optional)]

@if not defined NUMBER_OF_PROCESSORS (
    @echo Error: This script will not work with powershell.
    @exit
)

@if "%1" == "" (
    @echo.
    @echo Error: Please pass your compiler type -mingw or -msvc as the first argument.
    @echo.
    @exit
) else if "%1" == "-help" (
    @exit
)

@echo The number of processors is %NUMBER_OF_PROCESSORS%.

@if not exist libs (
    @mkdir libs
)

@echo Setup nlohmann/json ----------------------------------------------------------------------
@if exist libs/nlohmann (
    @if "%3" == "-update" (
        @goto start_setup_nlohmann_json
    )
    @echo.
    @echo nlohmann/json is already installed. If you want to re-install, pass -update as the third argument.
    @echo Syntax: setup_libs.bat [-mingw/-msvc] [32/64] [-update]
    @echo.
    @goto end_of_nlohmann_json
)
:start_setup_nlohmann_json
call "scripts/install_nlohmann_json.bat" %1 %2
:end_of_nlohmann_json

@echo Setup maddy ----------------------------------------------------------------------
@if exist libs/maddy (
    @if "%3" == "-update" (
        @goto start_setup_maddy
    )
    @echo.
    @echo maddy is already installed. If you want to re-install, pass -update as the third argument.
    @echo Syntax: setup_libs.bat [-mingw/-msvc] [32/64] [-update]
    @echo.
    @goto end_of_setup_maddy
)
:start_setup_maddy
call "scripts/install_maddy.bat" %1 %2
:end_of_setup_maddy

@echo.
@echo Setup wxWidgets --------------------------------------------------------------------------
@echo.

@if exist libs/wxWidgets (
    @if "%3" == "-update" (
        @goto start_setup_wxWidgets
    )
    @echo.
    @echo wxWidgets is already installed. If you want to re-install, pass -update as the third argument.
    @echo Syntax: setup_libs.bat [-mingw/-msvc] [32/64] [-update]
    @echo.
    @goto end_of_wxWidgets
)
:start_setup_wxWidgets
call "scripts/install_wxWidgets.bat" %1 %2
:end_of_wxWidgets
