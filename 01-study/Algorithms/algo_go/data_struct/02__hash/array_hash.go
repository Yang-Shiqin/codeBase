/* 哈希表hash table的常用操作:
    - get   : 查询
    - put   : 添加
    - remove: 删除
*************************/
package main

import "fmt"

// 键值对
type pair struct{
    key int
    value string
}

// 基于数组实现的hash表
type Hash struct{
    buckets []*pair
    capacity int
}

// 创建
func create(capacity int) *Hash {
    b := make([]*pair, capacity)
    return &Hash{
        buckets : b,
        capacity: capacity,
    }
}

// 定义hash函数
func (h *Hash) hashMap(key int) int{
    return key%h.capacity
}

// 查询
func (h *Hash) get(key int) any {
    index := h.hashMap(key)
    pair := h.buckets[index]
    if pair == nil{
        return nil
    }else{
        return pair.value
    }
}

// 添加
func (h *Hash) put(key int, value string){
    pair := &pair{
        key  : key,
        value: value,
    }
    h.buckets[h.hashMap(key)] = pair
}

// 删除
func (h *Hash) remove(key int){
    h.buckets[h.hashMap(key)] = nil
}

// 获取所有键值对
func (h *Hash) pairSet() []*pair{
    var pairs []*pair
    for _, pair := range h.buckets{
        if pair != nil{
            pairs = append(pairs, pair)
        }
    }
    return pairs
}

// 获取所有键
func (h *Hash) keySet() []int{
    var keys []int
    for _, pair := range h.buckets{
        if pair != nil{
            keys = append(keys, pair.key)
        }
    }
    return keys
}

// 获取所有值
func (h *Hash) valueSet() []string{
    var values []string
    for _, pair := range h.buckets{
        if pair != nil{
            values = append(values, pair.value)
        }
    }
    return values
}

// 打印
func (h *Hash)printHash(){
    for _, pair := range h.buckets{
        if pair != nil{
            fmt.Println(pair.key, " -> ", pair.value)
        }
    }
}

func main(){
    h := create(10)
    h.put(33, "ysq")
    h.put(25, "lza")
    h.printHash()
    fmt.Println(h.valueSet())
}

