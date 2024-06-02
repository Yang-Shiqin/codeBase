#include <iostream>
#include <vector>
using namespace std;

int main()
{
    // 初始化
    vector<int> v1; // 1. 默认初始化. 空vector, 无元素, size=0, capacity=0
    vector<int> v2{10, 20, 30}; // 2. 列表拷贝初始化, 元素需类型相容
    vector<int> v3={10, 20, 30};// 等价上面的. 整形直接拷贝, 其他类型转换(好像会报错)
    vector<int> v4(v3);         // 3. 拷贝初始化
    vector<int> v5 = v3;        // 等价上面的(符号重载)
    vector<int> v6(v3.begin(), v3.end()-1); // 4. 首尾地址中间p拷贝初始化(首尾都包含)
    vector<int> v7(7);          // 5. 默认值初始化, 包含7个元素, 每个元素进行缺省值初始化
    vector<int> v8(7, 3);       // 6. 指定值初始化, 包含7个元素, 每个元素值为3

    // 操作
    cout << "v1 is empty? " << v1.empty() << endl;
    v1.push_back(3);
    v1.resize(3);   // 大小重定为3, 多删 少补0
    v1.begin(); // 不为空时开头(类型是迭代器) 为空与end()同
    cout << "v1 begin: " << v1[0] << endl;
    cout<< "v1 end: " << *v1.end() << endl; // 结尾后一个
    
    vector<int>::iterator it;
    it = v1.begin();
    it = v1.insert ( it , 200 );    // 开头插入200
    it = v1.insert (it,2,300);      // 开头插入2个300
    v1.erase(it);                   // 删除开头第一个
    
    // 遍历
    // 1. 最普通
    for(int i=0; i<v1.size(); i++){
        cout << v1[i] << ' ';
    }
    cout << endl;

    // 2. 另一种, auto是自动检测类型
    for(auto inum : v1){
        cout << inum << ' ';
    }
    cout << endl;

    // 3. 迭代器
    for(auto it=v1.begin(); it!=v1.end(); it++){
        cout << *it << ' ';
    }
    return 0;
}

