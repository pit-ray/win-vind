cd libs
@if not exist json (
    git clone https://github.com/nlohmann/json.git -b v3.9.1 --depth=1
    powershell cp -r json/single_include/nlohmann .
)
cd ..
@echo Installed nlohmann/json successfully
