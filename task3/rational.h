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

private:
    int num;
    int den;
    
    void update();
};

rational operator+(rational, rational const&);
rational operator-(rational, rational const&);
rational operator*(rational, rational const&);
rational operator/(rational, rational const&);
