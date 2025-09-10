#include <iostream>
#include <vector>
#include <list>
using namespace std;

template<typename K, typename V>
class Hash{
private:
    using Bucket = list<pair<K, V>>;
    vector<Bucket> buckets;
    size_t size_;   // 元素个数
    float load_factor_threshold = 0.75f;    // 负载因子
    std::hash<K> hasher;
    size_t bucket_index(const K& key) const{
        return hasher(key)%buckets.size();
    }
    // TODO: 扩容
public:
    explicit Hash(size_t init_cap = 17): buckets(init_cap), size_(0){}
    void insert(const K& key, const V& val){
        size_t idx = bucket_index(key);
        for (auto& pi : buckets[idx]){
            if (pi.first==key){
                pi.second = val;
                return;
            }
        }
        buckets[idx].emplace_back(key, val);
        ++size_;
    }
    void erase(const K& key){
        size_t idx = bucket_index(key);
        auto& buck = buckets[idx];
        for (auto it=buck.begin(); it!=buck.end(); it++){
            if (it->first==key){
                buck.erase(it);
                --size_;
                return;
            }
        }
    }
    V* find(const K& key){
        size_t idx = bucket_index(key);
        for (auto& pi : buckets[idx]){
            if (pi.first==key){
                return &pi.second;
            }
        }
        return nullptr;
    }
    size_t size(){
        return size_;
    }
    bool empty(){
        return size_==0;
    }
};

int main()
{
    Hash<int, int> h;
    h.insert(1, 10);
    h.insert(2, 20);
    h.insert(3, 30);
    h.insert(1, 100);
    cout << *h.find(2) << endl;
    cout << *h.find(1) << endl;
    h.erase(2);
    cout << h.find(2) << endl;
    std::cout << "Hello world" << std::endl;
    return 0;
}

