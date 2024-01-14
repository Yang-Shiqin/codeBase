
go开源，安装后 `/usr/local/go` 是根目录

```bash
.
├── CONTRIBUTING.md
├── LICENSE
├── PATENTS
├── README.md
├── SECURITY.md
├── VERSION
├── api
├── bin                 ### 可执行文件，有两个指令 `go` 和 `gofmt`
├── codereview.cfg
├── doc
├── go.env
├── lib
├── misc
├── pkg
├── src                 ### 源码，有排序、系统调用、加解密等
└── test
```

指令 `go` 是编译 `.go` 文件的可执行文件。在任意位置直接用 `go` 需将此目录加入环境变量: 

在 `~/.bashrc` 中添加：
```bash
export GOROOT=/usr/local/go                 # go源码包所在路径
export GOPATH=xxxxx                         # 用户的go项目工作路径，最好只设一个
export PATH=$PATH:$GOROOT/bin:$GOPATH/bin
```

但有了go modules后就不用gopath了，不过还是兼容的

```bash
echo $PATH
# 应当找到 `/usr/local/go/bin`
```

