package lib1        // 一般包名和文件名同名，好找(文件名不重要，包名重要)

import "fmt"

func Lib1Test(){    // 大写才可见
    fmt.Println("lib1Test()")
}

func init(){
    fmt.Println("lib1.init()")
}


