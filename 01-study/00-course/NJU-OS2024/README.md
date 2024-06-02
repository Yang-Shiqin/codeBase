
## TODO
1绪论(01|02)
- [o] minimal: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/minimal/"`
- [ ] tar(toybox): `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/tar/"`
    - 对我来说太大了，看不下去
- [o] **hanoi**: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/hanoi/"`
- [o] strace: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/strace/"`
- [o] compiler-opt: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/compiler-opt/"`
    - [查看编译器行为](https://godbolt.org/z/f698djMTq)
- [ ] 02阅读材料

2绪论(03|04)
- [o] gdb-tui
- [ ] 查看系统分区: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/partitions/"`
    - wsl没法 `lsblk`
- [o] 调试固件: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/debug-firmware/"`
    - `make debug`, `si`*3, 可以看到内存加载进来了
- [ ] 调试 MBR 中的加载器: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/debug-bootloader/"`
    - 看了但没成功
    - 需要先下载南大os的[am](https://github.com/NJU-ProjectN/abstract-machine), 再在wsl `export AM_HOME=/mnt/d/00/github/os-workbench-2022/abstract-machine`(示例), 再在makefile中加 `CFLAGS += -I/mnt/d/00/github/os-workbench-2022/abstract-machine/am/include`。才能 `make`
    - 先 `make`, 会生成 `build` 文件夹, 其下有 `hello-x86-qemu` 的磁盘镜像, 之后会加载到虚拟机中运行
    - 再 `make run` (需要先实现am)
    - 再 `make debug` (makefile和老师演示的不一样, 要-ggdb)
- [o] **os-model**: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/os-model/"`
    - 值得反复阅读+调试（观察 generator 是如何实现状态机的切换的）
- [ ] model checker: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/mosaic/mosaic/"`
    - `py mosaic.py -c examples/intro/hello.py | grep stdout | sort | uniq`
    - 可以等课程结束后回来
- [ ] L0


- [ ] : ``
- [ ] : ``
- [ ] : ``