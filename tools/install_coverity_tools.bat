@rem Assumed to be run on MSYS2

@if "%1" == "" (
    @echo Error: Invalid Coverity token
    @exit
)

@if not exist bin (
    mkdir bin
    cd bin
    wget https://scan.coverity.com/download/cxx/win64 --post-data "token=%1&project=pit-ray%%2Fwin-vind" -O cov_tools.zip
    unzip -j cov_tools.zip -d cov_tools
    cd ..
)
