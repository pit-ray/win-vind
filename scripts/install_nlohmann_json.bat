cd libs
@if not exist nlohmann (
    mkdir nlohmann
)
cd nlohmann
@if not exist json.hpp (
    @echo Downloading now from GitHub...
    powershell Invoke-WebRequest https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp -OutFile json.hpp -UseBasicParsing
)
cd ../..
@echo Installed nlohmann/json successfully
