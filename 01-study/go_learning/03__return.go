package main

import "fmt"

func foo(a string, b int) int {
    fmt.Println("==== foo ====")
    fmt.Println("a =", a)
    fmt.Println("b =", b)
    c := 100
    return c
}

// 返回多个返回值, 匿名
func foo2(a string, b int) (int, int) {
    fmt.Println("==== foo2 ====")
    fmt.Println("a =", a)
    fmt.Println("b =", b)
    return 666, 777
}

// 返回多个返回值, 有形参名称的(r1,r2属于foo3的形参，作用域为foo3)
func foo3(a string, b int) (r1 int, r2 int) {   // 类型相同也能写成(r1, r2 int)
    fmt.Println("==== foo3 ====")
    fmt.Println("a =", a)
    fmt.Println("b =", b)
    fmt.Println("r1 =", r1, "r2 =", r2) // 形参，是局部变量，已经定义，初值为默认值
    // 给有名称的返回值赋值
    r1 = 1000
    r2 = 2000
    return
    // 也能写成return 1000, 2000
}

func main(){
    c := foo("abc", 123)
    fmt.Println("c =", c)
    ret1, ret2 := foo2("abc", 123)
    fmt.Println("ret1 =", ret1, "ret2 =", ret2)

    ret1, ret2 = foo3("abc", 123)
    fmt.Println("ret1 =", ret1, "ret2 =", ret2)
}

