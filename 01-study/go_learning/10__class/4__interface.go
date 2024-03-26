// 多态, 抽象方法继承重写
package main

import "fmt"

// interface类型(抽象类), 本质是一个指针
type AnimalIF interface{
    Sleep()
    GetType() string    // 获取动物种类
}

// 具体的类
type Cat struct{
    // AnimalIF //继承接口不用像继承类一样写下来，只用把方法实现就行
    type_ string
}

func (this *Cat) Sleep(){
    fmt.Println("Cat sleeps")
}

func (this *Cat) GetType() string{
    return this.type_
}

type Dog struct{
    type_ string
}

func (this *Dog) Sleep(){
    fmt.Println("Dog sleeps")
}

func (this *Dog) GetType() string{
    return this.type_
}

func main(){
    var animal AnimalIF     // 父类指针
    animal = &Cat{"Cat"}
    animal.Sleep()
    animal = &Dog{"Dog"}
    animal.Sleep()
}

