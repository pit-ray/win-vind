@chcp 65001

cd docs

mkdir %1

xcopy /I /E /Y cheat_sheet\ %1\cheat_sheet
xcopy /I /E /Y downloads\ %1\downloads
xcopy /I /E /Y faq\ %1\faq
xcopy /I /E /Y usage\ %1\usage
copy /Y index.md %1\

cd ..

@echo Created new directory: %1
