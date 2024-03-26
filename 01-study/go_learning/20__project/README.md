
一个 go 程序中，如果定义了 main package main function，那么它会作为整个程序的入口，所有的用户过程都是从这里开始

## GOPATH环境
```bash
path of $GOPATH
├── README.md
├── bin                     # 存放编译后的可执行文件，一般go自动生成
├── pkg                     # 存放编译后的包文件，一般go自动生成
└── src                     # 存放项目源文件，自己建
    └── main                # 项目名（可以随便取名
        ├── calc            # 包名为calc，文件夹下所有文件属于同一个包，所以不同文件的函数变量也不能重复
        │   └── add.go      # 名字不重要
        └── main.go         # 入口
```

## go modules
更简单的目录组织
```bash
path of everywhere
├── go.mod
└── main.go
```

在目录 `path of everywhere` 运行
1. 方法1
    1. `go mod init <mod_name>` : 创建mod
    2. `go run <file_name>.go` : 运行 `main.go`(可执行程序生成在临时目录下用来测试)
2. 方法2
    1. `go build <file_name>.go` : 编译成可执行程序
    2. 或 `go build` : 生成的是 `<mod_name>.mod` 中定义的模块名称，可带 `-o`
    3. `./<file_name>`

执行 go build 或者 go run 命令时，会查找指向目录是不是一个 main package，再查找这个 main package 是否有一个 main 函数

## 交叉编译
