@if not exist libs (
    mkdir libs
)
cd libs

@if not exist openssl (
    git clone https://github.com/openssl/openssl.git -j %NUMBER_OF_PROCESSORS% --depth=1
) else (
    cd openssl
    git pull
    cd ..
)

cd openssl
git submodule update --init -j %NUMBER_OF_PROCESSORS%

@if "%1" == "-msvc" (
    @if "%2" == "" (
        @set BUILD_32=TRUE
        @set BUILD_64=TRUE
    ) else (
        @if "%2" == "32" (
            @set BUILD_32=TRUE

        ) else (
            @set BUILD_64=TRUE
        )
    )

    @if defined BUILD_64 (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
        @if not exist x64 (
            mkdir x64
        )
        perl Configure VC-WIN64A no-asm no-shared --prefix=/openssl/x64 --openssldir=/openssl/x64
        nmake
        nmake install
    )

    @if defined BUILD_32 (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
        @if not exist x86 (
            mkdir x86
        )
        perl Configure VC-WIN32 no-asm no-shared --prefix=/openssl/x86 --openssldir=/openssl/x86
        nmake
        nmake install
    )
) else (
    echo a
)

cd ../..
