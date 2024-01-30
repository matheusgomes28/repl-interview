import cmd_interpreter;

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace {

    std::string echo_handler(std::vector<std::string> const& args) {
        for (auto const& arg : args)
        {
            std::cout << arg << " ";
        }
        std::cout << std::endl;

        return "echo done";
    }

    bool test_empty_command() {
        CommandInterpreter interpreter;
        interpreter.add_command("echo", echo_handler);
        return !interpreter.queue_command("");
    }

    bool test_one_word_command() {
        CommandInterpreter interpreter;
        interpreter.add_command("echo", echo_handler);
        if (!interpreter.queue_command("echo")){
            return false;
        }
        interpreter.process_commands();
        return true;
    }

    bool test_multiple_word_command() {
        CommandInterpreter interpreter;
        interpreter.add_command("echo", echo_handler);
        std::vector<std::string> const commands {
            "echo hello world",
            "echo szop",
            "echo antonio brother",
            "echo ermanas my man some more text"
        };

        for (auto const& command : commands) {
            if (!interpreter.queue_command(command)) {
                return false;
            }
            interpreter.process_commands();
        }

        return true;
    }

    struct TestCase {
        std::string test_name;
        std::function<bool()> test_func;
    };
    
} // namespace

int main()
{
    std::vector<TestCase> const all_tests {
        {"empty command", test_empty_command},
        {"one word command", test_one_word_command},
        {"multiple words command", test_multiple_word_command},
    };

    for (auto const& test : all_tests)
    {
        std::cout << "Running test " << test.test_name << '\n';
        if (!test.test_func()) {
            std::cout << "Test failed\n";
            std::exit(-1);
        }
    }
}
