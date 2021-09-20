@chcp 65001
@if "%1" == "" (
    @echo Error: Please call this script with the version as a first argument.
    @exit
)

cd choco
choco pack
choco push win-vind.%1.nupkg --source https://push.chocolatey.org/
cd ..
powershell Compress-Archive -Path "choco/*" -DestinationPath "choco/win-vind_%1_package_source.zip"
