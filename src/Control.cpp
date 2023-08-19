#include "CommandInterpreter/Control.h"

#include "CommandInterpreter/CommandTypesHelper.h"
#include "CommandInterpreter/CommandInterpreterImpl.h"

#include <array>
#include <functional>
#include <map>
#include <queue>
#include <string>
#include <string_view>
#include <vector>
#include <variant>
#include <charconv>
#include <optional>

// TODO : for unique_ptr
#include <memory>

namespace
{
	static std::unique_ptr<repl::Interpreter> command_interpreter = nullptr;
} // namespace


// std::unique_ptr<Interpreter> make_interpreter();
void CmdInt_Initialise()
{
	// Initialise your resources here.
	if (!command_interpreter)
	{
		command_interpreter = std::make_unique<repl::Interpreter>();
		return;
	}

	// Could log it's alreayd created;
}

// void destroy_interpreter(Interpreter*);
// Maybe not needed
void CmdInt_ShutDown()
{
	if (command_interpreter)
	{
		// TODO : One last poll maybe to cleanly remove the commands?
		// TODO : Long running commands should be cleaned
	}

	command_interpreter = nullptr;
}

// Executes all commands in the queue
// void execute_commands(Interpreter*);
void CmdInt_Poll()
{
	if (!command_interpreter)
	{
		// TODO : maybe log we haven't initialised yet
		return;
	}

	command_interpreter->poll();
}

// bool register_command(Interpreter*, char** command, (*handler)(arg_types))
bool CmdInt_RegisterCommand(std::string const& command_name,
  repl::CommandFunction const& command)
{
	if (!command_interpreter)
	{
		// TODO : maybe log we haven't initialised yet
		return false;
	}

	return command_interpreter->registerCommand(command_name, command);
}


// queue_command(Interpreter*, char** command_str, (*callback)(arg_type))
bool CmdInt_QueueCommand(std::string const& command_str)
{
	if (!command_interpreter)
	{
		// TODO : maybe log we haven't initialised yet
		return false;
	}

	return command_interpreter->queueCommand(command_str);
}


// we dont need it
void CmdInt_RegisterCallback(repl::CommandCallback const& callback)
{
	if (!command_interpreter)
	{
		// TODO : maybe log we haven't initialised yet
	}

	command_interpreter->setCallback(callback);
}
