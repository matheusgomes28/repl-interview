/**
 * @file Control.h
 * @brief Defines functions used to control the overall state of the library.
 */

#ifndef COMMANDINTERPRETER_CONTROL_H
#define COMMANDINTERPRETER_CONTROL_H

#include "CommandInterpreter/LibExport.h"

#include <functional>
#include <string>
#include <variant>
#include <optional>
 
 // TODO : Document this
enum class ArgumentType
{
    INT,
    FLOAT,
    STR
};

using CommandArgument = std::variant<int, std::string>;
using ArgumentTypes = std::vector<ArgumentType>;

struct CommandFunction
{
    std::function<CommandArgument(std::vector<CommandArgument>)> f;
    std::optional<ArgumentTypes> argument_types;
};

// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template <typename T>
std::optional<T> get_argument(CommandArgument const& argument)
{
    return std::visit(overloaded{
        [](T const& arg) -> std::optional<T> { return arg; },
        [](auto const& arg) -> std::optional<T> { return std::nullopt; }
    }, argument);
}

/**
 * @brief Initialises resources within the library.
 *
 * Any calls to library functions will be ignored if the library
 * is not initialised.
 *
 * A call to this function should be paired eventually with a call
 * to @a CmdInt_ShutDown(), in order to clean up the resources.
 */
CMDINT_API void CmdInt_Initialise();

/**
 * @brief Cleans up resources within the library.
 *
 * This function should be called once the library has finished
 * being used.
 */
CMDINT_API void CmdInt_ShutDown();

/**
 * @brief Invokes internal processing on the library.
 *
 * This function should be called regularly in order to process commands.
 */
CMDINT_API void CmdInt_Poll();

/**
 * @brief Adds command to the command queue.
 *
 * This function should be called with a valid command that has
 * previously been registered.
 */
CMDINT_API bool CmdInt_ProcessCommand(std::string const& command);
// TODO : Maybe this should return a bool

/**
 * @brief Registers a command and the callback function for the command.
 * 
 * This 
 */
CMDINT_API bool CmdInt_RegisterCommand(std::string const& name, CommandFunction const& function);

#endif // COMMANDINTERPRETER_CONTROL_H
