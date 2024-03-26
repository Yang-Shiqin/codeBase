package main

import "fmt"

type Human struct{
    name string
    sex string
}

func (this *Human) Eat(){
    fmt.Println("Human.Eat()")
}

func (this *Human) Walk(){
    fmt.Println("Human.Walk()")
}

type SuperMan struct{
    Human   // SuperMan继承Human类的方法(没有公有私有之类的)
    level int
}

// 重定义父类的方法Walk()
func (this *SuperMan) Walk(){
    fmt.Println("SuperMan.Walk()")
}

func (this *SuperMan) Fly(){
    fmt.Println("SuperMan.Fly()")
}

func main(){
    s := SuperMan{
        Human{
            "ysq",
            "female",
        },
        88,
    }
    s.Eat()
    s.Walk()
    s.Fly()
    fmt.Println(s.name)
}

