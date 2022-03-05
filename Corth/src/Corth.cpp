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
    static_assert(static_cast<int>(Keyword::COUNT) == 28, "Exhaustive handling of keywords in GetKeywordStr");

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
            Error("Unreachable in GetKeywordStr");
            exit(1);
            return "Error";
        }
    }
}


std::string Corth::TokenTypeStr(TokenType& t)
{
    static_assert(static_cast<int>(TokenType::COUNT) == 5, "Exhaustive handling of TokenType in TokenTypeStr");

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

    Error("Unreachable in TokenTypeStr");
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
            asm_file << "    ;; CORTH COMPILER GENERATED THIS ASSEMBLY -- (BY LENSOR RADII)\n"
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
                    if (tok.text == GetKeywordStr(Keyword::IF))
                    {
                        asm_file << "    ;; -- if --\n"
                                << "    pop rax\n"
                                << "    cmp rax, 0\n"
                                << "    je addr_" << tok.data << "\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::ELSE))
                    {
                        asm_file << "    ;; -- else --\n"
                                << "    jmp addr_" << tok.data << "\n"
                                << "addr_" << instr_ptr << ":\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::ENDIF))
                    {
                        asm_file << "    ;; -- endif --\n"
                                << "addr_" << instr_ptr << ":\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::WHILE))
                    {
                        asm_file << "    ;; -- while --\n"
                                << "addr_" << instr_ptr << ":\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::DO))
                    {
                        asm_file << "    ;; -- do --\n"
                                << "    pop rax\n"
                                << "    cmp rax, 0\n"
                                << "    je addr_" << tok.data << "\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::ENDWHILE))
                    {
                        asm_file << "    ;; -- endwhile --\n"
                                << "    jmp addr_" << tok.data << "\n"
                                << "addr_" << instr_ptr << ":\n";
                    }

					else if (tok.text == GetKeywordStr(Keyword::DUP))
                    {
                        asm_file << "    ;; -- dup --\n"
                                << "    pop rax\n"
                                << "    push rax\n"
                                << "    push rax\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::TWODUP))
                    {
                        asm_file << "    ;; -- twodup --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    push rbx\n"
                                << "    push rax\n"
                                << "    push rbx\n"
                                << "    push rax\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::DROP))
                    {
                        asm_file << "    ;; -- drop --\n"
                                << "    pop rax\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::SWAP))
                    {
                        asm_file << "    ;; -- swap --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    push rax\n"
                                << "    push rbx\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::OVER))
                    {
                        asm_file << "    ;; -- over --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    push rbx\n"
                                << "    push rax\n"
                                << "    push rbx\n";
                    }
					else if (tok.text == GetKeywordStr(Keyword::DUMP))
                    {
                        asm_file << "    ;; -- dump --\n"
                                << "    lea rcx, [rel fmt]\n"
                                << "    pop rdx\n"
                                << "    xor rax, rax\n"
                                << "    sub rsp, 32\n"
                                << "    call printf\n"
                                << "    add rsp, 32\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::DUMP_C))
                    {
                        asm_file << "    ;; -- dump --\n"
                                << "    lea rcx, [rel fmt_char]\n"
                                << "    pop rdx\n"
                                << "    xor rax, rax\n"
                                << "    sub rsp, 32\n"
                                << "    call printf\n"
                                << "    add rsp, 32\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::DUMP_S))
                    {
                        asm_file << "    ;; -- dump --\n"
                                << "    lea rcx, [rel fmt_str]\n"
                                << "    pop rdx\n"
                                << "    xor rax, rax\n"
                                << "    sub rsp, 32\n"
                                << "    call printf\n"
                                << "    add rsp, 32\n";
                    }

					else if (tok.text == GetKeywordStr(Keyword::MEM))
                    {
                        // Pushes the relative address of allocated memory onto the stack
                        asm_file << "    ;; -- mem --\n"
                                << "    push mem\n";

                    }
                    else if (tok.text == GetKeywordStr(Keyword::LOADB))
                    {
                        asm_file << "    ;; -- load byte --\n"
                                << "    pop rax\n"
                                << "    xor rbx, rbx\n"
                                << "    mov bl, [rax]\n"
                                << "    push rbx\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::STOREB))
                    {
                        asm_file << "    ;; -- store byte --\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    mov [rax], bl\n";
                    }
					else if (tok.text == GetKeywordStr(Keyword::LOADW))
                    {
                        asm_file << "    ;; -- load word --\n"
                                << "    pop rax\n"
                                << "    xor rbx, rbx\n"
                                << "    mov bx, [rax]\n"
                                << "    push rbx\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::STOREW))
                    {
                        asm_file << "    ;; -- store word --\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    mov [rax], bx\n";
                    }
					else if (tok.text == GetKeywordStr(Keyword::LOADD))

                    {
                        asm_file << "    ;; -- load double word --\n"
                                << "    pop rax\n"
                                << "    xor rbx, rbx\n"
                                << "    mov ebx, [rax]\n"
                                << "    push rbx\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::STORED))
                    {
                        asm_file << "    ;; -- store double word --\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    mov [rax], ebx\n";
                    }
					else if (tok.text == GetKeywordStr(Keyword::LOADQ))
                    {
                        asm_file << "    ;; -- load quad word --\n"
                                << "    pop rax\n"
                                << "    xor rbx, rbx\n"
                                << "    mov rbx, [rax]\n"
                                << "    push rbx\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::STOREQ))
                    {
                        asm_file << "    ;; -- store quad word --\n"
                                << "    pop rbx\n"
                                << "    pop rax\n"
                                << "    mov [rax], rbx\n";
                    }

                    else if (tok.text == GetKeywordStr(Keyword::SHL))
                    {
                        asm_file << "    ;; -- bitwise-shift left --\n"
                                << "    pop rcx\n"
                                << "    pop rbx\n"
                                << "    shl rbx, cl\n"
                                << "    push rbx";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::SHR))
                    {
                        asm_file << "    ;; -- bitwise-shift right --\n"
                                << "    pop rcx\n"
                                << "    pop rbx\n"
                                << "    shr rbx, cl\n"
                                << "    push rbx";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::OR))
                    {
                        asm_file << "    ;; -- bitwise or --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    or rax, rbx\n"
                                << "    push rax\n";
                    }
                    else if (tok.text == GetKeywordStr(Keyword::AND))
                    {
                        asm_file << "    ;; -- bitwise and --\n"
                                << "    pop rax\n"
                                << "    pop rbx\n"
                                << "    and rax, rbx\n"
                                << "    push rax\n";
                    }
					else if (tok.text == GetKeywordStr(Keyword::MOD))
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
