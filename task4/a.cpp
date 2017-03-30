#include <cstdio>
#include "format.h"
#include <iostream>

using namespace std;

int main()
{
    auto res = format("Hello, my name is %s, I am %d years old", "John", 13);
    std::cout << "'" << res << "'" << std::endl;
}
