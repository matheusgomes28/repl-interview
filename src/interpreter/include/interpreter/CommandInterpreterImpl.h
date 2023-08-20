#ifndef COMMAND_INTERPRETER_IMPL_H
#define COMMAND_INTERPRETER_IMPL_H

#include <interpreter/interpreter_export.h>

#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <string>

#include "interpreter/interpreter_export.h"
#include <interpreter/CommandTypesHelper.h>
// #include <interpreter/Control.h>

namespace repl
{
    /**
     * @brief Command function / argument pairs for execution
     * 
     * for implementation use only.
     */
    struct Command
    {
        CommandFunction command;
        std::vector<CommandArgument> arguments;
    };

    /**
     * @brief This class is the implementation of the CommandInterpreter
     * 
     * Objects of CommandInterpreter type will hold their own
     * registered command map, callback and command execution queue.
     */
    REPL_INTERPRETER_EXPORT class Interpreter
    {
    public:
        bool registerCommand(std::string const& command_name, CommandFunction const& handler);
        bool queueCommand(std::string const& command);
        void setCallback(CommandCallback const& callback);
        void poll(); // rename this to execute commands

    // Since we've implemented everything without the need for
    // worker threads/async etc, there's nothing really to
    // destroy in a special manner!
    private:
        std::map<std::string, CommandFunction> _commands;
        std::queue<Command> _command_queue;
        std::optional<CommandCallback> _callback;
    };

    // TODO redo documentation
    REPL_INTERPRETER_EXPORT std::unique_ptr<Interpreter> make_interpreter();
} // namespace repl
#endif //COMMAND_INTERPRETER_IMPL_H
