导包但不调用会报错。但有时不想用包的接口，只是想用他的init。这时就有了一种匿名导包的方式
```go
// 会报错
import (
    "main/lib1"
)

// 不报错
import (
    _ "main/lib1"   // 起个别名'_'(即匿名)
)

// 别名
import (
    l1 "main/lib1"   // 起个别名'_'(即匿名)
)
func main(){
    l1.Lib1Test()
}

// 导到当前包(不推荐, 同名会歧义)
import (
    . "main/lib1"   // 起个别名'_'(即匿名)
)
func main(){
    Lib1Test()
}
```
