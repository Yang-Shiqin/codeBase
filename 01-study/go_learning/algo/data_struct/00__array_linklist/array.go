package main

import "fmt"

// 插入
func insert(array []int, index int, val int){
    for i:=len(array)-1; i>index; i--{
        array[i] = array[i-1]
    }
    array[index] = val
}

// 删除
func remove(array []int, index int){
    for i:=index; i<len(array)-1; i++{
        array[i] = array[i+1]
    }
}

// 查找
func find(array []int, val int) int {
    for i, v := range array{
        if v==val{
            return i
        }
    }
    return -1
}

// 扩容
func expand(array []int, expand_len int) []int{
    new_array := make([]int, len(array)+expand_len)
    for i, val := range array{
        new_array[i] = val
    }
    return new_array
}

func main(){
    // 初始化数组
    array1 := []int{1,2,3}  // 切片slice
    var array2 [3]int       // 数组array, 不能传给上面写的那些函数

    fmt.Println(array1)
    array1 = expand(array1, 4)
    fmt.Println(array1)
    insert(array1, 2, 100)
    fmt.Println(array1)
    remove(array1, 0)
    fmt.Println(array1)
    fmt.Println(find(array1, 2))
    fmt.Println(find(array1, 10))

    fmt.Println(array2)
}

