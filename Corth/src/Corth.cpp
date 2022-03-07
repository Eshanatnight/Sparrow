#include "Corth.h"
#include <fstream>

bool Corth::isOperator(char& c)
{
    		return c == '+'    // addition
			|| c == '-'    // subtraction
			|| c == '*'    // multiplication
			|| c == '/'    // division
			|| c == '='    // equality comparison
			|| c == '<'    // less than (or equal) comparison
			|| c == '>'    // greater than (or equal) comparison
			|| c == '#'    // dump (pop + print)
			|| c == '%'    // modulo
			|| c == '&'    // bitwise and (when combined with another)
			|| c == '|';   // bitwise or (when combined with another)
}

bool Corth::isKeyword(std::string& word)
{
    static_assert(static_cast<int>(Keyword::COUNT) == 28, "Exhaustive handling of keywords in iskeyword");

        if (word == "if"
        || word == "else"
        || word == "endif"
        || word == "do"
        || word == "while"
        || word == "endwhile"
		|| word == "dup"
        || word == "twodup"
        || word == "drop"
        || word == "swap"
        || word == "over"
		|| word == "dump"
        || word == "dump_c"
        || word == "dump_s"
        || word == "mem"
        || word == "loadb"
        || word == "storeb"
		|| word == "loadw"
        || word == "storew"
		|| word == "loadd"
        || word == "stored"
		|| word == "loadq"
        || word == "storeq"
        || word == "shl"
        || word == "shr"
        || word == "or"
        || word == "and"
		|| word == "mod")
        {
            return true;
        }
        return false;
}

std::string Corth::getKeywordStr(Keyword word)
{
    static_assert(static_cast<int>(Keyword::COUNT) == 28, "Exhaustive handling of keywords in getKeywordStr Function");

    switch(word)
    {
        case Keyword::IF:       { return "if";       }
        case Keyword::ELSE:     { return "else";     }
        case Keyword::ENDIF:    { return "endif";    }
        case Keyword::DO:       { return "do";       }
        case Keyword::WHILE:    { return "while";    }
        case Keyword::ENDWHILE: { return "endwhile"; }

        case Keyword::DUP:      { return "dup";      }
        case Keyword::TWODUP:   { return "twodup";   }
        case Keyword::DROP:     { return "drop";     }
        case Keyword::SWAP:     { return "swap";     }
        case Keyword::OVER:     { return "over";     }
        case Keyword::DUMP:     { return "dump";     }
        case Keyword::DUMP_C:   { return "dump_c";   }
        case Keyword::DUMP_S:   { return "dump_s";   }

        case Keyword::MEM:      { return "mem";      }
        case Keyword::LOADB:    { return "loadb";    }
        case Keyword::STOREB:   { return "storeb";   }
        case Keyword::LOADW:    { return "loadw";    }
        case Keyword::STOREW:   { return "storew";   }
        case Keyword::LOADD:    { return "loadd";    }
        case Keyword::STORED:   { return "stored";   }
        case Keyword::LOADQ:    { return "loadq";    }
        case Keyword::STOREQ:   { return "storeq";   }

        case Keyword::SHL:      { return "shl";      }
        case Keyword::SHR:      { return "shr";      }
        case Keyword::OR:       { return "or";       }
        case Keyword::AND:      { return "and";      }
        case Keyword::MOD:      { return "mod";      }
        default:
        {
            Error("Unreachable in getKeywordStr Function");
            exit(1);
            return "Error";
        }
    }
}


std::string Corth::TokenTypeStr(TokenType& t)
{
    static_assert(static_cast<int>(TokenType::COUNT) == 5, "Exhaustive handling of TokenType in TokenTypeStr Function");

    if(t == TokenType::WHITESPACE)
    {
        return "WHITESPACE";
    }

    else if (t == TokenType::INT)
    {
        return "INTEGER";
    }

    else if (t == TokenType::STRING)
    {
        return "STRING";
    }

    else if (t == TokenType::OP)
    {
        return "OPERATOR";
    }

    else if (t == TokenType::KEYWORD)
    {
        return "KEYWORD";
    }

    Error("Unreachable in TokenTypeStr Function");
    exit(1);
    return "Error";
}

Corth::Token::Token()
: type(TokenType::WHITESPACE), text(""), line_number(1), col_number(1)
{}


void Corth::printUseage()
{
printf("\n%s\n", "Usage: `Corth.exe <flags> <options> Path/To/File.corth`");
        printf("    %s\n", "Flags:");
        printf("        %s\n", "-win, -win64             | (default) Generate assembly for Windows 64-bit. If no platform is specified, this is the default.");
        printf("        %s\n", "-linux, -linux64         | Generate assembly for Linux 64-bit.");
        printf("        %s\n", "-com, --compile          | (default) Compile program from source into executable");
        printf("        %s\n", "-gen, --generate         | Generate assembly, but don't create an executable from it.");
        printf("        %s\n", "-NASM                    | (default) When generating assembly, use NASM syntax. Any OPTIONS set before NASM may or may be over-ridden; best practice is to put it first.");
        printf("        %s\n", "-GAS                     | When generating assembly, use GAS syntax. This is able to be assembled by gcc into an executable. (pass output file name to gcc with `-add-ao \"-o <output-file-name>\" and not the built-in `-o` option`). Any OPTIONS set before GAS may or may be over-ridden; best practice is to put it first.");
        printf("        %s\n", "-v, --verbose            | Enable verbose logging within Corth");
        printf("    %s\n", "Options (latest over-rides):");
        printf("        %s\n", "Usage: <option> <input>");
        printf("        %s\n", "If the <input> contains spaces, be sure to surround it by double quotes");
        printf("        %s\n", "-o, --output-name        | Specify name of generated files. On Linux, affects only generated assembly file; use -add-ao/-add-lo to specify output object and executable file name manually");
        printf("        %s\n", "-a, --assembler-path     | Specify path to assembler (include extension)");
        printf("        %s\n", "-l, --linker-path        | Specify path to linker (include extension)");
        printf("        %s\n", "-ao, --assembler-options | Command line arguments called with assembler");
        printf("        %s\n", "-lo, --linker-options    | Command line arguments called with linker");
        printf("        %s\n", "-add-ao, --add-asm-opt   | Append a command line argument to assembler options");
        printf("        %s\n", "-add-lo, --add-link-opt  | Append a command line argument to linker options");
}

