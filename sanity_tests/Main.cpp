#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

#include <CommandInterpreter/Control.h>
#include <CommandInterpreter/CommandExamples.h>

namespace
{
	bool TestRegisterCommandBeforeInit()
	{
		CmdInt::CommandHandler handler = [](std::vector<CmdInt::CommandArgument> const& /* args */) -> CmdInt::CommandArgument
		{
			return 0;
		};
		CmdInt::CommandFunction test_func{handler, std::nullopt};
		return CmdInt_RegisterCommand("something", test_func) == false;
	}

	bool TestQueueCommandBeforeInit()
	{
		return CmdInt_QueueCommand("command argument argument") == false;
	}

	bool TestQueueCommandBeforeRegisterBeforeInit()
	{
		auto const result = CmdInt_QueueCommand("command argument argument");
		return result == false;
	}

	bool TestQueueCommandBeforeRegisterAfterInit()
	{
		CmdInt_Initialise();
		auto const result = CmdInt_QueueCommand("command argument argument");
		CmdInt_ShutDown();
		return result == false;
	}

	bool TestRegisterEmptyString()
	{
		CmdInt::CommandHandler handler = [](std::vector<CmdInt::CommandArgument> const& /* args */) -> CmdInt::CommandArgument
		{
			return 0;
		};
		CmdInt::CommandFunction test_func{handler, std::nullopt};
		CmdInt_Initialise();
		auto const result = CmdInt_RegisterCommand("", test_func);
		CmdInt_ShutDown();

		return result == false;
	}

	bool TestRegisterInvalidCommand()
	{
		CmdInt::CommandHandler handler = [](std::vector<CmdInt::CommandArgument> const& /* args */) -> CmdInt::CommandArgument
		{
			return 0;
		};
		CmdInt::CommandFunction test_func{handler, std::nullopt};
		CmdInt_Initialise();
		auto const result = CmdInt_RegisterCommand("somthing with spaces", test_func);
		CmdInt_ShutDown();

		return result == false;
	}

	bool TestRegisterValidCommand()
	{
		CmdInt::CommandHandler handler = [](std::vector<CmdInt::CommandArgument> const& /* args */) -> CmdInt::CommandArgument
		{
			return 0;
		};
		CmdInt::CommandFunction test_func{handler, std::nullopt};
		CmdInt_Initialise();
		auto const result = CmdInt_RegisterCommand("some_command", test_func);
		CmdInt_ShutDown();

		return result == true;
	}

	bool TestCallCommand()
	{
		std::string result;
		CmdInt::CommandCallback callback = [&result](std::string const& output) {
			result = output;
		};
		CmdInt::CommandHandler handler = [](std::vector<CmdInt::CommandArgument> const& /* args */) -> CmdInt::CommandArgument
		{
			return std::string{"success"};
		};
		CmdInt::CommandFunction test_func{handler, std::nullopt};
		CmdInt_Initialise();
		CmdInt_RegisterCallback(callback);
		auto const register_result = CmdInt_RegisterCommand("some_command", test_func);
		if (!register_result)
		{
			return false;
		}
		
		auto const queue_result = CmdInt_QueueCommand("some_command this is a variadic function");
		CmdInt_Poll();
		CmdInt_ShutDown();

		return (queue_result == true) && (result == "success");
	}

	bool TestAddTwoNumbers()
	{
		std::string result;
		CmdInt::CommandCallback callback = [&result](std::string const& output) {
			result = output;
		};

		CmdInt_Initialise();
		CmdInt_RegisterCallback(callback);
		CmdInt_RegisterCommand("add", CmdInt::make_add_command());
		CmdInt_QueueCommand("add 5 11");
		CmdInt_Poll();
		CmdInt_ShutDown();

		return result == "16";
	}

	bool TestEcho()
	{
		std::string result;
		CmdInt::CommandCallback callback = [&result](std::string const& output) {
			result = output;
		};

		CmdInt_Initialise();
		CmdInt_RegisterCallback(callback);
		CmdInt_RegisterCommand("echo", CmdInt::make_echo_command());
		CmdInt_QueueCommand("echo hello world");
		CmdInt_Poll();
		CmdInt_ShutDown();

		// Need to either mock the std::cout, or easier,
		// also return the echoed string when finished.
		return result == "echo: finished";
	}

