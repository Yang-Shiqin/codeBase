#include <iostream>
#include <string>

int main() {
    // 初始化字符串
    std::string str1 = "Hello, ";
    std::string str2 = "world!";

    // 字符串连接
    std::string str3 = str1 + str2;
    std::cout << "Concatenated string: " << str3 << std::endl;

    // 字符串长度
    std::cout << "Length of str3: " << str3.length() << std::endl;

    // 字符串访问
    std::cout << "First character of str3: " << str3[0] << std::endl;
    std::cout << "Last character of str3: " << str3[str3.length() - 1] << std::endl;

    // 字符串比较
    if (str1 == "Hello, ") {
        std::cout << "str1 is equal to \"Hello, \"" << std::endl;
    } else {
        std::cout << "str1 is not equal to \"Hello, \"" << std::endl;
    }

    // 查找子字符串
    std::size_t pos = str3.find("world");
    if (pos != std::string::npos) {
        std::cout << "\"world\" found at position: " << pos << std::endl;
    } else {
        std::cout << "\"world\" not found" << std::endl;
    }

    // 子字符串提取
    std::string subStr = str3.substr(7, 5);
    std::cout << "Substring: " << subStr << std::endl;

    // 字符串插入
    str3.insert(7, "beautiful ");
    std::cout << "After insertion: " << str3 << std::endl;

    // 字符串删除
    str3.erase(7, 10);
    std::cout << "After erasure: " << str3 << std::endl;

    // 字符串替换
    str3.replace(7, 5, "everyone");
    std::cout << "After replacement: " << str3 << std::endl;

    return 0;
}

