#pragma once
#include <string>
#include <istream>
#include <ostream>
#include <memory>

struct lazy_string
{
    struct reference
    {
        friend struct lazy_string;
    
        reference &operator=(char);
        operator char() const;
    
    private:
        lazy_string *outer;
        size_t index;
    
        reference(lazy_string*, size_t);
    };

    lazy_string(std::string);
    operator std::string() const;

    size_t size() const;
    size_t length() const;
    
    char at(size_t) const;
    reference at(size_t);
    char operator[](size_t) const;
    reference operator[](size_t);
    
    friend std::istream &operator>>(std::istream&, lazy_string&);
    friend std::ostream &operator<<(std::ostream&, lazy_string const&);
    
    lazy_string substr(size_t) const;
    lazy_string substr(size_t, size_t) const;
    
private:
    std::shared_ptr<char> data;
    size_t offset;
    size_t len;
    
    lazy_string(std::shared_ptr<char>, size_t, size_t);
    void make_unique_copy();
};
