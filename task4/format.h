#pragma once

#include <string>
#include <stdexcept>

template<class... Args>
std::string format(std::string const &f, Args... args);
