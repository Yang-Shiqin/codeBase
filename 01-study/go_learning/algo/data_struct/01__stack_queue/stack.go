/* 栈stack的常用操作:
    - push  : 元素入栈
    - pop   : 元素出栈
    - peak  : 访问栈顶元素
*************************/
package main

import "fmt"

type Stack struct{
    data []int
}

// 入栈
func (s *Stack)Push(val int){   // Stack的方法
    s.data = append(s.data, val)
}

// 出栈
func (s *Stack)Pop() int {
    val := s.Peek()
    s.data = s.data[:len(s.data)-1]
    return val
}

// 访问栈顶元素
func (s *Stack) Peek() int {
    return s.data[len(s.data)-1]
}

func main(){
    s := Stack{
        data: []int{1,2,3},
    }
    fmt.Println(s.data)
    s.Push(4)
    fmt.Println(s.data)
    s.Pop()
    fmt.Println(s.data)
    s.Peek()
    fmt.Println(s.data)
}

