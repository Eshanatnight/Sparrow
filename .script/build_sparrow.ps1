# Write the build files with cmake
# MSVC Used
Write-Output "This script easily makes the compiler with cmake.";
Write-Output "Calls the cmake command using the build directory as '.\out'. ";
cmake -S . -B .\out;

# Initalize the build process with cmake
Write-Output "Cmake Build Initiated... ";
cmake --build .\out;

Write-Output "Copying the Sparrow Binary to the root Directory... ";
Copy-Item .\out\Sparrow\Debug\Sparrow.exe .\