	bool TestCountdownValid()
	{
		std::string result;
		CmdInt::CommandCallback callback = [&result](std::string const& output) {
			result = output;
		};

		CmdInt_Initialise();
		CmdInt_RegisterCallback(callback);
		CmdInt_RegisterCommand("countdown", CmdInt::make_countdown_command());
		CmdInt_QueueCommand("countdown 10");
		
		// Mimic the real world calls that are at most 100ms apart
		// It's also better to overshoot a bit to let the command run
		// fully
		for (int i = 0; i < 110; ++i)
		{
			CmdInt_Poll();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		CmdInt_Poll();
		CmdInt_ShutDown();

		// Need to either mock the std::cout, or easier,
		// also return the echoed string when finished.
		return result == "countdown helper: completed";
	}

	bool TestCountdownInvalid1()
	{
		std::string result;
		CmdInt::CommandCallback callback = [&result](std::string const& output) {
			result = output;
		};

		CmdInt_Initialise();
		CmdInt_RegisterCallback(callback);
		CmdInt_RegisterCommand("countdown", CmdInt::make_countdown_command());
		CmdInt_QueueCommand("countdown 11");
		
		// Mimic the real world calls that are at most 100ms apart
		CmdInt_Poll();
		CmdInt_ShutDown();

		// Need to either mock the std::cout, or easier,
		// also return the echoed string when finished.
		return result == "countdown: invalid argument";
	}

	bool TestCountdownInvalid2()
	{
		std::string result;
		CmdInt::CommandCallback callback = [&result](std::string const& output) {
			result = output;
		};

		CmdInt_Initialise();
		CmdInt_RegisterCallback(callback);
		CmdInt_RegisterCommand("countdown", CmdInt::make_countdown_command());
		CmdInt_QueueCommand("countdown -1");
		CmdInt_Poll();
		CmdInt_ShutDown();

		// Need to either mock the std::cout, or easier,
		// also return the echoed string when finished.
		return result == "countdown: invalid argument";
	}

	
} // naemspace

int main(int, char**)
{
	// This executable links against your library.
	// You may perform any sanity tests here that you wish,
	// to verify that your code works as expected.
	// Please leave these tests in your final solution,
	// as they will add to your overall evaluation.

	std::vector<std::pair<std::function<bool()>, std::string>> const tests
	{
		{TestRegisterCommandBeforeInit, "TestRegisterCommandBeforeInit"},
		{TestQueueCommandBeforeInit, "TestQueueCommandBeforeInit"},
		{TestQueueCommandBeforeRegisterBeforeInit, "TestQueueCommandBeforeRegisterBeforeInit"},
		{TestQueueCommandBeforeRegisterAfterInit, "TestQueueCommandBeforeRegisterAfterInit"},
		{TestRegisterEmptyString, "TestRegisterEmptyString"},
		{TestRegisterInvalidCommand, "TestRegisterInvalidCommand"},
		{TestRegisterValidCommand, "TestRegisterValidCommand"},
		{TestRegisterValidCommand, "TestRegisterValidCommand"},
		{TestCallCommand, "TestCallCommand"},

		// Next up is testing the example functions provided, as
		// per the specification
		{TestAddTwoNumbers, "TestAddTwoNumbers"},
		{TestEcho, "TestEcho"},
		{TestCountdownValid, "TestCountdownValid"},
		{TestCountdownInvalid1, "TestCountdownInvalid1"},
		{TestCountdownInvalid2, "TestCountdownInvalid2"}
	};

	auto const tests_passed = std::all_of(begin(tests), end(tests), [](auto const& test_name_pair){
		auto const result = test_name_pair.first();
		if (!result)
		{
			std::cout << test_name_pair.second << " failed!\n";
		}

		return true;
	});

	// I can write a lot more tests for this, specially the ones
	// that can go wrong. I tried to think about as many edge cases
	// as possible, but there will undoubtedly be bugs somewhere. If I
	// had more time, I'd probably write a lot more tests.


	// Again, I could do way more tests given I had more time
	return tests_passed ? 0 : -1;
}
