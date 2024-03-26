package main

import "fmt"

// 抽象父类接口
type Reader interface{
    ReadBook()
}

type Writer interface{
    WriteBook()
}

// 具体类，重写两个接口
type Book struct{
}

// 重写接口
func (this *Book) ReadBook(){
    fmt.Println("read")
}

func (this *Book) WriteBook(){
    fmt.Println("write")
}

func main(){
    // pair<type: Book, value: Book{}地址>
    b := &Book{}

    // pair<type: , value: >
    var r Reader
    // pair<type: Book, value: Book{}地址>
    r = b
    r.ReadBook()

    // pair<type: , value: >
    var w Writer
    // pair<type: Book, value: Book{}地址>
    w = r.(Writer)  // 能够成功，断言有两步: 得到变量的type(Book)，判断是否实现目标接口(Writer)
    w.WriteBook()
}

