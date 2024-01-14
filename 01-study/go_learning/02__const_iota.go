package main

import "fmt"

// const来定义枚举
const (
    RED = 0
    YELLOW = 1
    BLUE = 2
)

// iota只能用于const
// const()添加关键字iota, 每行的iota都会累加1, 第一行的iota的默认值是0
const (
    BEIJING = iota  // iota=0
    SHANGHAI        // iota=1
    SHENZHEN        // itoa=2
)

// 公式用于每行, 可以中途改公式
const (
    a = 10*iota     // iota=0, a=0
    b               // iota=1, b=10
    c               // itoa=2, c=20
)

// 更复杂的例子
const (
    d, e = iota+1, iota+2   // iota=0, d=1, e=2
    f, g                    // iota=1, f=2, g=3

    h, i = iota*2, iota*3   // iota=2, h=4, i=6
    j, k                    // iota=3, j=6, k=9
)

func main(){
    // 常量(只读)
    const l int = 10
    fmt.Println("l =", l)

    // 下面会报错
    // l = 100
}

