```go
/******************************** import 和 init 执行顺序
- init作为当前包的主入口
- main作为项目的主入口
import -> const -> var -> init()

-|--->main    |
 |     ↓      |
 |import pkg1-|---|--->pkg1    |
 |            |   |     ↓      |
 | const .. <-|-┐ |import pkg2-|---|-->pkg2   |
 |     ↓      | | |            |   |    ↓     |
 |   var ..   | | | const .. <-|-┐ | const .. |
 |     ↓      | | |     ↓      | | |    ↓     |
 |   init()   | | |   var ..   | | |  var ..  |
 |     ↓      | | |     ↓      | | |    ↓     |
 |   main()   | └-|---init()   | └-|--init()  |
       ↓
     Exit
***************************************************/
```

## 本项目创建过程(go mod导本地包)
1. 编写如下文件
    ```bash
    .
    ├── lib1
    │   └── lib1.go # package lib1, 其中有全局函数Lib1Test
    ├── lib2
    │   └── lib2.go # package lib2, 其中有全局函数Lib2Test
    └── main.go     # import ("main/lib1" "main/lib2"), main是设置的模块路径
    ```
2. 创建并初始化 `go.mod` 文件
    ```bash
    go mod init main
    ```
3. 运行
    ```bash
    go run .    # 或go run main.go
    ```
