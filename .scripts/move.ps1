# Move Sparrow Generated files to their respective folders
Move-Item .\test\HelloWorld\*.obj .\test\HelloWorld\Intermediates;
Move-Item .\test\HelloWorld\*.asm .\test\HelloWorld\Intermediates;
Move-Item .\test\HelloWorld\*.exe .\test\HelloWorld\bin;
Move-Item .\test\Fizzbuzz\*.obj .\test\Fizzbuzz\Intermediates;
Move-Item .\test\Fizzbuzz\*.asm .\test\Fizzbuzz\Intermediates;
Move-Item .\test\Fizzbuzz\*.exe .\test\Fizzbuzz\bin;
Move-Item .\test\Rule110\*.obj .\test\Rule110\Intermediates;
Move-Item .\test\Rule110\*.asm .\test\Rule110\Intermediates;
Move-Item .\test\Rule110\*.exe .\test\Rule110\bin;
Move-Item .\test\WriteToFile\*.obj .\test\WriteToFile\Intermediates;
Move-Item .\test\WriteToFile\*.asm .\test\WriteToFile\Intermediates;
Move-Item .\test\WriteToFile\*.exe .\test\WriteToFile\bin;

# Move the Linker and Assembler generated files to their respective folders
Move-Item .\assembler-log.txt .\test\logs;
Move-Item .\linker-log.txt .\test\logs;