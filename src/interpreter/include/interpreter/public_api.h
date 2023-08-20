/**
 * @file Control.h
 * @brief Defines functions used to control the overall state of the library.
 */

#ifndef COMMANDINTERPRETER_CONTROL_H
#define COMMANDINTERPRETER_CONTROL_H

#include "interpreter/interpreter_export.h"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <variant>

namespace repl
{
    /**
     * @brief Argument type enums
     * 
     * Used to optioanlly type-check command inputs,
     * for more information, see `CommandFunction` and
     * `CmdInt_RegisterCommand`
     */
    enum class ArgumentType
    {
        INT,
        FLOAT,
        STR
    };

    using ArgumentTypes = std::vector<ArgumentType>;

    /**
     * @brief Bread and butter of the command interpreter library
     * 
     * Considered many type-puning techniques in C++, but decided to use
     * variants for the arguments as it's fairly type-safe and should give
     * you compilation errors when you change (add) more types to it without
     * changing the handlers throughout the code 
     */
    using CommandArgument = std::variant<int, std::string, float>;


    using CommandHandler = std::function<CommandArgument(std::vector<CommandArgument> const&)>;
    /**
     * @brief Command function pair, function & expected argument types.
     * 
     * Stores the command function and the optional argument types. If
     * argument types are provided, it means that input type checking
     * will be done when queueing commands. If no argument types are
     * provided (std::nullopt), this is a variadic function and the user
     * is responsible for checking the inputs. 
     */
    struct CommandFunction
    {
        std::function<CommandArgument(std::vector<CommandArgument> const&)> f;
        std::optional<ArgumentTypes> argument_types;
    };

    using CommandCallback = std::function<void(std::string const&)>;
} // namespace CmdInt 

#endif // COMMANDINTERPRETER_CONTROL_H
