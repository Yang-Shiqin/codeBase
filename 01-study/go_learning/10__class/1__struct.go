package main

import "fmt"

// 声明新变量myint为int(类似c的typedef)
type myint int

// 定义结构体
type Book struct {
    title string
    author string
}

// 值传递，传的是book的副本
func printBook(book Book){
    fmt.Println(book.title, book.author)
    book.title = "lza's Book"
    fmt.Println(book.title, book.author)
}

func main(){
    var a myint = 10        // 等价 var a int = 10
    fmt.Println("a = ", a)
    var book1 Book
    book1.title = "ysq's Book"
    book1.author = "ysq"
    fmt.Printf("%v\n", book1)
    printBook(book1)
    printBook(book1)
}

