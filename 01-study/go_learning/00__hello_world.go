package main    // 包，main包是项目入口，有且仅有一个

import "fmt"    // 引入包，fmt是go原生包，用来输出文本

func main(){    // 每个项目只有一个main函数; {要和函数在同一行
    fmt.Println("Hello, World!")
}

// 运行：`go run <filename>.go`
// 定义或导入但没引用的话，会报错。go编译器比较严格
