#include <iostream>
#include <map>
#include <string>

int main() {
    // 初始化一个map
    std::map<std::string, int> myMap;

    // 插入元素
    myMap["apple"] = 3;
    myMap["banana"] = 2;
    myMap["orange"] = 5;

    // 插入元素的另一种方法
    myMap.insert(std::make_pair("grape", 4));

    // 迭代并打印元素
    std::cout << "Map elements:" << std::endl;
    for (const auto& pair : myMap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 查找元素
    auto it = myMap.find("banana");
    if (it != myMap.end()) {
        std::cout << "Found: banana -> " << it->second << std::endl;
    } else {
        std::cout << "banana not found in the map." << std::endl;
    }

    // 删除元素
    myMap.erase("orange");

    // 使用迭代器删除元素
    it = myMap.find("grape");
    if (it != myMap.end()) {
        myMap.erase(it);
    }

    // 迭代并打印删除后的元素
    std::cout << "Map elements after deletion:" << std::endl;
    for (const auto& pair : myMap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 检查元素是否存在
    if (myMap.count("apple")) {
        std::cout << "Element 'apple' is in the map." << std::endl;
    } else {
        std::cout << "'apple' is not in the map." << std::endl;
    }

    // 清空map
    myMap.clear();
    std::cout << "Map size after clear: " << myMap.size() << std::endl;

    return 0;
}

