#include <iostream>

#include <CommandInterpreter/Control.h>

namespace
{
	CommandArgument add(std::vector<CommandArgument> const& arguments)
	{
		int sum = 0;
		for (auto const& argument : arguments)
		{
			auto const maybe_val = get_argument<int>(argument);
			if (!maybe_val)
			{
				std::cout << "TODO : Maybe return an error here\n";
				return -1;
			}
			sum += *maybe_val;
		}

		return sum;
	}

	CommandArgument echo(std::vector<CommandArgument> const& arguments)
	{
		for (auto const& argument : arguments)
		{
			auto const maybe_val = get_argument<std::string>(argument);
			if (!maybe_val)
			{
				std::cout << "TODO : Maybe return an error here\n";
				return -1;
			}
			std::cout << *maybe_val << " ";
		}
		return 1;
	}

	CommandArgument countdown(std::vector<CommandArgument> const& arguments)
	{
		return std::string{"not implemented"};
	}
} // naemspace

int main(int, char**)
{
	// This executable links against your library.
	// You may perform any sanity tests here that you wish,
	// to verify that your code works as expected.
	// Please leave these tests in your final solution,
	// as they will add to your overall evaluation.

	CommandFunction add_function{add, std::make_optional<ArgumentTypes>({ArgumentType::INT, ArgumentType::INT})};
	CmdInt_RegisterCommand("add", add_function);

	CommandFunction echo_function{echo, std::nullopt};
	CmdInt_RegisterCommand("echo", echo_function);

	
	// Test the shit here
	auto const res1 = CmdInt_ProcessCommand("add 1 2");
	auto const res2 = CmdInt_ProcessCommand("add 1 3");
	auto const res3 = CmdInt_ProcessCommand("add 1 11qq");
	auto const res4 = CmdInt_ProcessCommand("add 1 hello");
	auto const res5 = CmdInt_ProcessCommand("add 1 2147483655");

	auto const res6 = CmdInt_ProcessCommand("echo 1 2");
	auto const res7 = CmdInt_ProcessCommand("echo 1");
	auto const res8 = CmdInt_ProcessCommand("echo hello there");
	auto const res9 = CmdInt_ProcessCommand("echo hello there you beautiful human being");
	
	CmdInt_Poll();

	return 0;
}
