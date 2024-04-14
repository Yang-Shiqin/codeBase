package main

import (
    "fmt"
    "strings"
)

// 循环数组实现的队列类
type Queue struct{
    data []int      // 队列内容
    head int        // 队列头指针(指向头)
    tail int        // 队列尾指针(指向尾后一个)
    capacity int    // 队列容量
}

// 创建空队列
func Create(capacity int) *Queue{
    return &Queue{
        data: make([]int, capacity),
        head: 0,
        tail: 0,
        capacity: capacity,
    }
}

// 获取队列长(最大为q.capacity-1)
func (q *Queue) size() int{
    return (q.tail+q.capacity-q.head)%q.capacity
}

// 是否空
func (q *Queue) isEmpty() bool{
    return q.tail==q.head
}

// 入队
func (q *Queue) push(val int){
    if q.size()==q.capacity{
        fmt.Println("queue is full, push failed")
    }else{
        q.data[q.tail] = val
        q.tail = (q.tail+1)%q.capacity
    }
}

// 访问队首元素
func (q *Queue) peek() any{
    if q.isEmpty(){
        return nil
    }else{
        return q.data[q.head]
    }
}

// 出队
func (q *Queue) pop() any{
    p := q.peek()
    if p==nil{
        fmt.Println("queue is empty, pop failed")
    }else{
        q.head = (q.head+1)%q.capacity
    }
    return p
}

// 打印queue
func (q *Queue) printQueue(){
    fmt.Println(q.data)
    s := strings.Repeat(" ", q.capacity*2)
    runes := []rune(s)
    runes[q.head*2+1] = 'h'
    runes[q.tail*2+1] = 't'
    if q.isEmpty(){
        runes[q.tail*2] = 'h'
    }
    fmt.Println(string(runes))
    fmt.Printf("queue's head = %d, queue's size = %d, is ", q.peek(), q.size())
    if !q.isEmpty(){
        fmt.Printf("not ")
    }
    fmt.Println("empty.")
}

func main(){
    // 创建queue
    q := Create(10)
    // 添加元素
    q.push(1)
    q.push(2)
    q.printQueue()
    q.push(3)
    q.push(3)
    q.push(3)
    q.push(3)
    q.push(3)
    q.push(3)
    q.push(3)
    q.printQueue()
    // 删除元素
    q.pop()
    q.printQueue()
    q.pop()
    q.pop()
    q.push(3)
    q.push(3)
    q.printQueue()
    q.pop()
}

