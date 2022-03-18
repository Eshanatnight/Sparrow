Write-Content "This script easily makes the compiler with cmake. \n";
Write-Content "Calls the cmake command using the build directory as '.\out'. ";
cmake -S . -B .\out;

Write-Content "Cmake Build Initiated... \n";
cmake --build .\out;

Write-Content "Copying the Sparrow Binary to the root Directory... \n";
Copy-Item .\out\Sparrow\Debug\Sparrow.exe .\

Write-Content "Sparrow is passed '-com' flag to compile and '-NASM' flag to generate the assembly code for NASM. \n";
Write-Content "Testing helloworld.spar ....\n";
Write-Content ".\Sparrow.exe -com -NASM "".\helloworld.spar"" -o HW";
.\Sparrow.exe -com -NASM ".\examples\helloworld.spar" -o .\test\HelloWorld\helloworld;

Write-Content "";
Write-Content "Testing fizzbuzz.spar ....\n";
Write-Content ".\Sparrow.exe -com -NASM "".\examples\fizzbuzz.spar"" -o fizzbuzz";
.\Sparrow.exe -com -NASM ".\examples\fizzbuzz.spar" -o .\test\Fizzbuzz\fizzbuzz;