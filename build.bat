@chcp 65001
echo [cmake] _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _

@if "%1" == "" (
    @goto debug
)
@if %1 == -debug (
    @goto debug
)
@if %1 == -release (
    @goto release
)
@if %1 == --debug (
    @goto debug
)
@if %1 == --release (
    @goto release
)
if %1 == -D (
    @goto debug
)
if %1 == -R (
    @goto release
)

:debug
    @if not exist debug (
        @mkdir debug
    )
    cd debug

    cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" ..
    cmake --build . --config Debug
    cd ..
    @goto exit

:release
    @if not exist release (
        @mkdir release
    ) 
    cd release

    cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ..
    cmake --build . --config Release
    cd ..
    @goto exit

:exit
