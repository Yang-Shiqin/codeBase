#include <iostream>
#include <set>

int main() {
    // 初始化一个set
    std::set<int> mySet = {1, 2, 3, 4, 5};

    // 插入元素
    mySet.insert(6);
    mySet.insert(2); // 插入重复元素，不会添加到set中

    // 迭代并打印元素
    std::cout << "Set elements: ";
    for (const int& elem : mySet) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 查找元素
    auto it = mySet.find(3);
    if (it != mySet.end()) {
        std::cout << "Element 3 found in the set." << std::endl;
    } else {
        std::cout << "Element 3 not found in the set." << std::endl;
    }

    // 删除元素
    mySet.erase(4);

    // 使用迭代器删除元素
    it = mySet.find(5);
    if (it != mySet.end()) {
        mySet.erase(it);
    }

    // 迭代并打印删除后的元素
    std::cout << "Set elements after deletion: ";
    for (const int& elem : mySet) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 检查元素是否存在
    if (mySet.count(2)) {
        std::cout << "Element 2 is in the set." << std::endl;
    } else {
        std::cout << "Element 2 is not in the set." << std::endl;
    }

    // 清空set
    mySet.clear();
    std::cout << "Set size after clear: " << mySet.size() << std::endl;

    return 0;
}

