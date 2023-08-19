#ifndef COMMAND_INTERPRETER_IMPL_H
#define COMMAND_INTERPRETER_IMPL_H

#include <functional>
#include <map>
#include <queue>
#include <string>

#include <CommandInterpreter/CommandTypesHelper.h>
#include <CommandInterpreter/Control.h>
#include <CommandInterpreter/LibExport.h>

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
    class Interpreter
    {
    public:
        bool registerCommand(std::string const& command_name, CommandFunction const& handler);
        bool queueCommand(std::string const& command);
        void setCallback(CommandCallback const& callback);
        void poll();

    // Since we've implemented everything without the need for
    // worker threads/async etc, there's nothing really to
    // destroy in a special manner!
    private:
        std::map<std::string, CommandFunction> _commands;
        std::queue<Command> _command_queue;
        std::optional<CommandCallback> _callback;
    };
}
#endif //COMMAND_INTERPRETER_IMPL_H
