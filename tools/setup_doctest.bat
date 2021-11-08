@echo.
@echo Setup doctest --------------------------------------------------------------------------
echo.

git clone https://github.com/onqtam/doctest.git -j %NUMBER_OF_PROCESSORS% --depth=1 tests/doctest

@echo done.
