#include <CommandInterpreter/Control.h>
#include <CommandInterpreter/CommandExamples.h>
#include <CommandInterpreter/CommandTypesHelper.h>

#include <iostream>
#include <sstream>

namespace
{

    // This is basically cheating to get what the specification wants.
    // As I stayed away from different threads, async programming, &
    // the heap as much as possible, so this was the solution to the countdown
    // without blocking.
    
    // A neat way of achieving the countdown
    // effect is to add another appropriate countdown command request to the
    // queue with the decreased timer, if a second has passed. Otherwise, just
    // add the same countdown request with the same last epoch call time to the
    // queue. When countdown reaches 0, don't add any more calls to the queue!

    // Obviously, this only works if the "poll()" function was implemented the
    // way it was. I.e. it should only execute the commands it has at the time of
    // calling it, and ignore the other commands that were added during polling.
    CmdInt::CommandArgument
    countdown_helper(std::vector<CmdInt::CommandArgument> const& arguments)
    {
        if (arguments.size() != 2)
        {
            return std::string{"countdown helper: invalid argument"};
        }
        
        auto const last_call = CmdInt::convert_argument<int>(arguments[0]);
        if (!last_call)
        {
            return std::string{"countdown helper: invalid argument"};
        }

        auto const starting_count = CmdInt::convert_argument<int>(arguments[1]);
        if (!starting_count)
        {
            return std::string{"countdown helper: invalid argument"};
        }

        using namespace std::chrono;
        int now = static_cast<int>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

        if ((now - *last_call) > 1000)
        {
            if (*starting_count > 0)
            {
                std::cout << std::to_string(*starting_count) + " seconds remaining\n";
                std::string const command_str = "countdown_helper " + std::to_string(now) + " " + std::to_string(*starting_count - 1);
                CmdInt_QueueCommand(command_str);
                return std::string{"countdown helper: continuing"};
            }

            std::cout << "Countdown complete\n";
            return std::string{"countdown helper: completed"};
        }

        std::string const command_str = "countdown_helper " + std::to_string(*last_call) + " " + std::to_string(*starting_count);
        CmdInt_QueueCommand(command_str);
        return std::string{"countdown: starting countdown"};
    }

} // namespace

CmdInt::CommandArgument
CmdInt::add_handler(std::vector<CmdInt::CommandArgument> const& arguments)
{
    int sum = 0;
    for (auto const& argument : arguments)
    {
        auto const str_val = CmdInt::convert_argument<std::string>(argument).value_or("0");
        auto const maybe_val = CmdInt::TypeConverter<ArgumentType::INT>::convert(str_val);
        if (!maybe_val)
        {
            return 0;
        }
        sum += CmdInt::convert_argument<int>(*maybe_val).value_or(0);
    }

    return sum;
}

CmdInt::CommandArgument
CmdInt::echo_handler(std::vector<CmdInt::CommandArgument> const& arguments)
{
    std::ostringstream output;
    for (auto const& argument : arguments)
    {
        auto const maybe_val = CmdInt::convert_argument<std::string>(argument);
        if (!maybe_val)
        {
            return std::string{"echo: invalid argument"};
        }
        output << *maybe_val << " ";
    }
    
    return output.str();
}

CmdInt::CommandArgument
CmdInt::countdown_handler(std::vector<CmdInt::CommandArgument> const& arguments)
{
    if (arguments.size() != 1)
    {
        return std::string{"countdown: invalid argument"};
    }
    
    // Doesn't hurt to attempt to register it all the time
    CommandFunction helper{countdown_helper,
      std::make_optional<ArgumentTypes>({ArgumentType::INT, ArgumentType::INT})};
    CmdInt_RegisterCommand("countdown_helper", helper);

    using namespace std::chrono;
    int now = static_cast<int>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

    auto const starting_count = CmdInt::convert_argument<int>(arguments[0]);
    if (!starting_count)
    {
        return std::string{"countdown: invalid argument"};
    }

    if ((*starting_count < 0) || (*starting_count > 10))
    {
        // TODO : not sure if you guys want me to also
        // log this?? Anyway, returning is easier for
        // testing
        return std::string{"countdown: invalid argument"};
    }

    std::cout << std::to_string(*starting_count) << " seconds remaining\n";
    std::string const command_string = "countdown_helper " + std::to_string(now) + " " + std::to_string(*starting_count - 1);
    CmdInt_QueueCommand(command_string);
    return std::string{"countdown: starting countdown"};
}

CmdInt::CommandFunction CmdInt::make_add_command()
{
    CommandFunction add_function{add_handler, std::make_optional<ArgumentTypes>({ArgumentType::INT, ArgumentType::INT})};
    return add_function;
}

CmdInt::CommandFunction CmdInt::make_echo_command()
{
    //! no optional argument types -> function is variadic
    //! and user is more responsible for checking values!
    CommandFunction echo_function{echo_handler, std::nullopt};
    return echo_function;
}

CmdInt::CommandFunction CmdInt::make_countdown_command()
{
    CommandFunction countdown_function{countdown_handler, std::make_optional<ArgumentTypes>({ArgumentType::INT})};
    return countdown_function;
}
