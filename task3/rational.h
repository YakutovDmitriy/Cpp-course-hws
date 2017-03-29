#pragma once

struct rational
{
    rational(int);
    rational(int, int);
    
    int getNum() const;
    int getDenom() const;
    
    rational &operator+=(rational const&);
    rational &operator-=(rational const&);
    rational &operator*=(rational const&);
    rational &operator/=(rational const&);

    friend rational operator+(rational, rational const&);
    friend rational operator-(rational, rational const&);
    friend rational operator*(rational, rational const&);
    friend rational operator/(rational, rational const&);

private:
    int num;
    int den;
    
    void update();
};
