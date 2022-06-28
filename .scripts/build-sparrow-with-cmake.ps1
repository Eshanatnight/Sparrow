# Write the build files with cmake
# MSVC Used
Write-Output "This script easily makes the compiler with cmake.";
Write-Output "Calls the cmake command using the build directory as '.\out'. ";
cmake -S . -B .\out -DCMAKE_TOOLCHAIN_FILE="D:/tools/vcpkg/scripts/buildsystems/vcpkg.cmake";

# Initalize the build process with cmake
Write-Output "Cmake Build Initiated... ";
Write-Output "Default State: Debug";
cmake --build .\out;

Write-Output "Copying the Sparrow Binary to the root Directory... ";
Copy-Item .\out\Sparrow\Debug\Sparrow.exe .\bin