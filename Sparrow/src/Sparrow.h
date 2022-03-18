#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace Sparrow
{
    /*
        * ENUM For different compiler modes
        * Compile
        * Generate -> Assembly
        * Count -> Number Of Modes
    */
    enum class MODE
    {
        COMPILE,
        GENERATE,
        COUNT
    };


    /*
        * ENUM For different Enums for different platforms
        * if i ever update this to support linux
        *
        * WIN -> Windows
        * COUNT -> Number Of Platforms
    */
    enum class PLATFORM
    {
        WIN,
        COUNT
    };


    /*
        * ENUM for different assemblers
        * NASM -> NASM Assembler
        * GAS -> GNU Assembler (This is not tested properly)
        * COUNT -> Number Of Assemblers
    */
    enum class ASM_SYNTAX
    {
        NASM,
        GAS,
        COUNT
    };


    /*
        *  Enum Class For the Keywords that are currently supported.
        *  This is used to check if the given symbol is a keyword or not.
        * Note To self: What were you thinking when you wrote this?
    */
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
        OPEN_FILE,
        WRITE_TO_FILE,
        CLOSE_FILE,
        LENGTH_S,
        WRITE,
        WRITE_PLUS,
        APPEND,
        APPEND_PLUS,
        COUNT
    };

    /*
        * Checks if the given symbol is an operator or not.
        * if it is an operator then sets the Correct Enum value.
    */
    bool isOperator(char& c);

    /*
        * Checks if the given symbol is a keyword or not.
        * @return: bool
        * @param: std::string&
    */
    bool isKeyword(std::string& word);

    /*
        * This function outlines the sparrow source input and the output it will generate.
        * case <output>: { return "<input>"; }
    */
    std::string getKeywordStr(Keyword word);

    /*
        * Enum Class for TokenTypes used in our lexer.
    */
    enum class TokenType
    {
        WHITESPACE,
        INT,
        STRING,
        OP,
        KEYWORD,
        COUNT
    };

    /*
        * Function that returns the TokenType of the given symbol.
        * if say an int is parsed, as a TokenType then return a string "INTEGER"
        * @return: std::string
        * @param: TokenType&
    */
    std::string TokenTypeStr(TokenType& type);

/*
    * Data Structure for a Token
*/
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

    /*
        * Data Structure for the Program
        * this would collect the code as a string and the tokens as a vector
    */
    struct Program
    {
        std::string source;
        std::vector<Token> tokens;
    };

    /*
        * Function that prints out the useage of the compiler
    */
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

    // Print the tokens
    void PrintToken(Token& t);

    // Print all the tokens on the token vector in a program
    void PrintTokens(Program& p);

    // parses for the removeable Tokens
    bool removableToken(Token& tok);

    /*
        * This token could cause serious memory issues (by popping a value off the stack that doesn't exist)
        * It is marked for removal by setting it's type to whitespace.
        * Might be better to think of a better way to handle this.
    */
    void TokenStackError(Token& tok);

    /*
        * This function simulates the program running on its most basic level: adding and removing from the stack.
        * A read-access violation can occur if trying to pop off the stack without pushing something on first.
        * To prevent this violation during runtime of Sparrow, this function catches over-popping before it can happen.
    */
    void validateTokens_stack(Program& prog);

    /*
        * Validates the Block of code that is being parsed. Prevents the runtime violations that can occur.
    */
    bool validateBlock(Program& prog, size_t& instr_ptr, size_t instr_ptr_max);

    void validateTokens_blocks(Program& prog);

    void validateTokens(Program& prog);

    /*
        * This function is my Windows version of the `where` cmd
         *this would check for the file with respect to the binary location.
    */
    bool fileExists(const std::string& filepath);

    std::string loadFromFile(const std::string& filepath);

    // Uses fopen_s
    void printCharactersFromFile__fopen_s(const std::string& filePath, const std::string logPrefix);

    // Uses depricated fopen
    void printCharactersFromFile(std::string filePath, std::string logPrefix = "[LOG]");

    void staticCheck();
    std::string getSourceFile(int argc, char** argv);
}


extern Sparrow::MODE RUN_MODE;
extern Sparrow::PLATFORM RUN_PLATFORM;

//I have not tried this with GAS Assembler yet. So there might be some bugs.
// For the GAS Syntax. Look Sparrow.cpp Line: 694
extern Sparrow::ASM_SYNTAX ASSEMBLY_SYNTAX;

// move to a sparrow namespace?
extern bool verboseLogging;

// Number of Operators
const std::size_t OP_COUNT = 15;

const unsigned int MEM_CAPACITY = 720000;
extern std::string SOURCE_PATH;
extern std::string OUTPUT_NAME;
extern std::string ASMB_PATH;
extern std::string LINK_PATH;
extern std::string ASMB_OPTS;
extern std::string LINK_OPTS;
extern std::string ASMB_NAME;
extern std::string LINK_NAME;
