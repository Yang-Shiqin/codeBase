package main

import "fmt"

// interface{}是万能类型
func myFunc(arg interface{}){
    fmt.Println("myFunc is called, arg is ", arg)

    // interface{}通过"类型断言"机制区分引用的底层数据类型
    value, ok := arg.(string)   // 类型断言
    if ok { // 是string
        fmt.Println(arg, "is string, value = ", value)
    }else{  // 不ok的话value无效
        fmt.Println(arg, "is not string")
    }
}

func main(){
    a := "lza"
    b := 123
    myFunc(a)
    myFunc(b)
}

