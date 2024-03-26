package main

import "fmt"

func main(){
    a := "acid" // pair<static type: string, value: "acid">

    var b interface{}   // 此时pair的type和value都为空(引用 `interface` 也都是空)
    b = a       // b: pair<static type: string, value: "acid">
    fmt.Println(b.(string))
    // pair可以通过指针的形式进行赋值到其他变量中
}

