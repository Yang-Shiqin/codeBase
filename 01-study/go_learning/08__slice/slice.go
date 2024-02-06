// 动态数组/切片
package main

import "fmt"

func printArray(array []int){
    for _, v := range array{
        fmt.Println("value: ", v)
    }
}

func doubleArray(array []int){
    for i, v := range array{
        array[i] = v*2
    }
}

func main(){
    array := []int{1,2,3}   // 动态数组, 切片slice, 就是指针
    fmt.Printf("array type: %T\n", array)
    printArray(array)
    doubleArray(array)  // 指针引用，所以直接在真正的array上做修改
    printArray(array)
}

