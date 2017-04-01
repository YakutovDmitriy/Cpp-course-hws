#include "format.h"

std::string make_simple_fm(format_t fm)
{
    std::string temp = "%";
    if(fm.force_sign)
        temp += '+';
    if(fm.left_justify)
        temp += '-';
    if(fm.space_or_sign)
        temp += ' ';
    if(fm.alt_num_format)
        temp += '#';
    if(fm.left_pad)
        temp += '0';
    return temp;
}    

int to_int(std::string s)
{
    std::stringstream ss(s);
    int ret;
    ss >> ret;
    return ret;
}

std::string to_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

std::string find_spec(std::string const &fmt, size_t &pos, bool have_args)
{
    std::string ret = "";
    while (pos < fmt.length())
    {
        while (pos < fmt.size() && fmt[pos] != '%')
            ret += fmt[pos++];
        if (pos == fmt.length())
        {
            if (have_args)
                throw std::invalid_argument("Too many arguments for format");
            return ret;
        }
        if (pos == fmt.length() - 1)
            throw std::invalid_argument("Spurious trailing '%%' in format");
        if(fmt[pos + 1] == '%')
        {
            ret += '%';
            pos += 2;
        }
        else
        {
            ++pos;
            if (!have_args)
                throw std::out_of_range("Need more arguments");
            break;
        }
    }
    return ret;
}

std::string format_impl(std::string const &fmt, size_t pos, size_t)
{
    return find_spec(fmt, pos, false);
}
