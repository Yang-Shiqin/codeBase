// slice的4种声明方式
package main

import "fmt"

func main(){
    // 1. 声明一个slice并初始化，长度为3
    slice1 := []int{1,2,3}
    fmt.Printf("slice1 = %v, len = %d\n", slice1, len(slice1))   // %v是任何类型的详细信息

    // 2. 声明一个slice，但没分配空间, 长度为0
    var slice2 []int
    fmt.Printf("slice2 = %v, len = %d\n", slice2, len(slice2))   // %v是任何类型的详细信息
    // slice2[0] = 1    // 会报越界的错误
    // 给slice2开辟3个容量的空间, 长度为3, 默认值都为0
    slice2 = make([]int, 3)
    fmt.Printf("slice2 = %v, len = %d\n", slice2, len(slice2))   // %v是任何类型的详细信息

    // 3. 声明并分配空间，初始值为0
    var slice3 []int = make([]int, 3)
    fmt.Printf("slice3 = %v, len = %d\n", slice3, len(slice3))   // %v是任何类型的详细信息

    // 4. 通过:=推导出slice(比较常见)
    slice4 := make([]int, 3)

    // 判断一个slice是否为空(长度为0)
    if slice4 == nil {
        fmt.Println("slice is nil")
    }else{      // 这仨必须在同一行，不然报错
        fmt.Println("slice is not nil")
    }
}

