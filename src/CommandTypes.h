#ifndef COMMAND_TYPES_H
#define COMMAND_TYPES_H

#include <CommandInterpreter/Control.h>

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
#endif // COMMAND_TYPES_H
