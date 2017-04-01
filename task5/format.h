#ifndef _FORMAT_H_
#define _FORMAT_H_
#include <bits/stdc++.h>
 
template<class... Args>
std::string format(std::string const &fmt, Args const&... args);

enum length_t
{
    hh, h, def, l, ll, j, z, t, L, error
};

struct format_t
{
    bool force_sign = false;
    bool left_justify = false;
    bool space_or_sign = false;
    bool alt_num_format = false;
    bool left_pad = false;
    bool uppercase = false;
    bool floating = false;
    int width = 0;
    int precision = -1;
    char type;
    length_t length = length_t::def;
};

template<class To, class From>
typename std::enable_if<std::is_convertible<From, To>::value, To>::type cast_to(From value)
{
    return static_cast<To>(value);
}

template<class To, class From>
typename std::enable_if<!std::is_convertible<From, To>::value, To>::type cast_to(From)
{
    throw std::invalid_argument("Invalid argument type");
}

std::string make_simple_fm(format_t);
int to_int(std::string);
std::string to_string(int);
std::string find_spec(std::string const&, size_t&, bool);
std::string format_impl(std::string const&, size_t, size_t);
std::string print_at(nullptr_t value);

template<class T>
typename std::enable_if<!std::is_integral<T>::value && !std::is_convertible<T, std::string>::value && !std::is_pointer<T>::value, std::string>::type
        print_at(T const&)
{
    throw std::invalid_argument("Invalid argument type");
}

template<class T>
typename std::enable_if<std::is_integral<T>::value, std::string>::type
        print_at(T value)
{
    return to_string(value);
}

template<class T, size_t size>
typename std::enable_if<!std::is_convertible<T*, std::string>::value, std::string>::type
        print_at(T const (&a)[size])
{
    std::string ret = "[";
    for (size_t i = 0; i != size; ++i)
    {
        if (i)
            ret += ",";
        ret += to_string(a[i]);
    }
    ret += ']';
    return ret;
}

template<class T>
typename std::enable_if<std::is_convertible<T, std::string>::value, std::string>::type
        print_at(T const &value)
{
    return value;
}
    
template<class T>
typename std::enable_if<!std::is_array<T>::value && !std::is_convertible<T, std::string>::value && std::is_pointer<T>::value, std::string>::type
        print_at(T& value)
{
    std::string name(typeid(*value).name());
    return value == 0 ? "nullptr<" + name + ">" : "ptr<" + name + ">(" + format("%@", *value) + ")";
}

template<class T>
typename std::enable_if<std::is_arithmetic<T>::value, std::string>::type
        print_num(format_t fm, T value)
{
    if (!fm.floating)
    {
        if (fm.precision < 0)
            fm.precision = 1;
        else if (fm.left_pad)
            fm.left_pad = false;
    }

    std::string cur = make_simple_fm(fm);
    if (fm.precision >= 0)
        cur += "." + to_string(std::min(fm.precision, 1 << 10));
    
    if (fm.floating)
    {
        if (fm.length == length_t::L)
            cur += 'L';
        if (fm.length == length_t::l)
            cur += 'l';
        cur += fm.type;
    }
    else
    { 
        cur += 'j';
        cur += fm.type;
    }
    static int const bsize = 1 << 10;
    static char buffer[(bsize << 1) | 1];
    snprintf(buffer, sizeof(buffer), cur.c_str(), value);

    std::string ret = buffer;
    if (fm.precision > bsize && ret.size() > (bsize >> 1))
    {
        if (fm.floating)
            ret += std::string(fm.precision - ret.size() + ret.find_first_of('.') + 1, '0');
        else
            ret = ret.substr(0, 2) + std::string(fm.precision - ret.size() + (ret[0] == '0' ? 0 : 1), '0') + ret.substr(2);
    }

    if ((size_t) fm.width > ret.size())
    {
        if (fm.left_justify)
        {
            ret += std::string(fm.width - ret.size(), ' ');
        }
        else
        {
            if (fm.left_pad)
                ret = (ret.find_first_of("+- ") == 0) ?
                        ret[0] + std::string(fm.width - ret.size(), '0') + ret.substr(1) : std::string(fm.width - ret.size(), '0') + ret;
            else
                ret = std::string(fm.width - ret.size(), ' ') + ret;
        }
    }

    return ret;
}

