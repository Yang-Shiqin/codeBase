#include <iostream>

class P{
public:
    P(){}
    P(int age):age(age){}
    int get_age() const{
        return this->age;
    }
    int get_like() const{
        return this->like;
    }
    void set_likes(int a) const{
        this->likes = a;
    }
    int get_likes() const{
        return this->likes;
    }
    int get_num(){
        return this->num;
    }

private:
    int age;
    static int num;
    const int like=10;
    mutable int likes=20;
};

int P::num = 0;

int main()
{
    P p(10);
    std::cout << p.get_age() << std::endl;
    std::cout << p.get_num() << std::endl;
    std::cout << p.get_like() << std::endl;
    p.set_likes(3);
    std::cout << p.get_likes() << std::endl;
    const P p2(20);
    std::cout << p2.get_age() << std::endl;

    return 0;
}

