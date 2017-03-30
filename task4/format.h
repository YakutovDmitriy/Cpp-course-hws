#pragma once

#include <string>
#include <stdexcept>
#include <sstream>
#include <typeinfo>

template<class T>
static std::string get_char()
{
    std::string msg = std::string("unknown type: ") + std::string(typeid(T).name());
    throw std::invalid_argument(msg);
}

template<class T>
std::string get_char(T const t)
{
    return get_char(const_cast<T>(t));
}

template<> std::string get_char<int>() { return "d"; }
template<> std::string get_char<long long>() { return "lld"; }
template<> std::string get_char<char>() { return "c"; }
template<> std::string get_char<char const*>() { return "s"; }
template<> std::string get_char<double>() { return "f"; }

static size_t sh_find(std::string const& f, size_t from, char x)
{
    bool sh = false;
    for (size_t pos = 0; pos != f.size(); ++pos)
    {
        if (sh)
            sh = false;
        else if (f[pos] == '\\')
            sh = true;
        else if (pos >= from && f[pos] == x)
            return pos;
    }
    return std::string::npos;
}

template<class... Args>
std::string format(std::string const &f, Args... args);

template<>
std::string format(std::string const &f)
{
    auto pos = sh_find(f, 0, '%');
    if (pos != std::string::npos)
        throw std::out_of_range("format does not match with arguments");
    return f;
}

template<class First, class... Args>
std::string format(std::string const& f, First s, Args... args)
{
    std::string need = "%" + get_char<First>();
    auto pos = sh_find(f, 0, '%');
    if (pos == std::string::npos || pos + need.size() > f.size() || f.substr(pos, need.size()) != need)
        throw std::invalid_argument(std::string("expected ") + need + std::string(" for type ") + std::string(typeid(First).name()));
    std::stringstream ss;
    ss << s;
    return f.substr(0, pos) + ss.str() + format(std::string(f.substr(pos + 2)), args...);
}
