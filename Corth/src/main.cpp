#include "Corth.h"

int main(int argc, char** argv)
{
    // PLATFORM SPECIFIC DEFAULTS
    #ifdef _WIN32
    // Defaults assume tools are the same directory as the "copmiler" on the same drive.
    Corth::ASMB_PATH = "\\NASM\\nasm.exe";
    Corth::ASMB_OPTS = "-f win64";
    Corth::LINK_PATH = "\\Golink\\golink.exe";
    Corth::LINK_OPTS = "/console /ENTRY:main msvcrt.dll";
    #endif

    #ifdef __linux__
    Corth::ASMB_PATH = "nasm";
    Corth::ASMB_OPTS = "-f elf64";
    Corth::LINK_PATH = "ld";
    Corth::LINK_OPTS = "-dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -m elf_x86_64";
    #endif

    // Non-graceful handling of command line arguments, abort execution.
    // Probably should refactor the function to work gracefully.
    if (!Corth::handleCommandLineArgs(argc, argv))
    {
        return -1;
    }

    Corth::Program prog;
    static_assert(static_cast<int>(Corth::MODE::COUNT) == 2, "Exhaustive handling of modes in main method");
    static_assert(static_cast<int>(Corth::PLATFORM::COUNT) == 1, "Exhaustive handling of platforms in main method");
    static_assert(static_cast<int>(Corth::ASM_SYNTAX::COUNT) == 2, "Exhaustive handling of asm syntax in main method");

    // Try to load program source from a file
    try
    {
        prog.source = loadFromFile(Corth::SOURCE_FILE);
        if(Corth::verboseLogging)
        {
            Corth::Log("Load File: Successful");
        }
    }
    catch(std::runtime_error e)
    {
        Corth::Error("Could not load file: " + std::string(e.what()));
        return -1;
    }

    catch (...)
    {
        Corth::Error("Could not load file: " + Corth::SOURCE_PATH + "\nUnknown Error: " + std::string(e.what()));
        return -1;
    }


    // Lex program source into tokens
    bool lexSuccessful = Corth::lex(prog);
    if(lexSuccessful)
    {
        Corth::validateTokens(prog);
    }
}
