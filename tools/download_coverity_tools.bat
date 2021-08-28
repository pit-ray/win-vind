@rem Assumed to be run on MSYS2

@if not exist cov_tools.zip (
    wget https://scan.coverity.com/download/cxx/win64 --post-data "token=%1&project=pit-ray%%2Fwin-vind" -O cov_tools.zip
)
