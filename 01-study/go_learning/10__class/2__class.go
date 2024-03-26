package main

import "fmt"

type Hero struct{
    Name string
    Ad int
    Level int   // 大写全局
}

// 类方法
func (this *Hero) Show(){
    fmt.Println("Name = ", this.Name)
    fmt.Println("Ad = ", this.Ad)
    fmt.Println("Level = ", this.Level)
}

func (this *Hero) GetName() string{
    return this.Name
}

func (this *Hero) SetName(name string){
    this.Name = name
}

func main(){
    hero := Hero{
        Name: "ysq",
        Level: 100,
    }
    hero.Show()
    hero.SetName("lza")
    hero.Show()
}

