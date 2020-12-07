@chcp 65001
@if "%1" == "" (
    @echo [Error] Please call this script with the version as a first argument.
    @exit
)
rmdir /s /q release

call create_ins.bat
call create_zip.bat %1
