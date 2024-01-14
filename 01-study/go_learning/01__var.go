package main

import "fmt"

// 变量的4中声明方法（本质还是有类型语言，类型定了不能变
// 全局变量，方法123都可，4不行
var gA int
var gB = 1

func main(){
    // 方法1: 不初始化，为默认值，int是0
    var a int
    var b string
    fmt.Println("a=", a, "b=", b)

    // 方法2: 初始化声明
    var c int = 100
    fmt.Println("c=", c)
    fmt.Printf("type of c = %T\n", c)

    // 方法3: 省去数据类型，根据初始化值自动匹配
    var d = 100
    var e = "ysq"
    fmt.Println("d=", d, "e=", e)
    fmt.Printf("type of d = %T, type of e = %T\n", d, e)

    // 方法4: (最常用)省var，直接自动匹配，象牙符(未声明则初始化，否则报错)
    f := "ysq"
    fmt.Println("f=", f)

    // 声明多个变量
    var aa,bb = 12, "lza"
    fmt.Println("aa=", aa, "bb=", bb)

    var (
        cc int = 100
        dd bool = true
    )
    fmt.Println("cc=", cc, "dd=", dd)

    fmt.Println("gA=", gA)
    fmt.Println("gB=", gB)
}

