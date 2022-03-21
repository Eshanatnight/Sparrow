# Test and create the intermediates folder
if(!(Test-Path .\test\HelloWorld\Intermediates))
{
    mkdir .\test\HelloWorld\Intermediates;
}

if(!(Test-Path .\test\Fizzbuzz\Intermediates))
{
    mkdir .\test\Fizzbuzz\Intermediates;
}

if(!(Test-Path .\test\Rule110\Intermediates))
{
    mkdir .\test\Rule110\Intermediates;
}

if(!(Test-Path .\test\WriteToFile\Intermediates))
{
    mkdir .\test\WriteToFile\Intermediates;
}


# Test and Create the bin folders
if(!(Test-Path .\test\HelloWorld\bin))
{
    mkdir .\test\HelloWorld\bin;
}

if (!(Test-Path .\test\Fizzbuzz\bin))
{
    mkdir .\test\Fizzbuzz\bin;
}

if(!(Test-Path .\test\Rule110\bin))
{
    mkdir .\test\Rule110\bin;
}

if(!(Test-Path .\test\WriteToFile\bin))
{
    mkdir .\test\WriteToFile\bin;
}


# Test and create the log folders
if(!(Test-Path .\test\logs))
{
    mkdir .\test\logs;
}