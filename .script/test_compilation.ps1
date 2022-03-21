# Tests
Write-Output "Sparrow is passed '-com' flag to compile and '-NASM' flag to generate the assembly code for NASM. ";
Write-Output "Testing helloworld.spar ....";
Write-Output ".\Sparrow.exe -com -NASM "".\helloworld.spar"" -o .\test\HelloWorld\helloworld";
.\Sparrow.exe -com -NASM ".\examples\helloworld.spar" -o .\test\HelloWorld\helloworld;
Write-Output "";
Write-Output "Testing fizzbuzz.spar ....";
Write-Output ".\Sparrow.exe -com -NASM "".\examples\fizzbuzz.spar"" -o .\test\Fizzbuzz\fizzbuzz";
.\Sparrow.exe -com -NASM ".\examples\fizzbuzz.spar" -o .\test\Fizzbuzz\fizzbuzz;
Write-Output "";
Write-Output "Testing rule110.spar ....";
Write-Output ".\Sparrow.exe -com -NASM "".\examples\rule110.spar"" -o .\test\Rule110\rule110";
.\Sparrow.exe -com -NASM ".\examples\rule110.spar" -o .\test\Rule110\rule110;
Write-Output "";
Write-Output "Testing write_to_file.spar ....";
Write-Output ".\Sparrow.exe -com -NASM "".\examples\write_to_file.spar"" -o .\test\WriteToFile\write_to_file";
.\Sparrow.exe -com -NASM ".\examples\write_to_file.spar" -o .\test\WriteToFile\write_to_file;