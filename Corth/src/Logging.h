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
    void DoLog(std::string msg, std::string prefix = "[LOG]", std::string = "\n");
    void DoLog(std::string msg, size_t line_num, std::string prefix = "[LOG]", std::string suffix = "\n");
    void DoLog(std::string msg, size_t line_num, size_t column_num, std::string prefix = "[LOG]", std::string suffix = "\n");

    // Error and Overloads
    void Error(std::string msg);
    void Error(std::string msg, std::size_t line_num);
    void Error(std::string msg, std::size_t line_num, std::size_t column_num);
    void Error(std::string msg, std::exception e);

    // Warning and Overloads
    void Warning(std::string msg);
    void Warning(std::string msg, std::size_t line_num);
    void Warning(std::string msg, std::size_t line_num, std::size_t column_num);

    // Debug Log and Overloads DbgLog
    void DbgLog(std::string msg);
    void DbgLog(std::string msg, std::size_t line_num);
    void DbgLog(std::string msg, std::size_t line_num, std::size_t column_num);

    // Logging Functions
    void Log(std::string msg);
    void Log(std::string msg, std::size_t line_num);
    void Log(std::string msg, std::size_t line_num, std::size_t column_num);

    // Error Specializations
    void StackError();
    void StackError(std::size_t line_num);
    void StackError(std::size_t line_num, std::size_t column_num);
}


