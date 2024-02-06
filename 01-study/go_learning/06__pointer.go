package main

import "fmt"

func swap(a *int, b *int){
    var c int
    c = *a
    *a = *b
    *b = c
}

func main(){
    var a int = 10
    var b int = 20
    swap(&a, &b)
    fmt.Println("a = ", a, "b = ", b)

    var p *int
    p = &a
    var pp **int    // 二级指针
    pp = &p

    fmt.Println("&p = ", &p, "pp = ", pp)
}

