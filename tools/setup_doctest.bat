@echo.
@echo Setup doctest --------------------------------------------------------------------------
echo.

cd test

git clone https://github.com/onqtam/doctest.git -j %NUMBER_OF_PROCESSORS% --depth=1 doctest

cd ..

@echo done.
