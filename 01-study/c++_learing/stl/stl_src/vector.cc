#include <iostream>
using namespace std;

template <typename T>
class Vector{
private:
    T* data_;
    size_t size_;
    size_t cap_;
public:
    Vector(): data_(nullptr), size_(0), cap_(0){}
    Vector(size_t n): data_(new T[n]), size_(n), cap_(n){}
    Vector(std::initializer_list<T> init): Vector(){
        reserve(init.size());
        for (const auto& item : init) {
            push_back(item);
        }
    }
    Vector(const Vector<T>& other): data_(new T[other.cap_]), size_(other.size_), cap_(other.cap_){
        copy(other.data_, other.data_+size_, data_);
    }
    Vector(Vector<T>&& other) noexcept: data_(other.data_), size_(other.size_), cap_(other.cap_){
        other.data_ = nullptr;
        other.size_ = 0;
        other.cap_ = 0;
    }
    ~Vector(){
        if (data_){
            delete [] data_;
        }
    }
    Vector<T>& operator=(const Vector<T>& other){
        if (&other!=this){
            clear();
            delete [] data_;
            size_ = other.size_;
            cap_ = other.cap_;
            data_ = new T[cap_];
            copy(other.data_, other.data_+size_, data_);
        }
        return *this;
    }
    Vector<T>& operator=(Vector<T>&& other) noexcept{
        if (&other!=this){
            clear();
            delete [] data_;
            data_ = other.data_;
            size_ = other.size_;
            cap_ = other.cap_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.cap_ = 0;
        }
        return *this;
    }
    size_t size(){return size_;}
    size_t cap(){return cap_;}
    void clear(){
        size_ = 0;
    }
    void push_back(const T& value){
        if (size_==cap_){
            size_t new_cap = cap_==0?1:2*cap_;
            reserve(new_cap);
        }
        new (&data_[size_]) T(value); // placement new
        ++size_;
    }
    void push_back(T&& value){
        if (size_==cap_){
            size_t new_cap = cap_==0?1:2*cap_;
            reserve(new_cap);
        }
        new (&data_[size_]) T(std::move(value)); // placement new
        ++size_;
    }
    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }
    T& operator[](size_t index) {
        return data_[index]; // 不检查越界（和 STL 一致）
    }
    void reserve(size_t new_cap){
        if (new_cap > cap_){
            T* new_data = new T[new_cap];
            for (size_t i=0; i<size_; i++){
                new (&new_data[i]) T(std::move(data_[i])); // placement new + move
            }
            delete [] data_;
            data_ = new_data;
            cap_ = new_cap;
        }
    }
};

int main()
{
    Vector<int> v = {1,2,3};
    v.push_back(4);
    v.pop_back();
    for (int i=0; i<v.size(); i++){
        cout << v[i] << endl;
    }
    return 0;
}

