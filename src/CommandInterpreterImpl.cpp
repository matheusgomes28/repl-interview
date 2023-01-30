#include "CommandInterpreter/CommandInterpreterImpl.h"

#include <iterator>
#include <string>
#include <string_view>
#include <vector>

namespace
{
	/**
	 * @brief Splits the string_view on the spaces
	 * 
	 * @param str view to the string to be split
	 * @return std::vector<std::string_view> containing substrings,
	 * where each element is between white spaces
	 * 
	 * Note: I obtained this from a blog post about how
	 * to efficiently split string_views:
	 * https://www.cppstories.com/2018/07/string-view-perf-followup/
	 * 
	 * Not sure why I focused on string efficiecy too much at the
	 * start of the challenge, it probably doesn't make much difference
	 * if I switched to std::string;s all the way as opposed to views.
	 */
	std::vector<std::string_view> space_split(std::string_view str)
	{
		constexpr std::string_view delims{" "};
		std::vector<std::string_view> output;

		for (auto first = str.data(),
		       second = str.data(),
			   last = first + str.size();
			   second != last && first != last;
			   first = second + 1) {
			second = std::find_first_of(first, last, std::cbegin(delims), std::cend(delims));

			if (first != second)
				output.emplace_back(first, second - first);
		}

		return output;
	}

	std::optional<std::vector<CmdInt::CommandArgument>> parse_arguments(
		std::vector<std::string_view> const& str_arguments,
		CmdInt::ArgumentTypes const& types)
	{
		if (str_arguments.size() != types.size())
		{
			return std::nullopt;;
		}
		
		// Attempt to cast each string to right arg
		std::vector<CmdInt::CommandArgument> arguments;

		for (std::size_t i = 0; i < str_arguments.size(); ++i)
		{
			std::optional<CmdInt::CommandArgument> curr_argument = std::nullopt;
			switch (types[i])
			{
			case CmdInt::ArgumentType::INT:
				curr_argument = CmdInt::TypeConverter<CmdInt::ArgumentType::INT>::convert(str_arguments[i]);
				break;
			case CmdInt::ArgumentType::STR:
				curr_argument = CmdInt::TypeConverter<CmdInt::ArgumentType::STR>::convert(str_arguments[i]);
				break;
			case CmdInt::ArgumentType::FLOAT:
				curr_argument = CmdInt::TypeConverter<CmdInt::ArgumentType::FLOAT>::convert(str_arguments[i]);
			}

			// if callback was set, call it
			if (!curr_argument)
			{
				return std::nullopt;
			}

			arguments.push_back(*curr_argument);
		}

		return arguments;
	}

	std::vector<CmdInt::CommandArgument> make_arguments(std::vector<std::string_view> const& str_arguments)
	{
		std::vector<CmdInt::CommandArgument> command_arguments;
		std::transform(begin(str_arguments),
		  end(str_arguments),
		  back_inserter(command_arguments),
		  [](auto const& arg) {
			return std::string{begin(arg), end(arg)};
		  });
		return command_arguments;
	}
} // namespace

bool CmdInt::CommandInterpreter::queueCommand(std::string const& command_str)
{
	// TODO Check the command_str split isn't empty
	auto const split_commands = space_split(command_str);
	auto const command_name = std::string{begin(split_commands[0]), end(split_commands[0])};

	auto const found = _commands.find(command_name);
	if (found == end(_commands))
	{
		if (_callback)
		{
			(*_callback)("Unrecognised command: " + command_name);
		}
		return false;
	}

	std::vector<std::string_view> const str_arguments{next(begin(split_commands)), end(split_commands)};

	// Check if the right size arguments are provided, if
	// the user has provided a signature
	auto const& command = found->second;
	if (command.argument_types)
	{
		auto const maybe_arguments = parse_arguments(str_arguments, *command.argument_types);
		if (!maybe_arguments)
		{
			return false;
		}

		_command_queue.push({command, *maybe_arguments});
		return true;
	}

	_command_queue.push({command, make_arguments(str_arguments)});
	return true;
}

bool CmdInt::CommandInterpreter::registerCommand(std::string const& command_name, CommandFunction const& handler)
{
	// either an empty string or it has spaces...
	auto const split_commands = space_split(command_name);
	if (split_commands.size() != 1)
	{
		return false;
	}

	auto const found = _commands.find(command_name);
	if (found != end(_commands))
	{
		return false;
	}

	_commands[command_name] = handler;
	return true;
}

void CmdInt::CommandInterpreter::poll()
{
	// Perform any processing here.
	for (std::size_t i = 0; i < _command_queue.size(); ++i)
	{
		auto const command_request = _command_queue.front();
		auto const result = command_request.command.f(command_request.arguments);
		
		if (_callback)
		{

			auto const out_str = std::visit(overloaded{
				[](std::string const& arg) -> std::optional<std::string> { return arg; },
				[](int const& arg) -> std::optional<std::string> { return std::to_string(arg); },
				[](float const& arg) -> std::optional<std::string> { return std::to_string(arg); },
				[](auto const& arg) -> std::optional<std::string> { return std::nullopt; }
			}, result);

			if (out_str)
			{
				// Should not happen, but lets keep this here
				// for now
				(*_callback)(*out_str);
			}
		}

		_command_queue.pop();
		// TODO : Call the callback with the resturned result
	}
}

void CmdInt::CommandInterpreter::setCallback(CommandCallback const& callback)
{
	_callback = callback;
}
