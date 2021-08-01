cd libs
@if not exist maddy (
    git clone https://github.com/progsource/maddy.git -b 1.1.2 --depth=1 maddy_src
    powershell cp -r maddy_src/include/maddy .
)
cd ..
@echo Installed maddy successfully
