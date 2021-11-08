cd tests
@if not exist fff (
    mkdir fff
    cd fff
    @echo Downloading now from GitHub...
    powershell Invoke-WebRequest https://github.com/meekrosoft/fff/releases/download/v1.1/fff.h -OutFile fff.h -UseBasicParsing
)
cd ../..
