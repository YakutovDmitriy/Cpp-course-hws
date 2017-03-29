#include "lazy_string.h"
#include <exception>
#include <cstring>

lazy_string::lazy_string(std::string s)
{
    char *cur = new char[s.size() + 1];
    memcpy(cur, s.data(), s.size());
    cur[s.size()] = 0;
    data = std::shared_ptr<char>(cur);
    offset = 0;
    len = s.size();
}

lazy_string::operator std::string() const
{
    return std::string(data.get() + offset, data.get() + offset + len);
}

size_t lazy_string::size() const
{
    return len;
}

size_t lazy_string::length() const
{
    return len;
}

char lazy_string::at(size_t pos) const
{
    if (pos >= len)
        throw std::range_error("wtf");
    return data.get()[offset + pos];
}

char &lazy_string::at(size_t pos)
{
    if (pos >= len)
        throw std::range_error("wtf");
    make_unique_copy();
    return data.get()[offset + pos];
}

char lazy_string::operator[](size_t pos) const
{
    return at(pos);
}

char &lazy_string::operator[](size_t pos)
{
    return at(pos);
}

std::istream &operator>>(std::istream &is, lazy_string &s)
{
    std::string data;
    is >> data;
    s = data;
    return is;
}

std::ostream &operator<<(std::ostream &os, lazy_string const &s)
{
    return os << std::string(s);
}

lazy_string lazy_string::substr(size_t offset) const
{
    if (offset >= len)
        throw std::range_error("wtf");
    return substr(offset, len - offset);
}

lazy_string lazy_string::substr(size_t offset, size_t length) const
{
    if (offset + length > len)
        throw std::range_error("wtf");
    return lazy_string(data, offset, length);
}

lazy_string::lazy_string(std::shared_ptr<char> data, size_t offset, size_t len)
    : data(data)
    , offset(offset)
    , len(len)
{}

void lazy_string::make_unique_copy()
{
    if (!data.unique())
    {
        char *cur = new char[len + 1];
        memcpy(cur, data.get() + offset, len);
        cur[len] = 0;
        data.reset(cur);
        offset = 0;
    }
}
