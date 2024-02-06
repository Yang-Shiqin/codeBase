// 数组
package main

import "fmt"

// 值拷贝，原数值元素内容不会被改变
func printArray(array [10]int){
    for index, value := range array{
        fmt.Println("index: ", index, "value: ", value)
    }
}

func main(){
    var array1 [10]int
    array2 := [6]int{1,2,3,4}
    for i := 0; i < len(array1); i++ {
        fmt.Println(array1[i])
    }

    for index, value := range array2{
        fmt.Println("index: ", index, "value: ", value)
    }

    fmt.Printf("array1 type = %T\n", array1)
    fmt.Printf("array2 type = %T\n", array2)
    printArray(array1)
    // printArray(array2)   // 会报错

}

