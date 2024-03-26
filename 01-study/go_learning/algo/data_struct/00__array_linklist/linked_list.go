package main

import "fmt"

type ListNode struct{
    Val int
    Next *ListNode
}

// 创建一个节点
func Create(val int) *ListNode{
    node := ListNode{
        Val: val,
        Next: nil,
    }
    return &node
}

// 插入
func Insert(n0 *ListNode, n1 *ListNode){
    if n0==nil{
        return
    }
    n1.Next = n0.Next
    n0.Next = n1
}

// 删除
func Remove(n0 *ListNode){
    if n0==nil || n0.Next==nil{
        return
    }
    n1 := n0.Next
    n0.Next = n1.Next
}

// 查找
func Find(head *ListNode, val int) *ListNode{
    var node *ListNode = nil
    for head != nil{
        if head.Val==val{
            node = head
            break
        }
        head = head.Next
    }
    return node
}

// 打印链表元素
func PrintVal(head *ListNode){
    for head != nil{
        fmt.Printf("%d -> ", head.Val)
        head = head.Next
    }
    fmt.Println("nil")
}

func main(){
    buf := make([]*ListNode, 5)
    for i:=0; i<5; i++{
        buf[i] = Create(i)
    }
    for i:=0; i<4; i++{
        buf[i].Next = buf[i+1]
    }
    PrintVal(buf[0])
    newNode := Create(100)
    Insert(buf[2], newNode)
    PrintVal(buf[0])
    Remove(buf[0])
    PrintVal(buf[0])
    PrintVal(Find(buf[0], 3))
    PrintVal(Find(buf[0], 10))
}

