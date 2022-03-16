#include "Sparrow.h"
#include "Logging.h"

int main(int argc, char** argv)
{
    // PLATFORM SPECIFIC DEFAULTS
    // Defaults assume tools are the same directory as the "copmiler" on the same drive.
    Sparrow::ASMB_PATH = "\\NASM\\nasm.exe";
    Sparrow::ASMB_OPTS = "-f win64";
    Sparrow::LINK_PATH = "\\Golink\\golink.exe";
    Sparrow::LINK_OPTS = "/console /ENTRY:main msvcrt.dll";

    // Non-graceful handling of command line arguments, abort execution.
    // Probably should refactor the function to work gracefully.
    if (!Sparrow::handleCommandLineArgs(argc, argv))
    {
        return -1;
    }

    Sparrow::Program prog;
    Sparrow::staticCheck();

    // Try to load program source from a file
    try
    {
        prog.source = Sparrow::loadFromFile(Sparrow::SOURCE_PATH);
        if(Sparrow::verboseLogging)
        {
            Sparrow::Log("Load File: Successful");
        }
    }
    catch(std::runtime_error e)
    {
        Sparrow::Error("Could not load file: " + std::string(e.what()));
        return -1;
    }

    catch (...)
    {
        Sparrow::Error("Could not load file: " + Sparrow::SOURCE_PATH + "\nUnknown Error");
        return -1;
    }


    // Lex program source into tokens
    bool lexSuccessful = Sparrow::lex(prog);
    if(lexSuccessful)
    {
        Sparrow::validateTokens(prog);
        if(Sparrow::verboseLogging)
        {
            Sparrow::Log("Lexed File in to Tokens: Successful");
            Sparrow::PrintTokens(prog);
        }
    }
    else
    {
        Sparrow::Error("Failure when lexing file into tokens");
        return -1;
    }

    if(Sparrow::RUN_MODE == Sparrow::MODE::GENERATE)
    {
        if(Sparrow::RUN_PLATFORM == Sparrow::PLATFORM::WIN)
        {
            if(Sparrow::ASSEMBLY_SYNTAX == Sparrow::ASM_SYNTAX::NASM)
            {
                Sparrow::generateAssembly_NASM_win64(prog);
            }
            else if(Sparrow::ASSEMBLY_SYNTAX == Sparrow::ASM_SYNTAX::GAS)
            {
                Sparrow::generateAssembly_GAS_win64(prog);
            }
            else
            {
                Sparrow::Error("Invalid assembly syntax");
                return -1;
            }
        }
    }

    else if(Sparrow::RUN_MODE == Sparrow::MODE::COMPILE)
    {
        if (Sparrow::RUN_PLATFORM == Sparrow::PLATFORM::WIN)
        {
            if (Sparrow::ASSEMBLY_SYNTAX == Sparrow::ASM_SYNTAX::GAS)
            {
                Sparrow::generateAssembly_GAS_win64(prog);
                if (Sparrow::fileExists(Sparrow::ASMB_PATH))
                {
                    /* Construct Commands
                    Assembly is generated at `Sparrow::OUTPUT_NAME.s` */

                    std::string cmd_asmb = Sparrow::ASMB_PATH + " "+ Sparrow::ASMB_OPTS + " "+ Sparrow::OUTPUT_NAME + ".s " + ">assembler-log.txt 2>&1";

                    printf("[CMD]: `%s`\n", cmd_asmb.c_str());
                    if (system(cmd_asmb.c_str()) == 0)
                    {
                        Sparrow::Log("Assembler successful!");
                    }

                    else
                    {
                        Sparrow::Log("Assembler returned non-zero exit code, indicating a failure. "
                                   "Check `assembler-log.txt` for the output of the assembler, or enable verbose logging "
                                   "(`-v` flag) to print output to the console.");
                    }

                    if (Sparrow::verboseLogging)
                    {
                        Sparrow::printCharactersFromFile("assembler-log.txt", "Assembler Log");
                    }
                }
                else
                {
                    Sparrow::Error("Assembler not found at " + Sparrow::ASMB_PATH + "\n");
                    return -1;
                }
            }
            else if (Sparrow::ASSEMBLY_SYNTAX == Sparrow::ASM_SYNTAX::NASM)
            {
                Sparrow::generateAssembly_NASM_win64(prog);
                if (Sparrow::fileExists(Sparrow::ASMB_PATH))
                {
                    if (Sparrow::fileExists(Sparrow::LINK_PATH))
                    {
                        /* Construct Commands
                        Assembly is generated at `Sparrow::OUTPUT_NAME.asm`
                        By default on win64, NASM generates an output `.obj` file of the same name as the input file.
                        This means the linker needs to link to `Sparrow::OUTPUT_NAME.obj` */

                        std::string cmd_asmb = Sparrow::ASMB_PATH + " " + Sparrow::ASMB_OPTS + " " + Sparrow::OUTPUT_NAME + ".asm " + ">assembler-log.txt 2>&1";

                        std::string cmd_link = Sparrow::LINK_PATH + " " + Sparrow::LINK_OPTS + " " + Sparrow::OUTPUT_NAME + ".obj " + ">linker-log.txt 2>&1";

                        printf("[CMD]: `%s`\n", cmd_asmb.c_str());
                        if (system(cmd_asmb.c_str()) == 0)
                        {
                            Sparrow::Log("Assembler successful!");
                        }
                        else
                        {
                            Sparrow::Log("Assembler returned non-zero exit code, indicating a failure. "
                                       "Check `assembler-log.txt` for the output of the assembler, "
                                       "or enable verbose logging (`-v` flag) to print output to the console.");
                        }

                        printf("[CMD]: `%s`\n", cmd_link.c_str());
                        if (system(cmd_link.c_str()) == 0)
                        {
                            Sparrow::Log("Linker successful!");
                        }

                        else
                        {
                            Sparrow::Log("Linker returned non-zero exit code, indicating a failure. "
                                       "Check `linker-log.txt` for the output of the linker, "
                                       "or enable verbose logging (`-v` flag) to print output to the console.");
                        }

                        if (Sparrow::verboseLogging)
                        {
                            Sparrow::printCharactersFromFile("assembler-log.txt", "Assembler Log");
                            Sparrow::printCharactersFromFile("linker-log.txt", "Linker Log");
                        }
                    }

                    else
                    {
                        Sparrow::Error("Linker not found at " + Sparrow::LINK_PATH + "\n");
                        return -1;
                    }
                }

                else
                {
                    Sparrow::Error("Assembler not found at " + Sparrow::ASMB_PATH + "\n");
                    return -1;
                }
            }
        }

    }

    return 0;
}