std::vector<std::string> Corth::stringToHex(const std::string& input)
{
    static const char hexDegits[] = "0123456789abcdef";

    std::vector<std::string> output;
    for(unsigned char c : input)
    {
        std::string hex;
        hex += hexDegits[(c & 0xF0) >> 4];
        hex += hexDegits[c & 0x0F];
        output.push_back(hex);
    }

    /*
    // More readable way
    for (unsigned char c : input)
    {
        std::string hex;
		hex.append("0x");
        hex.push_back(hex_digits[c >> 4]);
        hex.push_back(hex_digits[c & 15]);
        output.push_back(hex);
    }
    */

    // Problems with escape characters
    // \n, \t \r would get converted in to 2 hex characters
    // we need to detect and fix that

    std::vector<std::string> new_output;
    for (size_t i = 0; i < output.size(); ++i)
    {
        if(output[i] == "0x5c")
        {
            // backslash found
            ++i;
            if(i < output.size())
            {
                if(output[i] == "0x6e")
                {
                    // Found \n character
                    new_output.push_back("0xa");
                    continue;
                }

                else if (output[i] == "0x72")
                {
                    // Found \r character
                    new_output.push_back("0xd");
                    continue;
                }

                else if(output[i] == "0x74")
                {
                    // Found \t character
                    new_output.push_back("0x9");
                    continue;
                }
            }
            // Special Character pattern not found, undo lookahead
            --i;
        }
        new_output.push_back(output[i]);
    }
    return new_output;
}

