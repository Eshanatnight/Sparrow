# Create a test folder
mkdir test;

# Build Sparrow
..\.scripts\build-sparrow-with-cmake.ps1;

# Creates the Folders for the Tests, so it's neater and more organized for me
..\.scripts\test_paths.ps1;

# Tests
..\.scripts\test_compilation.ps1;

# Move the files to their respective folders
..\.scripts\move.ps1;

# Test Executables
..\.scripts\test_executables.ps1;