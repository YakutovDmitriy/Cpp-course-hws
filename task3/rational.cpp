#include "rational.h"

static int gcd(int a, int b)
{
    a = a > 0 ? a : -a;
    b = b > 0 ? b : -b;
    while (b != 0)
    {
        int temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

void rational::update()
{
    if (den < 0)
    {
        num *= -1;
        den *= -1;
    }
    int g = gcd(num, den);
    num /= g;
    den /= g;
}

rational::rational(int num)
    : num(num)
    , den(1)
{}

rational::rational(int _num, int _den)
    : num(_num)
    , den(_den)
{
    update();
}

int rational::getNum() const
{
    return num;
}

int rational::getDenom() const
{
    return den;
}

rational &rational::operator+=(rational const& rat)
{
    num = num * rat.den + rat.num * den;
    den *= rat.den;
    update();
    return *this;
}

rational &rational::operator-=(rational const& rat)
{
    num = num * rat.den - rat.num * den;
    den *= rat.den;
    update();
    return *this;
}

rational &rational::operator*=(rational const& rat)
{
    num *= rat.num;
    den *= rat.den;
    update();
    return *this;
}

rational &rational::operator/=(rational const& rat)
{
    num *= rat.den;
    den *= rat.num;
    update();
    return *this;
}

rational operator+(rational a, rational const &b)
{
    return a += b;
}

rational operator-(rational a, rational const &b)
{
    return a -= b;
}

rational operator*(rational a, rational const &b)
{
    return a *= b;
}

rational operator/(rational a, rational const &b)
{
    return a /= b;
}
