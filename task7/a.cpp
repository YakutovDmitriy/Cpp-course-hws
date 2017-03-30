#include "lazy_string.h"
#include <iostream>

using namespace std;

int main()
{
    lazy_string a(std::string("abacaba"));
    lazy_string b = a.substr(1);
    lazy_string c = b.substr(1);
    cout << a << ' ' << b << ' ' << c << '\n';
}
