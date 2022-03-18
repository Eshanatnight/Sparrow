#include "Sparrow.h"
#include "Logging.h"

Sparrow::MODE RUN_MODE = Sparrow::MODE::COMPILE;
Sparrow::PLATFORM RUN_PLATFORM = Sparrow::PLATFORM::WIN;
Sparrow::ASM_SYNTAX ASSEMBLY_SYNTAX = Sparrow::ASM_SYNTAX::NASM;
bool verboseLogging;
std::string SOURCE_PATH = "";
std::string OUTPUT_NAME = "";
std::string ASMB_PATH = "";
std::string LINK_PATH = "";
std::string ASMB_OPTS = "";
std::string LINK_OPTS = "";
std::string ASMB_NAME = "";
std::string LINK_NAME = "";


int main(int argc, char** argv)
{
    // PLATFORM SPECIFIC DEFAULTS
    // Defaults assume tools are the same directory as the "copmiler" on the same drive.
    ASMB_PATH = "Assembler\\nasm-2.15.05\\nasm.exe";
    ASMB_OPTS = "-f win64";
    LINK_PATH = "Linker\\GoLink\\GoLink.exe";
    LINK_OPTS = "/console /ENTRY:main msvcrt.dll";


    // Non-graceful handling of command line arguments, abort execution.
    // Probably should refactor the function to work gracefully.
    if (!Sparrow::handleCommandLineArgs(argc, argv))
    {
        return -1;
    }

    Sparrow::Program prog;
    Sparrow::staticCheck();

    // ! Some how the Source Path is getting courrupted. So had to create this function to fix it.
    SOURCE_PATH = Sparrow::getSourceFile(argc, argv);

    // Try to load program source from a file
    try
    {
        printf("\nSOURCE_PATH: %s\n", SOURCE_PATH.c_str());
        prog.source = Sparrow::loadFromFile(SOURCE_PATH);
        if(verboseLogging)
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
        Sparrow::Error("Could not load file: " + SOURCE_PATH + "\nUnknown Error");
        return -1;
    }


    // Lex program source into tokens
    bool lexSuccessful = Sparrow::lex(prog);
    if(lexSuccessful)
    {
        Sparrow::validateTokens(prog);
        if(verboseLogging)
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

    if(RUN_MODE == Sparrow::MODE::GENERATE)
    {
        if(RUN_PLATFORM == Sparrow::PLATFORM::WIN)
        {
            if(ASSEMBLY_SYNTAX == Sparrow::ASM_SYNTAX::NASM)
            {
                Sparrow::generateAssembly_NASM_win64(prog);
            }

            else if(ASSEMBLY_SYNTAX == Sparrow::ASM_SYNTAX::GAS)
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

    else if(RUN_MODE == Sparrow::MODE::COMPILE)
    {
        if (RUN_PLATFORM == Sparrow::PLATFORM::WIN)
        {
            if (ASSEMBLY_SYNTAX == Sparrow::ASM_SYNTAX::GAS)
            {
                Sparrow::generateAssembly_GAS_win64(prog);
                if (Sparrow::fileExists(ASMB_PATH))
                {
                    /* Construct Commands
                    Assembly is generated at `Sparrow::OUTPUT_NAME.s` */

                    std::string cmd_asmb = ASMB_PATH + " "+ ASMB_OPTS + " "+ OUTPUT_NAME + ".s " + ">assembler-log.txt 2>&1";

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

                    if (verboseLogging)
                    {
                        Sparrow::printCharactersFromFile__fopen_s("assembler-log.txt", "Assembler Log");
                    }
                }
                else
                {
                    Sparrow::Error("Assembler not found at " + ASMB_PATH + "\n");
                    return -1;
                }
            }
            else if (ASSEMBLY_SYNTAX == Sparrow::ASM_SYNTAX::NASM)
            {
                Sparrow::generateAssembly_NASM_win64(prog);
                if (Sparrow::fileExists(ASMB_PATH))
                {
                    if (Sparrow::fileExists(LINK_PATH))
                    {
                        /* Construct Commands
                        Assembly is generated at `Sparrow::OUTPUT_NAME.asm`
                        By default on win64, NASM generates an output `.obj` file of the same name as the input file.
                        This means the linker needs to link to `Sparrow::OUTPUT_NAME.obj`
                         */

                        std::string cmd_asmb = ASMB_PATH + " " + ASMB_OPTS + " " + OUTPUT_NAME + ".asm " + ">assembler-log.txt 2>&1";

                        std::string cmd_link = LINK_PATH + " " + LINK_OPTS + " " + OUTPUT_NAME + ".obj " + ">linker-log.txt 2>&1";

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

                        if (verboseLogging)
                        {
                            Sparrow::printCharactersFromFile__fopen_s("assembler-log.txt", "Assembler Log");
                            Sparrow::printCharactersFromFile__fopen_s("linker-log.txt", "Linker Log");
                        }
                    }

                    else
                    {
                        Sparrow::Error("Linker not found at " + LINK_PATH + "\n");
                        return -1;
                    }
                }

                else
                {
                    Sparrow::Error("Assembler not found at " + ASMB_PATH + "\n");
                    return -1;
                }
            }
        }

    }

    return 0;
}


