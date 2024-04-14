/* 队列queue的常用操作:
    - push  : 元素入队(队尾)
    - pop   : 元素出队(队首)
    - peak  : 访问队首元素
*************************/
package main

import (
    "container/list"
    "fmt"
)

func main(){
    // 直接用go现成的，list作为queue
    // 创建队列
    queue := list.New()
    fmt.Println(queue)

    // 入队
    queue.PushBack(1)
    queue.PushBack(2)
    queue.PushBack(3)
    fmt.Println(queue)

    // 访问队首元素
    pop := queue.Front()
    fmt.Println(pop)

    // 出队
    queue.Remove(pop)
    fmt.Println(queue)

    // 获取队列长度
    size := queue.Len()
    fmt.Println(size)
}

