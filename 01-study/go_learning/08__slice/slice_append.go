// slice切片追加与截取
// 切片的长度和容量不一样，append可以自动扩容，容量较小时两倍扩容，较大时会尝试1/4扩容
// 截取与py差不多(但不能用-1)，不是浅拷贝. 就是设置两个指针(头尾), 直接指源数据位置
package main

import "fmt"

func main(){
    slice := make([]int, 3, 5)  // 3为长度，默认值是0；5是容量
    fmt.Printf("len = %d, cap = %d, slice = %v, pslice = %p\n", len(slice), cap(slice), slice, slice)
    /************ slice示意图
    slice     尾指针ptr
      v           v
    | 0 | 0 | 0 |   |   |
    └──────────len      |
    └──────────────────cap
    *************/
    
    // 追加
    slice = append(slice, 1)    // len=4, cap=5
    fmt.Printf("len = %d, cap = %d, slice = %v, pslice = %p\n", len(slice), cap(slice), slice, slice)
    slice = append(slice, 5)    // len=5, cap=5
    slice = append(slice, 6)    // len=6, cap=10. 动态开辟空间, 大小为之前的cap两倍, 新地址然后拷贝的
    fmt.Printf("len = %d, cap = %d, slice = %v, pslice = %p\n", len(slice), cap(slice), slice, slice)

    // 切片
    slice2 := slice[:2]
    slice2[0] = 100
    fmt.Printf("len = %d, cap = %d, slice = %v, pslice = %p\n", len(slice), cap(slice), slice, slice)
    fmt.Printf("len = %d, cap = %d, slice2 = %v, pslice2 = %p\n", len(slice2), cap(slice2), slice2, slice2)

    // copy, slice2拷贝到slice3(dst, src)
    slice3 := make([]int, 3, 10)
    fmt.Printf("len = %d, cap = %d, slice3 = %v, pslice3 = %p\n", len(slice3), cap(slice3), slice3, slice3)
    copy(slice3, slice2)    // 一个元素一个元素拷贝，最多只会拷贝dst长度(而不是容量)的元素数
    fmt.Printf("len = %d, cap = %d, slice3 = %v, pslice3 = %p\n", len(slice3), cap(slice3), slice3, slice3)
}

