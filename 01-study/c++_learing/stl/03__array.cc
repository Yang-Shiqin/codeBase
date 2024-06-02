#include <iostream>
#include <array>

int main() {
    // 初始化一个包含5个元素的array
    std::array<int, 5> arr = {1, 2, 3, 4, 5};

    // 访问元素
    std::cout << "First element: " << arr[0] << std::endl;
    std::cout << "Second element: " << arr.at(1) << std::endl;

    // 获取大小
    std::cout << "Array size: " << arr.size() << std::endl;

    // 迭代元素
    std::cout << "Array elements: ";
    for (int elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 修改元素
    arr[2] = 10;
    std::cout << "After modification, third element: " << arr[2] << std::endl;

    // 获取指向数据的指针
    int* dataPtr = arr.data();
    std::cout << "Pointer to data: " << *dataPtr << std::endl;

    // 填充数组
    arr.fill(0);
    std::cout << "After fill, array elements: ";
    for (int elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // 获取数组的前两个元素
    std::array<int, 2> firstTwo = {arr[0], arr[1]};
    std::cout << "First two elements of new array: ";
    for (int elem : firstTwo) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}

