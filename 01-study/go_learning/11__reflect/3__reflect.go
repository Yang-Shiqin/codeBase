// reflect库的两个函数: TypeOf, ValueOf
package main

import (
    "fmt"
    "reflect"
)

type Book struct{
    Id int
    Name string
}

func rfNum(arg interface{}){
    fmt.Println("type: ", reflect.TypeOf(arg))
    fmt.Println("value: ", reflect.ValueOf(arg))
}

func rfField(arg interface{}){  // 调用接口查看复杂类型内容
    t := reflect.TypeOf(arg)
    v := reflect.ValueOf(arg)
    for i := 0; i<t.NumField(); i++{
        field := t.Field(i)
        value := v.Field(i).Interface()
        fmt.Printf("%s: %v = %v\n", field.Name, field.Type, value)
    }
}

func main(){
    var num float64 = 1.23
    rfNum(num)
    book := Book{1,"ysq"}
    rfField(book)
}

