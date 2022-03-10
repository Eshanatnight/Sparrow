#pragma once
#include <string>
#include <vector>
#include <sstream>


namespace Corth
{
    const unsigned int MEM_CAPACITY = 720000;
    std::string SOURCE_PATH = "main.corth";
    std::string OUTPUT_NAME = "main_corth";
    std::string ASMB_PATH = "";
    std::string LINK_PATH = "";
    std::string ASMB_OPTS = "";
    std::string LINK_OPTS = "";
    std::string ASMB_NAME = "";
    std::string LINK_NAME = "";

    enum class MODE
    {
        COMPILE,
        GENERATE,
        COUNT
    };

    MODE RUN_MODE = MODE::COMPILE;

    enum class PLATFORM
    {
        WIN,
        COUNT
    };

    PLATFORM RUN_PLATFORM = PLATFORM::WIN;

    enum class ASM_SYNTAX
    {
        NASM,
        GAS,
        COUNT
    };

    //I have not tried this with GAS Assembler yet. So there might be some bugs.
    // For the GAS Syntax. Look Corth.cpp Line: 694
    ASM_SYNTAX ASSEMBLY_SYNTAX = ASM_SYNTAX::NASM;

    bool verboseLogging = false;

    // Operators
    // Number of Operators
    const std::size_t OP_COUNT = 15;

    // Checks if the given symbol is an operator or not.
    // if it is an operator then sets the Correct Enum value.
    bool isOperator(char& c);

    // Enum Class For the Keywords that are currently supported.
    enum class Keyword
    {
        IF,
        ELSE,
        ENDIF,
        DO,
        WHILE,
        ENDWHILE,
		DUP,
        TWODUP,
        DROP,
        SWAP,
        OVER,
		DUMP,
        DUMP_C,
        DUMP_S,
		MEM,
        LOADB,
        STOREB,
		LOADW,
        STOREW,
		LOADD,
        STORED,
		LOADQ,
        STOREQ,
        SHL,
        SHR,
        OR,
        AND,
		MOD,
        COUNT
    };

    // Checks if the given symbol is a keyword or not.
    bool isKeyword(std::string& word);

    // This function outlines the corth source input and the output it will generate.
    // case <output>: { return "<input>"; }
    std::string getKeywordStr(Keyword word);

    enum class TokenType
    {
        WHITESPACE,
        INT,
        STRING,
        OP,
        KEYWORD,
        COUNT
    };

    std::string TokenTypeStr(TokenType& type);

    struct Token
    {
        public:
        TokenType type;
        std::string text;
        std::string data;
        size_t line_number;
        size_t col_number;

        Token();
    };

    struct Program
    {
        std::string source;
        std::vector<Token> tokens;
    };

    void printUseage();

    // NASM doesn't deal with strings well, so I construct hex by hand to ensure behaviour is expected.
    std::vector<std::string> stringToHex(const std::string& str);

    // Generating the ASM code for NASM ASSEMBLER
    void generateAssembly_NASM_win64(Program& program);

    // Generating the ASM code for GAS ASSEMBLER
    void generateAssembly_GAS_win64(Program& program);

    /* Handle the command line arguments that are passed to the program.
        @return value:
        * False = Execution will halt in main function
        * True = Execution will continue in main function
    */
    bool handleCommandLineArgs(int argc, char** argv);

    // Checks if a passed character is a whitespace character or not
    bool isWhiteSpace(char& c);

    // Push tokens in to the token vector
    void pushToken(std::vector<Token>& tokList, Token& tok);

    // Convert the source code into tokens
    bool lex(Program& program);

    void PrintToken(Token& t);

    void PrintTokens(Program& p);

    bool removableToken(Token& tok);

    void TokenStackError(Token& tok);

    // This function simulates the program running on its most basic level: adding and removing from the stack.
    // A read-access violation can occur if trying to pop off the stack without pushing something on first.
    // To prevent this violation during runtime of Corth, this function catches over-popping before it can happen.
    void validateTokens_stack(Program& prog);

    bool validateBlock(Program& prog, size_t& instr_ptr, size_t instr_ptr_max);

    void validateTokens_blocks(Program& prog);

    void validateTokens(Program& prog);

    // This function is my Windows version of the `where` cmd
    // this would check for the file with respect to the binary location.
    bool fileExists(const std::string& filepath);

    std::string loadFromFile(const std::string& filepath);

    void printCharactersFromFile(std::string filePath, std::string logPrefix = "[LOG]");
}