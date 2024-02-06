package main

import "fmt"

func main(){
    // 1. 声明myMap1类型为 map[string]string, 只是声明没分配空间
    var myMap1 map[string]string    // map[key类型]value类型
    if myMap1 == nil{
        fmt.Println("myMap1 is nil")
    }else{
        fmt.Println("myMap1 is not nil")
    }

    // 分配空间
    myMap1 = make(map[string]string, 1) // 长度不指定也行
    // 超过分配的空间长度，会动态扩容(slice不行, slice用append才行)
    myMap1["1"] = "lza"
    myMap1["2"] = "ysq" // 是乱序的，而不是像python一样先者在前
    fmt.Println(myMap1)

    // 2. 声明map
    myMap2 := map[string]string {
        "1": "ysq",
        "2": "lza", // 最后一行也得有','
    }
    fmt.Println(myMap2)
}

