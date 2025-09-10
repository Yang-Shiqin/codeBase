#include <cstddef>
#include <iostream>
#include <cstring>
#include <algorithm>

class String{
public:
    String(): m_data(new char[1]{'\0'}), size_(0){}
    String(const char *s);
    String(const String &other);
    String(String&& other) noexcept;
    ~ String(void);
    String & operator=(const String &other);
    String & operator=(String&& other) noexcept;
    // 下标访问
    char& operator[](size_t i) { return m_data[i]; }
    const char& operator[](size_t i) const { return m_data[i]; }
    // c_str()，返回以 '\0' 结尾的字符串
    const char* c_str() const { return m_data; }
    void reserve(size_t n);
    void push_back(char c);
    void append(const char* s);
    // 输出支持
    friend std::ostream& operator<<(std::ostream& os, const String& s) {
        return os << s.c_str();
    }
private:
    char *m_data;
    size_t size_;
    size_t capacity_;   // 容量，不含\0
    void ensure_null_terminated(){m_data[size_]='\0';}
};

// 有参构函
String::String(const char *s){
    size_ = strlen(s);
    capacity_= size_;
    m_data = new char[capacity_+1];
    memcpy(m_data, s, size_+1);
}

// 拷贝构函
String::String(const String &other){
    size_ = strlen(other.m_data);
    capacity_= size_;
    m_data = new char[capacity_+1];
    memcpy(m_data, other.m_data, size_+1);
}

// 移动构函
String::String(String&& other) noexcept{
    size_ = strlen(other.m_data);
    capacity_= size_;
    m_data = other.m_data;
    other.m_data = new char[1]{'\0'};
    other.size_ = 0;
    other.capacity_ = 0;
}

String::~String(void){
    delete [] m_data;
}

// 拷贝赋值
String & String::operator=(const String &other){
    if (this!=&other){
        size_ = other.size_;
        if (capacity_<size_){
            delete [] m_data;
            m_data = new char[size_+1];
            capacity_ = size_;
        }
        memcpy(m_data, other.m_data, size_+1);
    }
    return *this;
}

// 移动赋值
String & String::operator=(String&& other) noexcept{
    if (this!=&other){
        delete [] m_data;
        size_ = other.size_;
        capacity_ = size_;
        m_data = other.m_data;
        other.m_data = new char[1]{'\0'};
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

void String::reserve(size_t n){
    if (n>capacity_){
        char *new_data = new char[n+1];
        memcpy(new_data, m_data, size_+1);
        delete [] m_data;
        m_data = new_data;
        capacity_ = n;
    }
}

void String::push_back(char c){
    if (size_ + 1 > capacity_) {
        reserve(std::max(2 * capacity_, size_ + 1));
    }
    m_data[size_++] = c;
    ensure_null_terminated();
}

void String::append(const char* s){
    size_t len = std::strlen(s);
    if (size_ + len > capacity_) {
        reserve(std::max(2 * capacity_, size_ + len));
    }
    memcpy(m_data + size_, s, len);
    size_ += len;
    ensure_null_terminated();
}

int main()
{
    String s("sdfsdf");
    String s2(s);
    String s3 = s;
    std::cout << "Hello world" << std::endl;
    return 0;
}

