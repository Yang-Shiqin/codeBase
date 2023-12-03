#include <iostream>
using namespace std;

#define PI (3.1415926)

// 类
struct Circle{
// 权限
public:
    // 成员函数
    double cal_area(){
        return PI*r*r;
    }
    double get_r(){
        return r;
    }
    void set_r(double R){
        r = R;
    }
private:
    // 成员变量
    double r;
};

int main()
{
    // 对象，实例化
    Circle c;
    c.set_r(10);
    cout << "the area of c is " << c.cal_area() << endl;
    return 0;
}

