package main

import (
    "fmt"
    "main/calc"

)

func main(){
    a := 1
    b := 1
    res := calc.Add(a, b)
    fmt.Printf( format: "%d + %d = %d \n", a, b, res)
}

