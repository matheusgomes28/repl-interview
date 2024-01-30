import cmd_interpreter;

#include <cstdint>
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

    std::string add_handler(std::vector<std::string> const& args) {
        std::size_t count = 0;
        for (auto const& arg : args)
        {
            count += std::atoll(arg.c_str());
        }
        std::cout << count << std::endl;
        return "count done";
    }

    std::string countdown(std::vector<std::string> const& args) {
        std::size_t count = 0;
        for (auto const& arg : args)
        {
            count += std::atoll(arg.c_str());
        }
        std::cout << count << std::endl;
        return "count done";
    }
} // namespace


int main()
{
    CommandInterpreter interpreter;
    interpreter.add_command("echo", echo_handler);
    interpreter.add_command("add", add_handler);

    while(true)
    {
        std::cout << ">> ";
        
        std::string command;
        std::getline(std::cin, command);

        if (!interpreter.queue_command(command)) {
            std::cout << "invalid command!\n";
            continue;
        }
        interpreter.process_commands();
    }
}