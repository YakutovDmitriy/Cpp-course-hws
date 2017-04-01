#include <cstdio>
#include "format.h"
#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    cout << "$ " << format("Hello, my name is %s, I am %.*f years old", "John", 6, sqrt(182))<< endl;
    cout << "$ " << format("nullptr: %@", nullptr_t()) << endl;
    cout << "$ " << format("int null: %@", (int*)0) << endl;
    cout << "$ " << format("int not null: %@", new int(5)) << endl;
    int a[3] = {1, 2, 3};
    cout << "$ " << format("array: %@", a) << endl;
    char const *s = "abacaba";
    cout << "$ " << format("char[] to string: %@", s) << endl;
    //cout << "'" << format("exception: %@", 123.4) << endl;
}
