cd test

@if not exist build (
    mkdir build
)
cd build
cmake ..
cmake --build . --config Debug
cd ../..

"test/build/Debug/core-test.exe"
