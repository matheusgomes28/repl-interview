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
	static std::unique_ptr<CmdInt::CommandInterpreter> command_interpreter = nullptr;
} // namespace



void CmdInt_Initialise()
{
	// Initialise your resources here.
	if (!command_interpreter)
	{
		command_interpreter = std::make_unique<CmdInt::CommandInterpreter>();
		return;
	}

	// Could log it's alreayd created;
}

void CmdInt_ShutDown()
{
	if (command_interpreter)
	{
		// TODO : One last poll maybe to cleanly remove the commands?
		// TODO : Long running commands should be cleaned
	}

	command_interpreter = nullptr;
}

void CmdInt_Poll()
{
	if (!command_interpreter)
	{
		// TODO : maybe log we haven't initialised yet
		return;
	}

	command_interpreter->poll();
}

bool CmdInt_RegisterCommand(std::string const& command_name,
  CmdInt::CommandFunction const& command)
{
	if (!command_interpreter)
	{
		// TODO : maybe log we haven't initialised yet
		return false;
	}

	return command_interpreter->registerCommand(command_name, command);
}

bool CmdInt_QueueCommand(std::string const& command_str)
{
	if (!command_interpreter)
	{
		// TODO : maybe log we haven't initialised yet
		return false;
	}

	return command_interpreter->queueCommand(command_str);
}

void CmdInt_RegisterCallback(CmdInt::CommandCallback const& callback)
{
	if (!command_interpreter)
	{
		// TODO : maybe log we haven't initialised yet
	}

	command_interpreter->setCallback(callback);
}
