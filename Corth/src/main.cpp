#include "Corth.h"

int main(int argc, char** argv)
{
    // PLATFORM SPECIFIC DEFAULTS
    // Defaults assume tools are the same directory as the "copmiler" on the same drive.
    Corth::ASMB_PATH = "\\NASM\\nasm.exe";
    Corth::ASMB_OPTS = "-f win64";
    Corth::LINK_PATH = "\\Golink\\golink.exe";
    Corth::LINK_OPTS = "/console /ENTRY:main msvcrt.dll";

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
        if(Corth::verboseLogging)
        {
            Corth::Log("Lexed File in to Tokens: Successful");
            Corth::PrintTokens(prog);
        }
    }
    else
    {
        Corth::Error("Failure when lexing file into tokens");
        return -1;
    }

    if(Corth::RUN_MODE == Corth::MODE::GENERATE)
    {
        if(Corth::RUN_PLATFORM == Corth::PLATFORM::WIN)
        {
            if(Corth::ASSEMBLY_SYNTAX == Corth::ASM_SYNTAX::NASM)
            {
                Corth::generateAssembly_NASM_win64(prog);
            }
            else if(Corth::ASSEMBLY_SYNTAX == Corth::ASM_SYNTAX::GAS)
            {
                Corth::generateAssembly_GAS_win64(prog);
            }
            else
            {
                Corth::Error("Invalid assembly syntax");
                return -1;
            }
        }
    }

    else if(Corth::RUN_MODE == Corth::MODE::COMPILE)
    {
        if (Corth::RUN_PLATFORM == Corth::PLATFORM::WIN64)
        {
            if (Corth::ASSEMBLY_SYNTAX == Corth::ASM_SYNTAX::GAS)
            {
                Corth::generateAssembly_GAS_win64(prog);
                if (FileExists(Corth::ASMB_PATH))
                {
                    /* Construct Commands
                    Assembly is generated at `Corth::OUTPUT_NAME.s` */

                    std::string cmd_asmb = Corth::ASMB_PATH + " "+ Corth::ASMB_OPTS + " "+ Corth::OUTPUT_NAME + ".s " + ">assembler-log.txt 2>&1";

                    printf("[CMD]: `%s`\n", cmd_asmb.c_str());
                    if (system(cmd_asmb.c_str()) == 0)
                    {
                        Corth::Log("Assembler successful!");
                    }

                    else
                    {
                        Corth::Log("Assembler returned non-zero exit code, indicating a failure. "
                                   "Check `assembler-log.txt` for the output of the assembler, or enable verbose logging "
                                   "(`-v` flag) to print output to the console.");
                    }

                    if (Corth::verboseLogging)
                    {
                        printCharactersFromFile("assembler-log.txt", "Assembler Log");
                    }
                }
                else
                {
                    Corth::Error("Assembler not found at " + Corth::ASMB_PATH + "\n");
                    return -1;
                }
            }
            else if (Corth::ASSEMBLY_SYNTAX == Corth::ASM_SYNTAX::NASM)
            {
                Corth::generateAssembly_NASM_win64(prog);
                if (FileExists(Corth::ASMB_PATH))
                {
                    if (FileExists(Corth::LINK_PATH))
                    {
                        /* Construct Commands
                        Assembly is generated at `Corth::OUTPUT_NAME.asm`
                        By default on win64, NASM generates an output `.obj` file of the same name as the input file.
                        This means the linker needs to link to `Corth::OUTPUT_NAME.obj` */

                        std::string cmd_asmb = Corth::ASMB_PATH + " " + Corth::ASMB_OPTS + " " + Corth::OUTPUT_NAME + ".asm " + ">assembler-log.txt 2>&1";

                        std::string cmd_link = Corth::LINK_PATH + " " + Corth::LINK_OPTS + " " + Corth::OUTPUT_NAME + ".obj " + ">linker-log.txt 2>&1";

                        printf("[CMD]: `%s`\n", cmd_asmb.c_str());
                        if (system(cmd_asmb.c_str()) == 0)
                        {
                            Corth::Log("Assembler successful!");
                        }
                        else
                        {
                            Corth::Log("Assembler returned non-zero exit code, indicating a failure. "
                                       "Check `assembler-log.txt` for the output of the assembler, "
                                       "or enable verbose logging (`-v` flag) to print output to the console.");
                        }

                        printf("[CMD]: `%s`\n", cmd_link.c_str());
                        if (system(cmd_link.c_str()) == 0)
                        {
                            Corth::Log("Linker successful!");
                        }

                        else
                        {
                            Corth::Log("Linker returned non-zero exit code, indicating a failure. "
                                       "Check `linker-log.txt` for the output of the linker, "
                                       "or enable verbose logging (`-v` flag) to print output to the console.");
                        }

                        if (Corth::verboseLogging)
                        {
                            printCharactersFromFile("assembler-log.txt", "Assembler Log");
                            printCharactersFromFile("linker-log.txt", "Linker Log");
                        }
                    }

                    else
                    {
                        Corth::Error("Linker not found at " + Corth::LINK_PATH + "\n");
                        return -1;
                    }
                }

                else
                {
                    Corth::Error("Assembler not found at " + Corth::ASMB_PATH + "\n");
                    return -1;
                }
            }
        }

    }

    return 0;
}


