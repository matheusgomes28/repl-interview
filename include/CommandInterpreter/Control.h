/**
 * @file Control.h
 * @brief Defines functions used to control the overall state of the library.
 */

#ifndef COMMANDINTERPRETER_CONTROL_H
#define COMMANDINTERPRETER_CONTROL_H

#include "CommandInterpreter/LibExport.h"

#include <functional>
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


// As the specification mentioned, I can't change the signature
// of the following functions. However, I'd have added them to a common
// namespace and perhaps made it so the interpreter lib isn't 
// essentially just a "singleton", by exposing the implementation
// class and letting users have multiple command interpreters

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
CMDINT_API bool CmdInt_QueueCommand(std::string const& command);
// TODO : Maybe this should return a bool

/**
 * @brief Registers a command and the callback function for the command.
 * 
 * This must be called before you queue a command!
 */
CMDINT_API bool CmdInt_RegisterCommand(std::string const& name, repl::CommandFunction const& function);
// CmdInt_RegisterCommand("sub", funct_obj)

/**
 * @brief Registers a callback function to the command interpreter. The callback will
 *
 * The callback function will be called when a command is finished, and the input to the
 * callback is the return values from the callback.
 * 
 * @param callback a function object taking in a CommandArgument and returning a 
 * string representation of the command return value.
 */
CMDINT_API void CmdInt_RegisterCallback(repl::CommandCallback const& callback);

#endif // COMMANDINTERPRETER_CONTROL_H
