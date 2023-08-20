#include <interpreter/CommandInterpreterImpl.h>
#include <interpreter/public_api.h>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

namespace
{
    void print_callback(std::string const& result)
    {
        std::cout << ">>> " << result << std::endl;
    }

    static bool quit = false;
    repl::CommandArgument quit_callback(std::vector<repl::CommandArgument> const&)
    {
        quit = true;
        return 0;
    }

    repl::CommandArgument
    add_handler(std::vector<repl::CommandArgument> const& arguments)
    {
        int sum = 0;
        for (auto const& argument : arguments)
        {
            auto const str_val = repl::convert_argument<std::string>(argument).value_or("0");
            auto const maybe_val = repl::TypeConverter<repl::ArgumentType::INT>::convert(str_val);
            if (!maybe_val)
            {
                return 0;
            }
            sum += repl::convert_argument<int>(*maybe_val).value_or(0);
        }

        return sum;
    }

    repl::CommandArgument
    echo_handler(std::vector<repl::CommandArgument> const& arguments)
    {
        std::ostringstream output;
        for (auto const& argument : arguments)
        {
            auto const maybe_val = repl::convert_argument<std::string>(argument);
            if (!maybe_val)
            {
                return std::string{"echo: invalid argument"};
            }
            output << *maybe_val << " ";
        }
        
        return output.str();
    }
} // namespace

int main()
{
    auto interpreter = repl::make_interpreter();

    // Add all necessary function
    interpreter->registerCommand("echo", {echo_handler, std::nullopt});
    interpreter->registerCommand("add", {add_handler, std::nullopt});
    interpreter->registerCommand("quit", {quit_callback, std::nullopt});
    interpreter->setCallback(print_callback);

    // The repl loop
    std::string query;
    while (!quit)
    {
        std::cout << ">> ";
        std::getline(std::cin, query);

        interpreter->queueCommand(query);
        interpreter->poll();
    }
}
