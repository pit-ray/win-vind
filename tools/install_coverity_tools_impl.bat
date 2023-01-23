if not exist cov_tools (
    wget https://scan.coverity.com/download/cxx/win64 --post-data "token=%1&project=pit-ray%%2Fwin-vind" -O cov_tools.zip
    unzip cov_tools.zip
    mv cov-analysis-win64-*.* cov_tools
)
