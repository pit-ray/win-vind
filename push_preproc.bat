@if "%1" == "" (
    @echo [Error] Please call this script with the version as a first argument.
    @exit
)

call create_ins.bat
call create_zip.bat %1

rmdir /q /s ".\\build"
rmdir /q /s ".\\release"
rmdir /q /s ".\\debug"
rmdir /q /s ".\\.vscode"
