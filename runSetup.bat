@echo off
if not exist build (mkdir build)
if exist build (
	echo Y | rmdir /S build
	mkdir build
)
cd build
cmake ..
start Application.sln
cd ..
start assets
echo All done :3