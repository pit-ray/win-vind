@echo.
@echo Setup doctest --------------------------------------------------------------------------
echo.

git clone https://github.com/onqtam/doctest.git -j %NUMBER_OF_PROCESSORS% -b 2.4.6 --depth=1 tests/doctest

@echo done.
