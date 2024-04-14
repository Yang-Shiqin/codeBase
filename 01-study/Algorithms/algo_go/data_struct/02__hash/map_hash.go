package main

import "fmt"

func main(){
    // 创建
    hmap := make(map[int]string)
    // 添加
    hmap[111] = "sdf"
    hmap[222] = "er"
    hmap[333] = "23s"
    fmt.Println(hmap)
    // 查询
    fmt.Println("hmap[111] = ", hmap[111])
    // 删除
    delete(hmap, 222)
    fmt.Println(hmap)
}