void Corth::generateAssembly_NASM_win64(Program& prog)
{
        // Loop through a lexed program and then generate assembly file from it.
        std::string asm_file_path = OUTPUT_NAME + ".asm";
        std::fstream asm_file;
        asm_file.open(asm_file_path.c_str(), std::ios::out);

        if (asm_file)
        {
            Log("Generating NASM win64 assembly");

            // String constants
            std::vector<std::string> string_literals;

            // WRITE HEADER TO ASM FILE
            asm_file << "    ;; CORTH COMPILER GENERATED THIS ASSEMBLY\n"
                    << "    ;; USING `WINDOWS x64` CALLING CONVENTION (RCX, RDX, R8, R9, ETC)\n"
                    << "    SECTION .text\n"
                    << "    ;; DEFINE EXTERNAL C RUNTIME SYMBOLS (LINK AGAINST MSVCRT.DLL)\n"
                    << "    extern printf\n"
                    << "    extern exit\n"
                    << "\n"
                    << "    global main\n"
                    << "main:\n";

            // WRITE TOKENS TO ASM FILE MAIN LABEL
            static_assert(static_cast<int>(TokenType::COUNT) == 5, "Exhaustive handling of token types in GenerateAssembly_NASM_win64");
            size_t instr_ptr = 0;
            size_t instr_ptr_max = prog.tokens.size();
            while (instr_ptr < instr_ptr_max)
            {
                Token& tok = prog.tokens[instr_ptr];
                if (tok.type == TokenType::INT)
                {
                    asm_file << "    ;; -- push INT --\n"
                            << "    mov rax, " << tok.text << "\n"
                            << "    push rax\n";
                }
                else if (tok.type == TokenType::STRING)
                {
                    asm_file << "    ;; -- push STRING --\n"
                            << "    mov rax, str_" << string_literals.size() << '\n'
                            << "    push rax\n";
                    string_literals.push_back(tok.text);
                }
                else if (tok.type == TokenType::OP)
                {
                    static_assert(OP_COUNT == 15, "Exhaustive handling of operators in GenerateAssembly_NASM_win64");
                    if (tok.text == "+")
                    {
                        asm_file << "    ;; -- add --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    add rax, rbx\n"
                                << "    push rax\n";
                    }
                    else if (tok.text == "-")
                    {
                        asm_file << "    ;; -- subtract --\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    sub rax, rbx\n"
                                << "    push rax\n";
                    }
                    else if (tok.text == "*")
                    {
                        asm_file << "    ;; -- multiply --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    mul rbx\n"
                                << "    push rax\n";
                    }
                    else if (tok.text == "/")
                    {
                        asm_file << "    ;; -- divide --\n"
                                << "    xor rdx, rdx\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    div rbx\n"
                                << "    push rax\n";
                    }
					else if (tok.text == "%")
                    {
                        asm_file << "    ;; -- modulo --\n"
                                << "    xor rdx, rdx\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    div rbx\n"
                                << "    push rdx\n";
                    }
                    else if (tok.text == "=")
                    {
                        asm_file << "    ;; -- equality condition --\n"
                                << "    mov rcx, 0\n"
                                << "    mov rdx, 1\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    cmp rax, rbx\n"
                                << "    cmove rcx, rdx\n"
                                << "    push rcx\n";
                    }
                    else if (tok.text == "<")
                    {
                        asm_file << "    ;; -- less than condition --\n"
                                << "    mov rcx, 0\n"
                                << "    mov rdx, 1\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    cmp rax, rbx\n"
                                << "    cmovl rcx, rdx\n"
                                << "    push rcx\n";
                    }
                    else if (tok.text == ">")
                    {
                        asm_file << "    ;; -- greater than condition --\n"
                                << "    mov rcx, 0\n"
                                << "    mov rdx, 1\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    cmp rax, rbx\n"
                                << "    cmovg rcx, rdx\n"
                                << "    push rcx\n";
                    }
                    else if (tok.text == "<=")
                    {
                        asm_file << "    ;; -- less than or equal condition --\n"
                                << "    mov rcx, 0\n"
                                << "    mov rdx, 1\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    cmp rax, rbx\n"
                                << "    cmovle rcx, rdx\n"
                                << "    push rcx\n";
                    }
                    else if (tok.text == ">=")
                    {
                        asm_file << "    ;; -- greater than or equal condition --\n"
                                << "    mov rcx, 0\n"
                                << "    mov rdx, 1\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    cmp rax, rbx\n"
                                << "    cmovge rcx, rdx\n"
                                << "    push rcx\n";
                    }
                    else if (tok.text == "<<")
                    {
                        asm_file << "    ;; -- bitwise-shift left --\n"
                                << "    pop rcx\n"
                                << "    pop rbx\n"
                                << "    shl rbx, cl\n"
                                << "    push rbx";
                    }
                    else if (tok.text == ">>")
                    {
                        asm_file << "    ;; -- bitwise-shift right --\n"
                                << "    pop rcx\n"
                                << "    pop rbx\n"
                                << "    shr rbx, cl\n"
                                << "    push rbx";
                    }
                    else if (tok.text == "||")
                    {
                        asm_file << "    ;; -- bitwise or --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    or rax, rbx\n"
                                << "    push rax\n";
                    }
                    else if (tok.text == "&&")
                    {
                        asm_file << "    ;; -- bitwise and --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    and rax, rbx\n"
                                << "    push rax\n";
                    }
                    else if (tok.text == "#")
                    {
                        // A call in Windows x64 requires shadow space(spill space or home space) on the stack
                        // It's basically space on the stack the called function will assume to be usable and over-writable
                        // In Corth, a stack-based language, this obviously causes issues if I don't handle it correctly.
                        asm_file << "    ;; -- dump --\n"
                                << "    lea rcx, [rel fmt]\n"
                                << "    pop rdx\n"
                                << "    xor rax, rax\n"
                                << "    sub rsp, 32\n"
                                << "    call printf\n"
                                << "    add rsp, 32\n";
                    }
                }
                else if (tok.type == TokenType::KEYWORD)
                {
                    static_assert(static_cast<int>(Keyword::COUNT) == 28, "Exhaustive handling of keywords in GenerateAssembly_NASM_win64");
                    if (tok.text == getKeywordStr(Keyword::IF))
                    {
                        asm_file << "    ;; -- if --\n"
                                << "    pop rax\n"
                                << "    cmp rax, 0\n"
                                << "    je addr_" << tok.data << "\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::ELSE))
                    {
                        asm_file << "    ;; -- else --\n"
                                << "    jmp addr_" << tok.data << "\n"
                                << "addr_" << instr_ptr << ":\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::ENDIF))
                    {
                        asm_file << "    ;; -- endif --\n"
                                << "addr_" << instr_ptr << ":\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::WHILE))
                    {
                        asm_file << "    ;; -- while --\n"
                                << "addr_" << instr_ptr << ":\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::DO))
                    {
                        asm_file << "    ;; -- do --\n"
                                << "    pop rax\n"
                                << "    cmp rax, 0\n"
                                << "    je addr_" << tok.data << "\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::ENDWHILE))
                    {
                        asm_file << "    ;; -- endwhile --\n"
                                << "    jmp addr_" << tok.data << "\n"
                                << "addr_" << instr_ptr << ":\n";
                    }

					else if (tok.text == getKeywordStr(Keyword::DUP))
                    {
                        asm_file << "    ;; -- dup --\n"
                                << "    pop rax\n"
                                << "    push rax\n"
                                << "    push rax\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::TWODUP))
                    {
                        asm_file << "    ;; -- twodup --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    push rbx\n"
                                << "    push rax\n"
                                << "    push rbx\n"
                                << "    push rax\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::DROP))
                    {
                        asm_file << "    ;; -- drop --\n"
                                << "    pop rax\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::SWAP))
                    {
                        asm_file << "    ;; -- swap --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    push rax\n"
                                << "    push rbx\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::OVER))
                    {
                        asm_file << "    ;; -- over --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    push rbx\n"
                                << "    push rax\n"
                                << "    push rbx\n";
                    }
					else if (tok.text == getKeywordStr(Keyword::DUMP))
                    {
                        asm_file << "    ;; -- dump --\n"
                                << "    lea rcx, [rel fmt]\n"
                                << "    pop rdx\n"
                                << "    xor rax, rax\n"
                                << "    sub rsp, 32\n"
                                << "    call printf\n"
                                << "    add rsp, 32\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::DUMP_C))
                    {
                        asm_file << "    ;; -- dump --\n"
                                << "    lea rcx, [rel fmt_char]\n"
                                << "    pop rdx\n"
                                << "    xor rax, rax\n"
                                << "    sub rsp, 32\n"
                                << "    call printf\n"
                                << "    add rsp, 32\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::DUMP_S))
                    {
                        asm_file << "    ;; -- dump --\n"
                                << "    lea rcx, [rel fmt_str]\n"
                                << "    pop rdx\n"
                                << "    xor rax, rax\n"
                                << "    sub rsp, 32\n"
                                << "    call printf\n"
                                << "    add rsp, 32\n";
                    }

					else if (tok.text == getKeywordStr(Keyword::MEM))
                    {
                        // Pushes the relative address of allocated memory onto the stack
                        asm_file << "    ;; -- mem --\n"
                                << "    push mem\n";

                    }
                    else if (tok.text == getKeywordStr(Keyword::LOADB))
                    {
                        asm_file << "    ;; -- load byte --\n"
                                << "    pop rax\n"
                                << "    xor rbx, rbx\n"
                                << "    mov bl, [rax]\n"
                                << "    push rbx\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::STOREB))
                    {
                        asm_file << "    ;; -- store byte --\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    mov [rax], bl\n";
                    }
					else if (tok.text == getKeywordStr(Keyword::LOADW))
                    {
                        asm_file << "    ;; -- load word --\n"
                                << "    pop rax\n"
                                << "    xor rbx, rbx\n"
                                << "    mov bx, [rax]\n"
                                << "    push rbx\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::STOREW))
                    {
                        asm_file << "    ;; -- store word --\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    mov [rax], bx\n";
                    }
					else if (tok.text == getKeywordStr(Keyword::LOADD))

                    {
                        asm_file << "    ;; -- load double word --\n"
                                << "    pop rax\n"
                                << "    xor rbx, rbx\n"
                                << "    mov ebx, [rax]\n"
                                << "    push rbx\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::STORED))
                    {
                        asm_file << "    ;; -- store double word --\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    mov [rax], ebx\n";
                    }
					else if (tok.text == getKeywordStr(Keyword::LOADQ))
                    {
                        asm_file << "    ;; -- load quad word --\n"
                                << "    pop rax\n"
                                << "    xor rbx, rbx\n"
                                << "    mov rbx, [rax]\n"
                                << "    push rbx\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::STOREQ))
                    {
                        asm_file << "    ;; -- store quad word --\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    mov [rax], rbx\n";
                    }

                    else if (tok.text == getKeywordStr(Keyword::SHL))
                    {
                        asm_file << "    ;; -- bitwise-shift left --\n"
                                << "    pop rcx\n"
                                << "    pop rbx\n"
                                << "    shl rbx, cl\n"
                                << "    push rbx";
                    }
                    else if (tok.text == getKeywordStr(Keyword::SHR))
                    {
                        asm_file << "    ;; -- bitwise-shift right --\n"
                                << "    pop rcx\n"
                                << "    pop rbx\n"
                                << "    shr rbx, cl\n"
                                << "    push rbx";
                    }
                    else if (tok.text == getKeywordStr(Keyword::OR))
                    {
                        asm_file << "    ;; -- bitwise or --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    or rax, rbx\n"
                                << "    push rax\n";
                    }
                    else if (tok.text == getKeywordStr(Keyword::AND))
                    {
                        asm_file << "    ;; -- bitwise and --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    and rax, rbx\n"
                                << "    push rax\n";
                    }
					else if (tok.text == getKeywordStr(Keyword::MOD))
                    {
                        asm_file << "    ;; -- modulo --\n"
                                << "    xor rdx, rdx\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    div rbx\n"
                                << "    push rdx\n";
                    }
                }
                instr_ptr++;
            }

            // EXIT GRACEFULLY
            asm_file << "    mov rcx, 0\n"
                    << "    call exit\n\n";

            // DECLARE CORTH CONSTANTS
            asm_file << "    SECTION .data\n"
                    << "    fmt db '%u', 0\n"
                    << "    fmt_char db '%c', 0\n"
                    << "    fmt_str db '%s', 0\n";

            // DECLARE USER-DEFINED STRING CONSTANTS HERE
            size_t index = 0;
            for (auto& string : string_literals)
            {
                std::vector<std::string> hex_chars = stringToHex(string);
                asm_file << "str_" << index << " db ";
                for (auto& c : hex_chars)
                {
                    asm_file << c << ',';
                }

                // Null-terminator
                asm_file << "0\n";
                index++;
            }

            // ALLOCATE MEMORY
            asm_file << "\n"
                    << "    SECTION .bss\n"
                    << "    mem resb " << MEM_CAPACITY << "\n";

            asm_file.close();
            Log("NASM win64 assembly generated at " + asm_file_path);
        }

        else
        {
            Error("Could not open file for writing. Does directory exist?");
            exit(1);
        }
}

// Generates GAS assembly file
// Note: I have not used GAS to test this but to my knowledge it should work
// If the person reading this is interested in using GAS, feel free to implement
// and submit a pull request.
void Corth::generateAssembly_GAS_win64(Program &prog)
{
    std::string asm_file_path = OUTPUT_NAME + ".s";
    std::fstream asm_file;
    // I know the constructor can be used to directly open a file for writing
    // this is just a thing i do to make it easier to read
    asm_file.open(asm_file_path.c_str(), std::ios::out);
    if (asm_file)
    {
        Log("Generating WIN64 GAS assembly");

        // Save list of defined strings in file to write at the end of the assembly in the `.data` section.
        std::vector<std::string> string_literals;

        // WRITE HEADER TO ASM FILE
        asm_file << "    # CORTH COMPILER GENERATED THIS ASSEMBLY\n"
                << "    # USING `GAS` SYNTAX\n"
                << "    # USING `WINDOWS X64` CALLING CONVENTION (RCX, RDX, R8, R9, -> STACK)\n"
                << "    .text\n"
                << "    .globl main\n"
                << "main:\n";

        // WRITE TOKENS TO ASM FILE MAIN LABEL
        static_assert(static_cast<int>(TokenType::COUNT) == 5, "Exhaustive handling of token types in GenerateAssembly_GAS_win64");
        size_t instr_ptr = 0;
        size_t instr_ptr_max = prog.tokens.size();

        while (instr_ptr < instr_ptr_max)
        {
            Token& tok = prog.tokens[instr_ptr];

            // Write assembly to opened file based on token type and value
            if (tok.type == TokenType::INT)
            {
                asm_file << "    # -- push INT --\n"
                        << "    mov $"  << tok.text << ", %rax" << "\n"
                        << "    push %rax\n";
            }
            else if (tok.type == TokenType::STRING)
            {
                asm_file << "    # -- push STRING --\n"
                        << "    lea str_" << string_literals.size() << "(%rip), %rax\n"
                        << "    push %rax\n";

                // String value is stored in tok.text
                string_literals.push_back(tok.text);
            }
            else if (tok.type == TokenType::OP)
            {
                static_assert(OP_COUNT == 15, "Exhaustive handling of operators in GenerateAssembly_GAS_win64");
                if (tok.text == "+")
                {
                    asm_file << "    # -- add --\n"
                            << "    pop %rax\n"
                            << "    pop %rbx\n"
                            << "    add %rbx, %rax\n"
                            << "    push %rax\n";
                }
                else if (tok.text == "-")
                {
                    asm_file << "    # -- subtract --\n"
                            << "    pop %rbx\n"
                            << "    pop %rax\n"
                            << "    sub %rbx, %rax\n"
                            << "    push %rax\n";
                }
                else if (tok.text == "*")
                {
                    asm_file << "    # -- multiply --\n"
                            << "    pop %rax\n"
                            << "    pop %rbx\n"
                            << "    mul %rbx\n"
                            << "    push %rax\n";
                }
                else if (tok.text == "/")
                {
                    asm_file << "    # -- divide --\n"
                            << "    xor %rdx, %rdx\n"
                            << "    pop %rbx\n"
                            << "    pop %rax\n"
                            << "    div %rbx\n"
                            << "    push %rax\n";
                }
                else if (tok.text == "%")
                {
                        asm_file << "    # -- modulo --\n"
                            << "    xor %rdx, %rdx\n"
                            << "    pop %rbx\n"
                            << "    pop %rax\n"
                            << "    div %rbx\n"
                            << "    push %rdx\n";
                }
                else if (tok.text == "=")
                {
                    asm_file << "    # -- equality condition --\n"
                            << "    mov $0, %rcx\n"
                            << "    mov $1, %rdx\n"
                            << "    pop %rax\n"
                            << "    pop %rbx\n"
                            << "    cmp %rbx, %rax\n"
                            << "    cmove %rdx, %rcx\n"
                            << "    push %rcx\n";
                }
                else if (tok.text == "<")
                {
                    asm_file << "    # -- less than condition --\n"
                            << "    mov $0, %rcx\n"
                            << "    mov $1, %rdx\n"
                            << "    pop %rbx\n"
                            << "    pop %rax\n"
                            << "    cmp %rbx, %rax\n"
                            << "    cmovl %rdx, %rcx\n"
                            << "    push %rcx\n";
                }
                else if (tok.text == ">")
                {
                    asm_file << "    # -- greater than condition --\n"
                        << "    mov $0, %rcx\n"
                        << "    mov $1, %rdx\n"
                        << "    pop %rbx\n"
                        << "    pop %rax\n"
                        << "    cmp %rbx, %rax\n"
                        << "    cmovg %rdx, %rcx\n"
                        << "    push %rcx\n";
                }
                else if (tok.text == "<=")
                {
                    asm_file << "    # -- less than or equal condition --\n"
                            << "    mov $0, %rcx\n"
                            << "    mov $1, %rdx\n"
                            << "    pop %rbx\n"
                            << "    pop %rax\n"
                            << "    cmp %rbx, %rax\n"
                            << "    cmovle %rdx, %rcx\n"
                            << "    push %rcx\n";
                }
                else if (tok.text == ">=")
                {
                    asm_file << "    # -- greater than or equal condition --\n"
                            << "    mov $0, %rcx\n"
                            << "    mov $1, %rdx\n"
                            << "    pop %rbx\n"
                            << "    pop %rax\n"
                            << "    cmp %rbx, %rax\n"
                            << "    cmovge %rdx, %rcx\n"
                            << "    push %rcx\n";
                }
                else if (tok.text == "<<")
                {
                    asm_file << "    # -- bitwise-shift left --\n"
                    << "    pop %rcx\n"
                    << "    pop %rbx\n"
                    << "    shl %cl, %rbx\n"
                    << "    push %rbx";
                }
                else if (tok.text == ">>")
                {
                    asm_file << "    # -- bitwise-shift right --\n"
                            << "    pop %rcx\n"
                            << "    pop %rbx\n"
                            << "    shr %cl, %rbx\n"
                            << "    push %rbx";
                }
                else if (tok.text == "||")
                {
                    asm_file << "    # -- bitwise or --\n"
                            << "    pop %rax\n"
                            << "    pop %rbx\n"
                            << "    or %rbx, %rax\n"
                            << "    push %rax\n";
                }
                else if (tok.text == "&&")
                {
                    asm_file << "    # -- bitwise and --\n"
                            << "    pop %rax\n"
                            << "    pop %rbx\n"
                            << "    and %rbx, %rax\n"
                            << "    push %rax\n";
                }
                else if (tok.text == "#")
                {
                    asm_file << "    # -- dump --\n"
                            << "    lea fmt(%rip), %rcx\n"
                            << "    pop %rdx\n"
                            << "    xor %rax, %rax\n"
                            << "    sub $32, %rsp\n"
                            << "    call printf\n"
                            << "    add $32, %rsp\n";
                }
            }
            else if (tok.type == TokenType::KEYWORD)
            {
                static_assert(static_cast<int>(Keyword::COUNT) == 28, "Exhaustive handling of token types in GenerateAssembly_GAS_win64");
                if (tok.text == getKeywordStr(Keyword::IF))
                {
                    asm_file << "    # -- if --\n"
                            << "    pop %rax\n"
                            << "    cmp $0, %rax\n"
                            << "    je addr_" << tok.data << "\n";
                }
                else if (tok.text == getKeywordStr(Keyword::ELSE))
                {
                    asm_file << "    # -- else --\n"
                            << "    jmp addr_" << tok.data << "\n"
                            << "addr_" << instr_ptr << ":\n";
                }
                else if (tok.text == getKeywordStr(Keyword::ENDIF))
                {
                    asm_file << "    # -- endif --\n"
                            << "addr_" << instr_ptr << ":\n";
                }
                else if (tok.text == getKeywordStr(Keyword::WHILE))
                {
                    asm_file << "    # -- while --\n"
                            << "addr_" << instr_ptr << ":\n";
                }
                else if (tok.text == getKeywordStr(Keyword::DO))
                {
                    asm_file << "    # -- do --\n"
                            << "    pop %rax\n"
                            << "    cmp $0, %rax\n"
                            << "    je addr_" << tok.data << "\n";
                }
                else if (tok.text == getKeywordStr(Keyword::ENDWHILE))
                {
                    asm_file << "    # -- endwhile --\n"
                            << "    jmp addr_" << tok.data << "\n"
                            << "addr_" << instr_ptr << ":\n";
                }

                else if (tok.text == getKeywordStr(Keyword::DUP))
                {
                    asm_file << "    # -- dup --\n"
                            << "    pop %rax\n"
                            << "    push %rax\n"
                            << "    push %rax\n";
                }
                else if (tok.text == getKeywordStr(Keyword::TWODUP))
                {
                    asm_file << "    # -- twodup --\n"
                            << "    pop %rax\n"
                            << "    pop %rbx\n"
                            << "    push %rbx\n"
                            << "    push %rax\n"
                            << "    push %rbx\n"
                            << "    push %rax\n";
                }
                else if (tok.text == getKeywordStr(Keyword::DROP))
                {
                    asm_file << "    # -- drop --\n"
                            << "    pop %rax\n";
                }
                else if (tok.text == getKeywordStr(Keyword::SWAP))
                {
                    asm_file << "    # -- swap --\n"
                            << "    pop %rax\n"
                            << "    pop %rbx\n"
                            << "    push %rax\n"
                            << "    push %rbx\n";
                }
                else if (tok.text == getKeywordStr(Keyword::OVER))
                {
                    asm_file << "    # -- over --\n"
                        << "    pop %rax\n"
                        << "    pop %rbx\n"
                        << "    push %rbx\n"
                        << "    push %rax\n"
                        << "    push %rbx\n";
                }
                else if (tok.text == getKeywordStr(Keyword::DUMP))
                {
                    asm_file << "    # -- dump --\n"
                            << "    lea fmt(%rip), %rcx\n"
                            << "    pop %rdx\n"
                            << "    xor %rax, %rax\n"
                            << "    sub $32, %rsp\n"
                            << "    call printf\n"
                            << "    add $32, %rsp\n";
                }
                else if (tok.text == getKeywordStr(Keyword::DUMP_C))
                {
                    asm_file << "    # -- dump character --\n"
                            << "    lea fmt_char(%rip), %rcx\n"
                            << "    pop %rdx\n"
                            << "    xor %rax, %rax\n"
                            << "    sub $32, %rsp\n"
                            << "    call printf\n"
                            << "    add $32, %rsp\n";
                }
                else if (tok.text == getKeywordStr(Keyword::DUMP_S))
                {
                    asm_file << "    # -- dump string --\n"
                            << "    lea fmt_str(%rip), %rcx\n"
                            << "    pop %rdx\n"
                            << "    xor %rax, %rax\n"
                            << "    sub $32, %rsp\n"
                            << "    call printf\n"
                            << "    add $32, %rsp\n";
                }

                else if (tok.text == getKeywordStr(Keyword::MEM))
                {
                    // Pushes the relative address of allocated memory onto the stack
                    asm_file << "    # -- mem --\n"
                            << "    lea mem(%rip), %rax\n"
                            << "    push %rax\n";
                }
                else if (tok.text == getKeywordStr(Keyword::LOADB))
                {
                    asm_file << "    # -- load byte --\n"
                            << "    pop %rax\n"
                            << "    xor %rbx, %rbx\n"
                            << "    mov (%rax), %bl\n"
                            << "    push %rbx\n";
                }
                else if (tok.text == getKeywordStr(Keyword::STOREB))
                {
                    asm_file << "    # -- store byte --\n"
                            << "    pop %rbx\n"
                            << "    pop %rax\n"
                            << "    mov %bl, (%rax)\n";
                }
                else if (tok.text == getKeywordStr(Keyword::LOADW))
                {
                    asm_file << "    # -- load word --\n"
                            << "    pop %rax\n"
                            << "    xor %rbx, %rbx\n"
                            << "    mov (%rax), %bx\n"
                            << "    push %rbx\n";
                }
                else if (tok.text == getKeywordStr(Keyword::STOREW))
                {
                    asm_file << "    # -- store word --\n"
                            << "    pop %rbx\n"
                            << "    pop %rax\n"
                            << "    mov %bx, (%rax)\n";
                }
                else if (tok.text == getKeywordStr(Keyword::LOADD))
                {
                    asm_file << "    # -- load double word --\n"
                            << "    pop %rax\n"
                            << "    xor %rbx, %rbx\n"
                            << "    mov (%rax), %ebx\n"
                            << "    push %rbx\n";
                }
                else if (tok.text == getKeywordStr(Keyword::STORED))
                {
                    asm_file << "    # -- store double word --\n"
                            << "    pop %rbx\n"
                            << "    pop %rax\n"
                            << "    mov %ebx, (%rax)\n";
                }
                else if (tok.text == getKeywordStr(Keyword::LOADQ))
                {
                    asm_file << "    # -- load quad word --\n"
                            << "    pop %rax\n"
                            << "    xor %rbx, %rbx\n"
                            << "    mov (%rax), %rbx\n"
                            << "    push %rbx\n";
                }
                else if (tok.text == getKeywordStr(Keyword::STOREQ))
                {
                    asm_file << "    # -- store quad word --\n"
                            << "    pop %rbx\n"
                            << "    pop %rax\n"
                            << "    mov %rbx, (%rax)\n";
                }

                else if (tok.text == getKeywordStr(Keyword::SHL))
                {
                    asm_file << "    # -- bitwise-shift left --\n"
                            << "    pop %rcx\n"
                            << "    pop %rbx\n"
                            << "    shl %cl, %rbx\n"
                            << "    push %rbx";
                }
                else if (tok.text == getKeywordStr(Keyword::SHR))
                {
                    asm_file << "    # -- bitwise-shift right --\n"
                            << "    pop %rcx\n"
                            << "    pop %rbx\n"
                            << "    shr %cl %rbx\n"
                            << "    push %rbx";
                }
                else if (tok.text == getKeywordStr(Keyword::OR))
                {
                    asm_file << "    # -- bitwise or --\n"
                            << "    pop %rax\n"
                            << "    pop %rbx\n"
                            << "    or %rbx, %rax\n"
                            << "    push %rax\n";
                }
                else if (tok.text == getKeywordStr(Keyword::AND))
                {
                    asm_file << "    # -- bitwise and --\n"
                            << "    pop %rax\n"
                            << "    pop %rbx\n"
                            << "    and %rbx, %rax\n"
                            << "    push %rax\n";
                }
                else if (tok.text == getKeywordStr(Keyword::MOD))
                {
                    asm_file << "    # -- modulo --\n"
                            << "    xor %rdx, %rdx\n"
                            << "    pop %rbx\n"
                            << "    pop %rax\n"
                            << "    div %rbx\n"
                            << "    push %rdx\n";
                }
            }
            instr_ptr++;
        }

        // WRITE ASM FOOTER (GRACEFUL PROGRAM EXIT, CONSTANTS)
        asm_file << "    mov $0, %rcx\n"
                << "    call exit\n"
                << '\n'
                << "    .data\n"
                << "    fmt: .string \"%u\"\n"
                << "    fmt_char: .string \"%c\"\n"
                << "    fmt_str: .string \"%s\"\n";

        // WRITE USER DEFINED STRINGS
        size_t index = 0;
        if (string_literals.size() > 0)
        {
            asm_file << "\n    # USER DEFINED STRINGS\n";
        }

        for (auto& string : string_literals)
        {
            asm_file << "str_" << index << ": .string \"" << string << "\"\n";
            index++;
        }

        // ALLOCATE MEMORY
        asm_file << '\n'
        << "    .bss\n"
        << "    .comm mem, " << MEM_CAPACITY << '\n';

        // Close open filestream.
        asm_file.close();

        Log("WIN64 GAS assembly generated at " + asm_file_path);
    }

    else
    {
        Error("Could not open file for writing. Does directory exist?");
        exit(1);
    }

}


bool Corth::handleCommandLineArgs(int argc, char **argv)
{
    static_assert(static_cast<int>(MODE::COUNT) == 2, "Exhaustive handling of supported modes in handleCMDLineArgs function");
    static_assert(static_cast<int>(PLATFORM::COUNT) == 0 , "Exhaustive handling of supported platforms in handleCMDLineArgs function");
    static_assert(static_cast<int>(ASM_SYNTAX::COUNT) == 2, "Exhaustive handling of supported assembly syntaxes in handleCMDLineArgs function");

    // No command line arguments were passed.
    if(argc == 1)
    {
        printUseage();
        return false;
    }

    for(int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-v" || arg == "--verbose")
        {
            Log("Verbose mode enabled.");
            verboseLogging = true;
        }

        else if(arg == "-o" || arg == "--output-name")
        {
            if(i + 1 < argc)
            {
                ++i;
                OUTPUT_NAME= argv[i];
            }

            else
            {
                Error("Expected name of output file to be specified after `-o`!");
                return false;
            }
        }

        else if (arg == "-a" || arg == "--assembler-path")
        {
            if (i + 1 < argc)
            {
                ++i;
                ASMB_PATH = argv[i];
            }
            else 
            {
                Error("Expected path to assembler to be specified after `-a`!");
                return false;
            }
        }
        
        else if (arg == "-ao" || arg == "--assembler-options")
        {
            if (i + 1 < argc)
            {
                i++;
                ASMB_OPTS = argv[i];
            }
            else
            {
                Error("Expected assembler options to be specified after `-ao`!");
                return false;
            }
        }

        else if (arg == "-l" || arg == "--linker-path")
        {
            if (i + 1 < argc)
            {
                i++;
                LINK_PATH = argv[i];
            }
            else
            {
                Error("Expected path to linker to be specified after `-l`!");
                return false;
            }
        }

        else if (arg == "-lo" || arg == "--linker-options")
        {
            if (i + 1 < argc)
            {
                i++;
                LINK_OPTS = argv[i];
            }
            else
            {
                Error("Expected linker options to be specified after `-lo`!");
                return false;
            }
        }

        else if (arg == "-add-ao" || arg == "--add-asm-opt")
        {
            if (i + 1 < argc)
            {
                i++;
                ASMB_OPTS.append(1, ' ');
                ASMB_OPTS += argv[i];
            }
            else
            {
                Error("Expected an assembler command line option to be specified after `-add-ao`!");
                return false;
            }
        }

        else if (arg == "-add-lo" || arg == "--add-link-opt")
        {
            if (i + 1 < argc)
            {
                i++;
                LINK_OPTS.append(1, ' ');
                LINK_OPTS += argv[i];
            }
            else
            {
                Error("Expected a linker command line option to be specified after `-add-lo`!");
                return false;
            }
        }

        // To be used if other platforms are ever supported
        else if (arg == "-win" || arg == "-win64" )
        {
            RUN_PLATFORM = PLATFORM::WIN;
        }

        #ifdef  LIN_SUPPORT
        else if (arg == "--linux" || arg == "--linux64")
        {
            RUN_PLATFORM = PLATFORM::LINUX;
        }
        #endif

        else if (arg == "-win32" || arg == "-m32" || arg == "-Wa,--32")
        {
            Error("32-bit mode is not supported!");
            return false;
        }

        #ifdef LIN_SUPPORT
        else if(arg == "-linux32")
        {
            Error("32-bit mode is not supported!");
            return false;
        }
        #endif

        else if (arg == "-com" || arg == "--compile")
        {
            RUN_MODE = MODE::COMPILE;
        }

        else if (arg == "-gen" || arg == "--generate")
        {
            RUN_MODE = MODE::GENERATE;
        }

        else if(arg == "-NASM")
        {
            ASSEMBLY_SYNTAX = ASM_SYNTAX::NASM;

            // SET PLATFORM SPECIFIC DEFAULTS
            // Defaults assume tools were installed on the default drive as in the installer.
            Corth::ASMB_PATH = "C:\\Program Files\\NASM\\nasm.exe";
            Corth::ASMB_OPTS = "-f win64";
            // Get Golink in the system
             Corth::LINK_PATH = "\\Golink\\golink.exe";
             Corth::LINK_OPTS = "/console /ENTRY:main msvcrt.dll";
        }

        else if (arg == "-GAS")
        {
            ASSEMBLY_SYNTAX = ASM_SYNTAX::GAS;
            #ifdef _WIN64
            // Defaults assume tools were installed on the same drive as Corth as well as in the root directory of the drive.
            // TODO: Might need TDM-GCC to be installed
            Corth::ASMB_PATH = "\\TDM-GCC-64\\bin\\gcc.exe";
            Corth::ASMB_OPTS = "-e main";
            Corth::LINK_PATH = "";
            Corth::LINK_OPTS = "";
            #endif

            #ifdef LIN_SUPPORT
            Corth::ASMB_PATH = "gcc";
            Corth::ASMB_OPTS = "-e main";
            Corth::LINK_PATH = "";
            Corth::LINK_OPTS = "";
            #endif
        }

        else
        {
            SOURCE_PATH = arg[i];
        }
    }

    if(SOURCE_PATH.empty())
    {
        Error("No source file specified!");
        return false;
    }

    return true;
}


bool Corth::isWhiteSpace(char& c)
{
    // Basically checks if the character is a space, tab, newline or crlf
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void Corth::pushToken(std::vector<Token> &tokList, Token &tok)
{
    // Add token to program if it is not whitespace
    if(tok.type != TokenType::WHITESPACE)
    {
        tokList.push_back(tok);
    }

    // or else reset token
    tok.type = TokenType::WHITESPACE;
    tok.text.erase();
}

// LEXER
bool Corth::lex(Program &program)
{
    std::string src = program.source;
    size_t src_end = src.length();

    static_assert(static_cast<int>(TokenType::COUNT) == 5, "Exhaustive Handling of TokenType in Lex Method");

    std::vector<Token>& toks = program.tokens;
    Token tok;
    char current = src[0];

    for(int i =0; i < src_end ++i)
    {
        current = src[i];
        tok.col_number++;

        // Skipping whitespace
        if(isWhiteSpace(current))
        {
            if(current == '\n')
            {
                tok.line_number++;
                tok.col_number = 1;
            }
        }

        else if(isOperator(current))
        {
            static_assert(OP_COUNT == 15, "Exhaustive Handling of Operators in Lex Method");
            tok.type = TokenType::OP;
            tok.text.append(1, current);

            // Look-ahead to check for multi-character operators
            i++;
            current = src[i];
            if ((tok.text == "=" && current == '=')
                || (tok.text == "<" && current == '=')
                || (tok.text == ">" && current == '=')
                || (tok.text == "<" && current == '<')
                || (tok.text == ">" && current == '>'))
            {
                tok.text.append(1, current);
            }
            else if (tok.text == "|")
            {
                if (current == '|')
                {
                    tok.text.append(1, current);
                }

                else
                {
                    Warning("Expected '|' after '|' ", tok.line_number, tok.col_number);
                }
            }

            else if (tok.text == "&")
            {
                if (current == '&')
                {
                    tok.text.append(1, current);
                }
                else
                {
                    Warning("Expected '&' following '&'", tok.line_number, tok.col_number);
                    tok.text.append(1, '&'); // Create missing text so it will still work
                }
            }

            else if (tok.text == "/" && current == '/')
            {
                // This is a comment until new-line or end of file
                tok.type = TokenType::WHITESPACE;
                while (i < src_end)
                {
                    i++;
                    current = src[i];
                    tok.col_number++;
                    if (current == '\n')
                    {
                        tok.line_number++;
                        tok.col_number = 1;
                        break;
                    }
                }
            }
            PushToken(toks, tok);
        }
        else if (isdigit(current))
        {
            tok.type = TokenType::INT;
            tok.text.append(1, current);
            // Handle multi-digit numbers
            while (i < src_end)
            {
                // Look ahead for digit
                i++;
                current = src[i];
                tok.col_number++;
                if (isdigit(current))
                {
                    tok.text.append(1, current);
                }
                else
                {
                    break;
                }
            }

            i--; // Undo lookahead.
            PushToken(toks, tok);
        }
        else if (isalpha(current))
        {
            tok.text.append(1, current);

            // Handle multiple-alpha keywords
            while (i < src_end)
            {
                // Look ahead for alpha
                i++;
                current = src[i];
                tok.col_number++;
                if (isalpha(current) || current == '_')
                {
                    tok.text.append(1, current);
                }

                else
                {
                    break;
                }
            }
            // If the token is not a keyword, output a warning and keep this token's type as whitespace
            // (effectively ensuring it will be removed)
            if (isKeyword(tok.text))
            {
                tok.type = TokenType::KEYWORD;
            }
            else {
                Error("Unidentified keyword: " + tok.text, tok.line_number, tok.col_number);
                return false;
            }

            i--; // Undo lookahead.
            PushToken(toks, tok);
        }

        else if (current == '"')
        {
            tok.type = TokenType::STRING;

            // Eat quotes
            i++;
            current = src[i];
            tok.col_number++;

            // Find closing quotes
            while (i < src_end)
            {
                if (current == '"')
                {
                    // Eat closing quotes
                    i++;
                    current = src[i];
                    tok.col_number++;
                    break;
                }

                else
                {
                    tok.text.append(1, current);
                }

                // Increment current character
                i++;
                current = src[i];
                tok.col_number++;
            }

            if (i >= src_end)
            {
                Error("Expected closing quotes following opening quotes", tok.line_number, tok.col_number);
                return false;
            }

            i--;
            PushToken(toks, tok);
        }
    }
    return true;
}


void Corth::PrintToken(Token& t)
{

    if (t.data.empty())
    {
        printf("TOKEN(%s, %s)\n", TokenTypeStr(t.type).c_str(), t.text.c_str());
    }

    else
    {
        printf("TOKEN(%s, %s, %s)\n", TokenTypeStr(t.type).c_str(), t.text.c_str(), t.data.c_str());
    }
}


void Corth::PrintTokens(Program& p)
{
    printf("%s\n", "TOKENS:");
    size_t instr_ptr = 0;
    size_t instr_ptr_max = p.tokens.size();

    for (size_t instr_ptr = 0; instr_ptr < instr_ptr_max; instr_ptr++)
    {
        printf("    %zu: ", instr_ptr);
        PrintToken(p.tokens[instr_ptr]);
    }
}


bool Corth::RemovableToken(Token& tok)
{
    if (tok.type == TokenType::WHITESPACE)
    {
        return false;
    }

    return true;
}

void Corth::TokenStackError(Token& tok)
{
    // This token could cause serious memory issues (by popping a value off the stack that doesn't exist)
    // It is marked for removal by setting it's type to whitespace.
    // Might be better to think of a better way to handle this.
    tok.type = TokenType::WHITESPACE;
    StackError(tok.line_number, tok.col_number);
}


