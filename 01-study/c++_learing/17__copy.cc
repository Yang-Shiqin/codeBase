#include <iostream>
using namespace std;

#define PI (3.1415926)

// 类
class Circle{
// 权限
public:
    // 成员函数
    // 构造函数
    Circle(){
        this->r = new double(1);
    }
    Circle(double R){
        this->r = new double(R);
    }
    // 重载拷贝函数，以支持深拷贝
    Circle(const Circle& c){
        this->r = new double(*c.r);
    }
    // 析构函数
    ~Circle(){
        // 若浅拷贝赋值，则另一对象也指向这个堆分配的r，
        // 某一对象析构后其他对象析构就会多次delete r造成double free错误
        cout << "x" << endl;
        if (this->r){
            delete this->r;
            this->r = NULL;
        }
    }
    // 重载赋值函数，以支持深拷贝
    Circle& operator=(const Circle& c){
        if(this->r){    // 因为是已建立的obj
            delete this->r;
            this->r = NULL;
        }
        this->r = new double(*c.r);
        return *this;
    }
    double cal_area(){
        return PI*(*r)*(*r);
    }
    double get_r(){
        return *r;
    }
    void set_r(double R){
        *(this->r) = R;
    }
private:
    // 成员变量
    double *r;
};

int main()
{
    // 对象，实例化
    Circle c;
    Circle c1(c);       // 调用拷贝构造，拷贝构造没重载则double free
    Circle c2;
    c2 = c;             // 调用赋值操作符函数，赋值没重载则double free
    cout << "the area of c is " << c.cal_area() << endl;
    return 0;
}