template<class First, class... Args>
std::string format_impl(std::string const &fmt, size_t pos, size_t length_before, First const &value, Args const&... args)
{
    std::string ret = find_spec(fmt, pos, true);
    format_t fm;
    std::string cur = "";

    while (pos < fmt.length() && (fmt[pos] == '-' || fmt[pos] == '+' || fmt[pos] == ' ' || fmt[pos] == '#' || fmt[pos] == '0'))
    {
        switch(fmt[pos++])
        {
        case '-':
            fm.left_justify = true;
            fm.left_pad = false;
            break;
        case '+':
            fm.force_sign = true;
            fm.space_or_sign = false;
            break;
        case ' ':
            fm.space_or_sign = !fm.force_sign;
            break;
        case '#':
            fm.alt_num_format = true;
            break;
        case '0':
            fm.left_pad = !fm.left_justify;
            break;
        }
    }

    if (pos < fmt.length() && fmt[pos] == '*')
    {
        fm.width = cast_to<int>(value);
        if (fm.width < 0)
        {
            fm.width *= -1;
            fm.left_justify = true;
            fm.left_pad = false;
        }
        cur = make_simple_fm(fm);
        cur += to_string(fm.width);
        return ret + format_impl(cur + fmt.substr(pos + 1), 0, length_before + ret.length(), args...);
    }
    else
    {
        while (pos < fmt.size() && isdigit(fmt[pos]))
            cur += fmt[pos++];
        if (!cur.empty())
        {
            fm.width = to_int(cur);
            cur.clear();
        }
    }

    if (pos < fmt.length() - 1 && fmt[pos] == '.')
    {
        ++pos;
        if (fmt[pos] == '*')
        {
            fm.precision = cast_to<int>(value);
            cur = make_simple_fm(fm);
            if (fm.width != 0)
                cur += to_string(fm.width);
            cur += '.';
            cur += to_string(fm.precision);
            return ret + format_impl(cur + fmt.substr(pos + 1), 0, length_before + ret.length(), args...);
        }
        else
        {
            if (fmt[pos] == '-')
            {
                fm.precision = -1;
                ++pos;
            }
            else
                fm.precision = 1;
            while (pos < fmt.size() && isdigit(fmt[pos]))
                cur += fmt[pos++];
            if (!cur.empty())
            {
                fm.precision *= to_int(cur);
                cur.clear();
            }
            else
                fm.precision = 0;
        }
    }

    while (pos < fmt.size() && (fmt[pos] == 'h' || fmt[pos] == 'l' || fmt[pos] == 'j' || fmt[pos] == 'z' || fmt[pos] == 't' || fmt[pos] == 'L'))
    {
        switch(fmt[pos++])
        {
        case 'h':
            fm.length = fm.length == length_t::h ? length_t::hh : fm.length == length_t::def ? length_t::h : length_t::error;
            break;
        case 'l':
            fm.length = fm.length == length_t::l ? length_t::ll : fm.length == length_t::def ? length_t::l : length_t::error;
            break;
        case 'j':
            fm.length = fm.length == length_t::def ? length_t::j : length_t::error;
            break;
        case 'z':
            fm.length = fm.length == length_t::def ? length_t::z : length_t::error;
            break;
        case 't':
            fm.length = fm.length == length_t::def ? length_t::t : length_t::error;
            break;
        case 'L':
            fm.length = fm.length == length_t::def ? length_t::L : length_t::error;
            break;
        }
    }

    if (fm.length == length_t::error)
        throw std::invalid_argument("Unknown length specifier");

    if (pos == fmt.length())
        throw std::invalid_argument("Ñonversion lacks type at end of format");

    std::stringstream out;
    if (fm.force_sign)
        out << std::showpos;
    if (fm.left_justify)
        out << std::left;
    if (fm.width != 0)
        out.width(fm.width);
    if (fm.precision >= 0)
        out.precision(fm.precision);
    if (fm.alt_num_format)
        out << std::showbase << std::showpoint;
    
    intmax_t d;      // Integer
    uintmax_t u;     // Unsigned
    double f;        // Floating point
    char nil_p[6];   // Null pointer
    
    fm.type = fmt[pos++];
    switch(fm.type)
    {
    case 'd':
    case 'i':
        switch (fm.length)
        {
        case length_t::hh:
            d = cast_to<signed char>(value);
            break;
        case length_t::h:
            d = cast_to<short int>(value);
            break;
        case length_t::l:
            d = cast_to<long int>(value);
            break;
        case length_t::ll:
            d = cast_to<long long int>(value);
            break;
        case length_t::j:
            d = cast_to<intmax_t>(value);
            break;
        case length_t::z:
            d = cast_to<size_t>(value);
            break;
        case length_t::t:
            d = cast_to<ptrdiff_t>(value);
            break;
        case length_t::def:
            d = cast_to<int>(value);
            break;
        default:
            throw std::invalid_argument("Unsupported length specifier");
        }
        ret += print_num(fm, d);
        break;
    case 'X':
        fm.uppercase = true;
    case 'x':
    case 'o':
    case 'u':
        switch (fm.length){
        case length_t::hh:
            u = cast_to<unsigned char>(value);
            break;
        case length_t::h:
            u = cast_to<unsigned short int>(value);
            break;
        case length_t::l:
            u = cast_to<unsigned long int>(value);
            break;
        case length_t::ll:
            u = cast_to<unsigned long long int>(value);
            break;
        case length_t::j:
            u = cast_to<uintmax_t>(value);
            break;
        case length_t::z:
            u = cast_to<size_t>(value);
            break;
        case length_t::t:
            u = cast_to<ptrdiff_t>(value);
            break;
        case length_t::def:
            u = cast_to<unsigned int>(value);
            break;
        default:
            throw std::invalid_argument("Unsupported length specifier");
        }
        ret += print_num(fm, u);
        break;
    case 'E':
    case 'G':
    case 'A':
        fm.uppercase = true;
    case 'e':
    case 'g':
    case 'a':
    case 'F':
    case 'f':
        fm.floating = true;
        switch (fm.length)
        {
        case length_t::l:
        case length_t::def:
            f = cast_to<double>(value);
            break;
        case length_t::L:
            f = cast_to<long double>(value);
            break;
        default:
            throw std::invalid_argument("Unsupported length specifier");
        }
        ret += print_num(fm, f);
        break;
    case 'c':
        switch (fm.length)
        {
        case length_t::l:
            break;
        case length_t::def:
            out << cast_to<unsigned char>(value);
            break;
        default:
            throw std::invalid_argument("Unsupported length specifier");
        } 
        ret += out.str();
        break;
    case 's': {
        std::string str;
        switch (fm.length)
        {
        case length_t::l:
            break;
        case length_t::def:
            str = cast_to<std::string>(value);
            break;
        default:
            throw std::invalid_argument("Unsupported length specifier");
        }
        if (fm.precision >= 0 && str.length() > (size_t) fm.precision)
            str = str.substr(0, fm.precision);
        out << str;
        ret += out.str();
        }
        break;
    case 'p':
        if (fm.length != length_t::def)
            throw std::invalid_argument("Unsupported length specifier");
        out << std::setfill(fm.left_pad ? '0' : ' ');
        snprintf(nil_p, 2, "%p", cast_to<void*>(value));
        if (nil_p[0] != '(' && cast_to<void*>(value) != NULL && cast_to<void*>(value) != nullptr)
            out << cast_to<void*>(value);
        else
            out << "(nil)";
        ret += out.str();
        break;
    case 'n':
        length_before += ret.length();
        switch (fm.length)
        {
        case length_t::hh:
            *(cast_to<signed char*>(value)) = length_before;
            break;
        case length_t::h:
            *(cast_to<short int*>(value)) = length_before;
            break;
        case length_t::l:
            *(cast_to<long int*>(value)) = length_before;
            break;
        case length_t::ll:
            *(cast_to<long long int*>(value)) = length_before;
            break;
        case length_t::j:
            *(cast_to<intmax_t*>(value)) = length_before;
            break;
        case length_t::z:
            *(cast_to<size_t*>(value)) = length_before;
            break;
        case length_t::t:
            *(cast_to<ptrdiff_t*>(value)) = length_before;
            break;
        case length_t::def:
            *(cast_to<int*>(value)) = length_before;
            break;
        default:
            throw std::invalid_argument("Unsupported length specifier");
        }
        break;
    case '@':
        ret += print_at(value);
        break;
    default:
        throw std::invalid_argument("Unknown format specifier: '" + fmt[pos] + std::string("'"));
        break;
    }
    ret += format_impl(fmt, pos, length_before + ret.length(), args...);
    return ret;
}

template<class... Args>
std::string format(std::string const &fmt, Args const&... args)
{
    return format_impl(fmt, 0, 0, args...);
}

#endif
