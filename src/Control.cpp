#include "CommandInterpreter/Control.h"

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

namespace
{
	std::vector<std::string_view> split(std::string_view str)
	{
		constexpr std::string_view delims{" "};
		std::vector<std::string_view> output;

		for (auto first = str.data(), second = str.data(), last = first + str.size(); second != last && first != last; first = second + 1) {
			second = std::find_first_of(first, last, std::cbegin(delims), std::cend(delims));

			if (first != second)
				output.emplace_back(first, second - first);
		}

		return output;
	}

	struct Command
	{
		CommandFunction command;
		std::vector<CommandArgument> arguments;
	};

	std::map<std::string, CommandFunction> commands;
	std::queue<Command> command_queue;


	// Rename this to argument converter
	template <ArgumentType T>
	struct TypeConverter
	{
		static std::optional<CommandArgument> convert(std::string_view const& argument) = delete;
	};

	template <>
	struct TypeConverter<ArgumentType::STR>
	{
		static std::optional<CommandArgument> convert(std::string_view const& argument)
		{
			return std::string{begin(argument), end(argument)};
		}
	};

	template<>
	struct TypeConverter<ArgumentType::INT>
	{
		static std::optional<CommandArgument> convert(std::string_view const& str)
		{
			int result;
			auto const err = std::from_chars(str.data(), str.data() + str.size(), result);

			// there were chars in the string not matching
			// the int pattern
			if (err.ptr != end(str))
			{
				return std::nullopt;
			}

			// No char matched the pattern or the number
			// can't be represented by the int type
			if ((err.ec == std::errc::invalid_argument) || (err.ec == std::errc::result_out_of_range))
			{
				return std::nullopt;
			}

			return result;
		}
	};

	std::optional<std::vector<CommandArgument>> parse_arguments(std::vector<std::string_view> const& str_arguments, ArgumentTypes const& types)
	{
		if (str_arguments.size() != types.size())
		{
			return std::nullopt;;
		}
		
		// Attempt to cast each string to right arg
		std::vector<CommandArgument> arguments;

		for (auto i = 0; i < str_arguments.size(); ++i)
		{
			std::optional<CommandArgument> curr_argument = std::nullopt;
			switch (types[i])
			{
			case ArgumentType::INT:
				curr_argument = TypeConverter<ArgumentType::INT>::convert(str_arguments[i]);
			case ArgumentType::STR:
				curr_argument = TypeConverter<ArgumentType::STR>::convert(str_arguments[i]);
			}

			if (!curr_argument)
			{
				return std::nullopt;
			}
			arguments.push_back(*curr_argument);
		}

		return arguments;
	}

	std::vector<CommandArgument> make_arguments(std::vector<std::string_view> const& str_arguments)
	{
		std::vector<CommandArgument> command_arguments;
		std::transform(begin(str_arguments),
		  end(str_arguments),
		  back_inserter(command_arguments),
		  [](auto const& arg) {
			return std::string{begin(arg), end(arg)};
		  });
		return command_arguments;
	}
} // namespace



void CmdInt_Initialise()
{
	// Initialise your resources here.
}

void CmdInt_ShutDown()
{
	// Clean up your resources here.
}

void CmdInt_Poll()
{
	// Perform any processing here.
	while (!command_queue.empty())
	{
		auto const command_request = command_queue.front();
		auto const result = command_request.command.f(command_request.arguments);
		command_queue.pop();
		// TODO : Call the callback with the resturned result
	}
}

bool CmdInt_RegisterCommand(std::string const& command_name, CommandFunction const& command)
{
	auto const found = commands.find(command_name);
	if (found != end(commands))
	{
		// TODO could log 
		return false;
	}

	commands[command_name] = command;
	return true;
}

bool CmdInt_ProcessCommand(std::string const& command_str)
{
	auto const command_name = command_str.substr(0, command_str.find_first_of(' '));

	auto const found = commands.find(command_name);
	if (found == end(commands))
	{
		// TODO : could also log
		return false;
	}

	auto const split_command = split(command_str);
	std::vector<std::string_view> const str_arguments{next(begin(split_command)), end(split_command)};

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

		command_queue.push({command, *maybe_arguments});
		return true;
	}

	command_queue.push({command, make_arguments(str_arguments)});
	return true;
}
