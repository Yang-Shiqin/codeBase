package main

import "fmt"

// 引用传递
func printMap(cityMap map[string]string){
    fmt.Println("---------")
    // 遍历
    for key, value := range cityMap{
        fmt.Printf("key = %v, value = %v\n", key, value)
    }
}

func main(){
    // 添加
    cityMap := map[string]string {
        "China": "Beijing",
        "USA": "NewYork",
    }
    printMap(cityMap)

    // 删除
    delete(cityMap, "China")
    printMap(cityMap)
    
    // 修改
    cityMap["USA"] = "DC"
    printMap(cityMap)
}

