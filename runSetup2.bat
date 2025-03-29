@echo off
if not exist build (mkdir build)
if exist build (
	echo Y | rmdir /S build
	mkdir build
)
cd build
H:\cmake-3.31.4-windows-x86_64\bin\cmake ..
start Application.sln
cd ..
start assets
echo All done :3