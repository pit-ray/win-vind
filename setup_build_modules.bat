@chcp 65001

@if not exist cmake_modules (
    mkdir cmake_modules
)

cd cmake_modules
@if not exist cotire.cmake (
    powershell Invoke-WebRequest https://github.com/sakra/cotire/releases/download/cotire-1.8.1/cotire.cmake -OutFile cotire.cmake -UseBasicParsing
)
cd ..
