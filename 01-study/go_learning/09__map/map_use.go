package main

import "fmt"

func main(){
    cityMap := map[string]string {
        "China": "Beijing",
        "USA": "NewYork",
    }
    for key, value := range cityMap{
        fmt.Println("value = ", value)

    }
}

