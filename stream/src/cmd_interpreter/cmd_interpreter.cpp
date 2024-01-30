module;

#include <functional>
#include <map>
#include <queue>
#include <string>
#include <string_view>
#include <ranges>
#include <vector>

export module cmd_interpreter;

export struct Command {
    std::string name;
    std::vector<std::string> arguments;
};

export using CommandHandler = std::function<std::string(std::vector<std::string> const&)>;
export using CommandQueue = std::queue<Command>;
export using CommandMap = std::map<std::string, CommandHandler>;


export class CommandInterpreter {

public:
    // queue a command (in command)
    auto queue_command(std::string_view const& command) -> bool {
        // // TODO : Make this work with views!
        std::vector<std::string> words;
        for (auto const& view : command | std::views::split(' ')) {
            words.emplace_back(view.data(), view.size());
        }

        if (words.size() < 1) {
            // we have invalid command
            return false;
        }

        std::string const name = words[0];
        auto command_found = _commands.find(name);
        if (command_found == end(_commands)) {
            return false;
        }

        // Lazy way to get remaining arguments (views::drop)
        std::vector<std::string> const args{std::next(begin(words)), end(words)};
        _command_queue.emplace(Command{name, args});
        return true;
    }

    // process commands in queue
    void process_commands()
    {
        // pop the first command
        auto const num_commands = _command_queue.size();
        for (int i = 0; i < num_commands; ++i) {
            auto command = std::move(_command_queue.front());
            _command_queue.pop();

            // Guaranteed to have the handler
            auto const handler = _commands[command.name];
            auto const result = handler(command.arguments);
            // callback(result)
        }
    }

    // add command (in name, in func handler, in func callback)
    bool add_command(std::string const& name, CommandHandler handler /*, CommandCallback callback*/)
    {
        if (name.empty())
        {
            return false;
        }

        if (name.find_first_of(' ') != std::string::npos) {
            return false;
        }

        auto const found = _commands.find(name);
        if (found != end(_commands)) {
            // already in command map
            return false;
        }

        _commands.insert({name, handler});
        return true;
    }
private:
    CommandQueue _command_queue;
    CommandMap _commands;
};

