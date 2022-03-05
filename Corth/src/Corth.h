#pragma once
#include <string>
#include <vector>

namespace Corth
{
    const unsigned int MEM_CAPACITY = 720000;
    std::string SOURCE_PATH = "main.corth";
    std::string OUTPUT_NAME = "main_corth";
    std::string ASMB_PATH = "";
    std::string LINK_PATH = "";
    std::string ASMB_OPT = "";
    std::string LINK_OPT = "";
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
        LINUX,
        COUNT
    };

    PLATFORM RUN_PLATFORM = PLATFORM::WIN;

    enum class ASM_SYNTAX
    {
        NASM,
        GAS,
        COUNT
    };

    ASM_SYNTAX ASSEMBLY_SYNTAX = ASM_SYNTAX::NASM;

    bool verboseLogging = false;

    // Operators
    const std::size_t OP_COUNT = 15;
    bool isOperator(char& c);

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

    // Generating the ASM code
    void generateAssembly_NASM_win64(Program& program);
}