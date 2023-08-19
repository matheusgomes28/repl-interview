#ifndef COMMAND_TYPES_H
#define COMMAND_TYPES_H

#include <CommandInterpreter/Control.h>


#include <charconv>
#include <optional>
#include <string_view>

namespace repl
{
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
            if (err.ptr != (str.data() + str.size()))
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

    template <>
    struct TypeConverter<ArgumentType::FLOAT>
    {
        static std::optional<CommandArgument> convert(std::string_view const& str)
        {
            //! I wanted to use <charconv> but I came to the crushing
            //! realisation that gcc has not yet implemented std::from_chars
            //! for floating point types (at least gcc9, the version in ubuntu)
            char* endptr = nullptr;
            std::string null_terminated_str{begin(str), end(str)};
            auto const result = std::strtof(null_terminated_str.c_str(), &endptr);

            // Whole string wasn't parsed
            if (*endptr != '\0')
            {
                return std::nullopt;
            }

            return result;
        }
    };
    // The following pattern improves the stl's visitor pattern
    // for the variants. More specifically, it lets tou add
    // labdas to convert values with the "overloaded" template.
    // For more information, you can read more on this technique
    // by visiting the post where I copied it from:
    // https://www.modernescpp.com/index.php/visiting-a-std-variant-with-the-overload-pattern
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
    template <typename T>
    std::optional<T> convert_argument(CommandArgument const& argument)
    {
        return std::visit(overloaded{
            [](T const& arg) -> std::optional<T> { return arg; },
            [](auto const&) -> std::optional<T> { return std::nullopt; }
        }, argument);
    }
} // namespace CmdInt

#endif // COMMAND_TYPES_H
