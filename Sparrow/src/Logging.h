#pragma once
#include <string>


// defines a helpful set of logging functions for the CCLI.
namespace Sparrow
{
    // Log Message, Would it be better to use a stringpool?
    // Keeping this for now. But the stringpool most definately a better idea.
    // Note: Maybe use RValue Moves? or Would the constructing the message in place be better?
    const static std::string STACK_VIOLATION_ERROR_MESSAGE = "Stack Protection Invoked.";

    // Dolog and Oveloads
    void DoLog(const std::string& msg, std::string prefix = "[LOG]", std::string = "\n");
    void DoLog(const std::string& msg, size_t line_num, std::string prefix = "[LOG]", std::string suffix = "\n");
    void DoLog(const std::string& msg, size_t line_num, size_t column_num, std::string prefix = "[LOG]", std::string suffix = "\n");

    // Error and Overloads
    void Error(const std::string& msg);
    void Error(const std::string& msg, std::size_t line_num);
    void Error(const std::string& msg, std::size_t line_num, std::size_t column_num);
    void Error(const std::string& msg, std::exception e);

    // Warning and Overloads
    void Warning(const std::string& msg);
    void Warning(const std::string& msg, std::size_t line_num);
    void Warning(const std::string& msg, std::size_t line_num, std::size_t column_num);

    // Debug Log and Overloads DbgLog
    void DbgLog(const std::string& msg);
    void DbgLog(const std::string& msg, std::size_t line_num);
    void DbgLog(const std::string& msg, std::size_t line_num, std::size_t column_num);

    // Logging Functions
    void Log(const std::string& msg);
    void Log(const std::string& msg, std::size_t line_num);
    void Log(const std::string& msg, std::size_t line_num, std::size_t column_num);

    // Error Specializations
    void StackError();
    void StackError(std::size_t line_num);
    void StackError(std::size_t line_num, std::size_t column_num);
}


