#include "Logging.h"
#include <fmt/core.h>

    // Dolog and Overloads
    #pragma region Logging
    void Sparrow::DoLog(const std::string& msg, std::string prefix, std::string suffix)
    {
        fmt::print(stderr, "{} {} {}", prefix, msg, suffix);
    }

    void Sparrow::DoLog(const std::string& msg, size_t line_num, std::string prefix, std::string suffix)
    {
        fmt::print(stderr, "{} LINE {} {}", prefix, line_num, msg);
        // fprintf(stderr, (prefix + " LINE %zu: %s" + suffix).c_str(), line_num, msg.c_str());
    }

    void Sparrow::DoLog(const std::string& msg, size_t line_num, size_t column_num, std::string prefix, std::string suffix)
    {
        fmt::print(stderr, "{}  LINE {}, COL {}: {}", prefix, line_num, column_num, msg);
        // fprintf(stderr, (prefix + " LINE %zu, COL %zu: %s" + suffix).c_str(), line_num, column_num, msg.c_str());
    }
    #pragma endregion


    // Error and Overloads
    #pragma region Errors
    void Sparrow::Error(const std::string& msg)
    {
        DoLog(msg, "\n[ERR]");
    }

    void Sparrow::Error(const std::string& msg, size_t line_num)
    {
        DoLog(msg, line_num, "\n[ERR]");
    }

    void Sparrow::Error(const std::string& msg, size_t line_num, size_t column_num)
    {
        DoLog(msg, line_num, column_num, "\n[ERR]");
    }

    void Sparrow::Error(const std::string& msg, std::exception e)
    {
        DoLog(msg + " (" + e.what() + ")", "\n[ERR]");
    }


    // Warning and Overloads
    #pragma region Warnings
    void Sparrow::Warning(const std::string& msg)
    {
        DoLog(msg, "[WRN]");
    }

    void Sparrow::Warning(const std::string& msg, std::size_t line_num)
    {
        DoLog(msg, line_num, "[WRN]");
    }

    void Sparrow::Warning(const std::string& msg, std::size_t line_num, std::size_t column_num)
    {
        DoLog(msg, line_num, column_num, "[WRN]");
    }
    #pragma endregion


    // Debug Log and Overloads DbgLog
    #pragma region DebugLogging
    void Sparrow::DbgLog(const std::string& msg)
    {
        DoLog(msg, "[DBG]");
    }

    void Sparrow::DbgLog(const std::string& msg, size_t line_num)
    {
        DoLog(msg, line_num, "[DBG]");
    }

    void Sparrow::DbgLog(const std::string& msg, size_t line_num, size_t column_num)
    {
        DoLog(msg, line_num, column_num, "[DBG]");
    }
    #pragma endregion


    // Logging Fuctions and Overloads
    #pragma region Logging
    void Sparrow::Log(const std::string& msg)
    {
        DoLog(msg);
    }

    void Sparrow::Log(const std::string& msg, size_t line_num)
    {
        DoLog(msg, line_num);
    }

    void Sparrow::Log(const std::string& msg, size_t line_num, size_t column_num)
    {
        DoLog(msg, line_num, column_num);
    }
    #pragma endregion

    // Error Specialized Functions
    #pragma region StackViolations
    void Sparrow::StackError()
    {
        Error("Stack protection invoked! \n"
            "Did you forget to put the operator after the operands"
            "(i.e. `5 5 +` not `5 + 5`))?");
    }

    void Sparrow::StackError(size_t line_num)
    {
        Error("Stack protection invoked! \n"
            "Did you forget to put the operator after the operands"
            "(i.e. `5 5 +` not `5 + 5`)?", line_num);
    }

    void Sparrow::StackError(size_t line_num, size_t column_num)
    {
        Error("Stack protection invoked! \n"
            "Did you forget to put the operator after the operands"
            "(i.e. `5 5 +` not `5 + 5`)?", line_num, column_num);
    }
#pragma endregion