#include <iostream>

class Pos{
    friend Pos operator+(Pos& p1, Pos& p2);
    friend std::ostream& operator<<(std::ostream& cout, Pos& p);
public:
    Pos(){}
    Pos(int x, int y): x(x), y(y){}
private:
    int x, y;
};

Pos operator+(Pos& p1, Pos& p2){
    Pos p;
    p.x = p1.x+p2.x;
    p.y = p1.y+p2.y;
    return p;
}

std::ostream& operator<<(std::ostream& cout, Pos& p){
    return std::cout << p.x << ", " << p.y;
}

int main()
{
    Pos p1(1,1), p2(2,2);
    Pos p3 = p1+p2;
    std::cout << p3 << std::endl;
    return 0;
}

