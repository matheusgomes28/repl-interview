/**
 * @file CommandExamples.h
 * @brief Examples of how to define basic commands
 * 
 */
#ifndef COMMAND_EXAMPLES_H
#define COMMAND_EXAMPLES_H

#include <CommandInterpreter/Control.h>
#include <CommandInterpreter/LibExport.h>

#include <chrono>
#include <iostream>
#include <optional>
#include <vector>

namespace CmdInt
{
	CMDINT_API CommandArgument add_handler(std::vector<CommandArgument> const& arguments);
	CMDINT_API CommandArgument echo_handler(std::vector<CommandArgument> const& arguments);
	CMDINT_API CommandArgument countdown_handler(std::vector<CommandArgument> const& arguments);

    CMDINT_API CommandFunction make_add_command();

    CMDINT_API CommandFunction make_echo_command();

    CMDINT_API CommandFunction make_countdown_command();
} // namespace CmdInt

#endif // COMMAND_EXAMPLES_H
