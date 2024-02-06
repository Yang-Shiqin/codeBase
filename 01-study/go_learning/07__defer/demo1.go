// defer关键字: 可以在函数体的任意位置出现(但不能在return后)，相当于析构，会在函数执行完执行。defer后接执行的函数。
// defer可以出现多个，栈的形式排序，先出现的最后执行


package main

import "fmt"

func func1(){
    fmt.Println("A")
}

func func2(){
    fmt.Println("B")
}

func func3(){
    fmt.Println("C")
}

func main(){
    defer func1()
    defer func2()
    defer func3()
    fmt.Println("Hello, World!")
}

