#include <cstdio>
#include "format.h"
#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    cout << "$ " << format("Hello, my name is %s, I am %.*f years old", "John", 6, sqrt(182))<< endl;
}
