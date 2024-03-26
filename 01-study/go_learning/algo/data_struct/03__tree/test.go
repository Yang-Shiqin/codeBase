package main

import "fmt"
func test(a *[]int){
    *a = append(*a, 2)
}

func main(){
    var a []int
    test(&a)
    fmt.Println(a)
}

