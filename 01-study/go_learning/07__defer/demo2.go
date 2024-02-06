package main

import "fmt"

func deferFunc() int {
    fmt.Println("defer func called..")
    return 0
}

func returnFunc() int {
    fmt.Println("return func called..")
    return 0
}

// return 和 defer 谁先谁后: return早，defer晚
func returnAndDeder() int {
    defer deferFunc()
    return returnFunc()
}

func main(){
    returnAndDeder()
}

