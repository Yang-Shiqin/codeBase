#include <iostream>

class Pos{
public:
    int x, y;
    Pos(){}
    Pos(int x, int y): x(x), y(y){}
};

Pos operator+(Pos& p1, Pos& p2){
    Pos p;
    p.x = p1.x+p2.x;
    p.y = p1.y+p2.y;
    return p;
}

void operator<<(std::ostream& cout, Pos& p){
    std::cout << p.x << ", " << p.y << std::endl;
}

int main()
{
    Pos p1(1,1), p2(2,2);
    Pos p3 = p1+p2;
    std::cout << p3;
    return 0;
}

