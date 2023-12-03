#include <iostream>

class Pos{
    friend Pos operator+(Pos& p1, Pos& p2);
    friend std::ostream& operator<<(std::ostream& cout, const Pos& p);
    friend Pos& operator++(Pos& p);
    friend Pos operator++(Pos& p, int);
public:
    Pos(){}
    Pos(int x, int y): x(x), y(y){}
    ~Pos(){std::cout<< "x" << std::endl;}
private:
    int x, y;
};

Pos operator+(Pos& p1, Pos& p2){
    Pos p;
    p.x = p1.x+p2.x;
    p.y = p1.y+p2.y;
    return p;
}

// Pos可以不用引用(因为cout没实际操作)，用引用应该是为了节省空间
// 但这里加引用就得加const，因为cout是常量对象
// 其他有对对象实际操作还是得加引用。
std::ostream& operator<<(std::ostream& cout, const Pos& p){
    return std::cout << p.x << ", " << p.y;
}

Pos& operator++(Pos& p){
    p.x++;
    return p;
}

Pos operator++(Pos& p, int){
    Pos tmp=p;  // 这是调用默认的拷贝构造，浅拷贝
    p.x++;
    return tmp;
}

int main()
{
    Pos p1(1,1), p2(2,2);
    Pos p3 = p1+p2;
    std::cout << p3++ << std::endl;
    std::cout << p3++ << ", " << p3 << ", " << ++(++p3) << std::endl;
    return 0;
}

