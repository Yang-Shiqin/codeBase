
## TODO
### 1绪论(01|02)
- [o] minimal: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/minimal/"`
- [ ] tar(toybox): `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/tar/"`
    - 对我来说太大了，看不下去
- [o] **hanoi**: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/hanoi/"`
- [o] strace: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/strace/"`
- [o] compiler-opt: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/introduction/compiler-opt/"`
    - [查看编译器行为](https://godbolt.org/z/f698djMTq)
- [ ] 02阅读材料

### 2绪论(03|04)
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
    - `-c` 应该会运行很多，check模式
    - 可以等课程结束后回来
    - mosaic与os-model的不同
        - 因为重点在并发，额外正式建模了共享内存heap（在不在多个cpu上不重要，python的不是真正的同时也不重要）

### 3并发-互斥(05|06|07)
- [o] 并发线程模型: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/thread-model/"`
    - 借助04的mosaic运行：`py mosaic.py examples/thread.py | py collect.py`
- [o] gdb cheat sheet
- [o] **最小线程库thread-lib**: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/thread-lib/"`
    - 封装c的pthread让其更易用
    - 在之后的代码中太常用，所以 `export TLIB_PATH=/mnt/d/00/github/codeBase/01-study/00-course/NJU-OS2024/05__mp/thread-lib/`
- [o] 线程自问自答: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/thread-qa/"`
- [o] 内存模型: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/mem-model/"`
    - mem-model.py: 也是用mosaic运行，结果没有00说明我们简化的model checker其实也没有考虑真实场景(动态指令调度和缓存)
        - `py mosaic.py -c ../../05__mp/mem-model/mem-model.py | py collect.py`
    - mem-model.c: 真实场景的mem-model.py
        - `make`, `./mem-model | head -n 10000 | sort | uniq -c`
- [o] Peterson: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/peterson/"`
    - py: `py mosaic.py -c ../../06__mutex/peterson/peterson.py | grep \"cs\" | sort | uniq`
    - c: `make`, 运行, 改代码跑没有barrier会报错
- [ ] stop the world: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/stop-the-world/"`
    - 用户态: make cli, 会segmentation fault, 因为不让关中断
    - 内核态: 改路径, make, 用qemu, 成功
    - `info registers eflags`
- [o] 原子指令求和: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/sum-atomic/"`
    - `time ./sum`
- [o] 使用 cmpxchg 实现自旋锁: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/sum-locked/"`
- [ ] 操作系统内核中的自旋锁实现: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/spinlock/"`
    - 看了，要运行的话应该要实现am.h
    - 因为在裸机上所以可以关中断？
- [o] 自旋锁 (手工实现)、互斥锁和原子指令求和的scalability: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/sum-mutex/"`
    - `make`, `time ./mutex 2`/`atomic`/`spin`, `py plot.py > /dev/null`
- [ ] [xv6操作系统的代码](https://github.com/mit-pdos/xv6-riscv)

### 4并发-同步(09|10)
- [ ] 同步音乐: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/sync/"`
    - `py gen-notes.py` 生成音乐文件, `make`
    - `ffplay -nodisp piano_key_40.wav` 播放，wsl不行也许是调不了硬件？
- [o] **生产者消费者**: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/producer-consumer/"`
    - `./pc-1 3 4 | py check.py 3`, check.py的参数是括号深度
- [o] 同步打印小鱼: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/fish/"`
- [o] 用互斥锁实现同步: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/sync-mutex/"`
    - 看了没法跑, 钢琴和弦
- [o] 用互斥锁实现计算图: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/graph/"`
- [o] 用信号量实现join: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/join-sem/"`
- [o] **使用信号量实现生产者-消费者问题**: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/pc-sem/"`
    - 优雅
- [o] 哲学家吃饭: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/philosophers/"`
    - 加table信号量则不会死锁，不加就死锁
- [o] 信号量实现条件变量(错误实现): `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/cv-sem/"`

### 5并发-其他(11|12|13)
- [o] 绘制 Mandelbrot set(并行画数学计算的分形图): `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/mandelbrot/"`
    - `make`, `./mandelbrot 线程数`, `convert mandelbrot.ppm mandelbrot.png`
    - omp注释形式写并行, 降低并行编程门槛
- [ ] Golang 并发编程案例: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/go-examples/"`
- [o] SIMD 指令演示: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/simd/"`
- [ ] SIMT 模型: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/simt/"`
    - 不太明白
- [o] 山寨支付宝: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/alipay/"`
- [o] **lockdep**: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/lockdep/"`
- [o] AddressSanitizer: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/asan/"`
- [o] ThreadSanitizer: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/concurrency/tsan/"`

### 虚拟化-进程(14)
- [ ] 最小 Linux: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/linux-minimal/"`
    - win文件名不区分大小写，所以把makefile里的.s的名字都改了，但是wsl里没有 `/boot/vmlinuz`，所以运行不起来
    - 创建文件系统的镜像，放在 `build/initramfs/` 里
- [o] 理解 fork(): `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/fork-demo/"`
    - `py mosaic.py -c ../../14__process/fork-demo/fork-demo.py | py collect.py`
    - `py mosaic.py -c ../../14__process/fork-demo/fork-printf.py | py collect.py` 6个星
    - `py mosaic.py -c ../../14__process/fork-demo/fork-printf1.py | py collect.py` 8个星
    - `seq 10 | xargs -n1 -P4 ./demo-1` 4个进程并行运行10次demo-1
    - `./demo-2 | wc -l` 是8，因为管道是fully buffered
- [o] 理解 execve: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/execve-demo/"`
- [o] 理解 exit: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/exit-demo/"`
    - `./demo [exit|_exit|__exit]`, 然后 `echo $status` 查看return的值
    - `strace ./demo _exit`

### 虚拟化-地址空间(15)
- [o] 最小可执行文件: 绪论
    - 看其内存空间：`gdb minimal`, `starti`, ctrl-z, `ps`, `cat /proc/[pid]/maps`
- [o] mmap: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/mmap/"`
    - `strace ./alloc`, 不就是一下子分配的吗?
- [o] vdso: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/vdso/"`
    - `strace -e trace=gettimeofday ./vdso` 还是出现了gettimeofday，可能是我是wsl？
- [ ] **金山游侠**: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/knight/"`
    - 看了，但是没有游戏给我们验证
    - 原理: `/proc/[pid]/mem` 实现入侵; 查找值+过滤实现定位重要数据地址
- [ ] 按键精灵: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/anjian/"`
    - 看了，但是似乎在win里用不了。`sudo ./anjian`
- [ ] 变速齿轮: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/gear/"`
    - 看了，但不能在gettimeofday打断点，因为在我这还是内核的系统调用
- [o] 软件动态更新: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/dsu/"`

### 虚拟化-系统调用(16|17)
- [o] 文件描述符: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/filedesc/"`
- [o] unix管道: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/pipe/"`
    - 管道两边进程同步
    - named-pipe: 两个同时写，一个读，只能读到一个；两个同时读，一个写，一个会读到一个读不到
    - `ls -l /tmp/my_pipe` 第一个是p代表这个对象是管道
    - `echo sdf > /tmp/my_pipe`, `./named-pipe read` 也会读到
    - `vi - < /tmp/my_pipe`, `./named-pipe write wss` 也能写
- [o] **Freestanding Shell**: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/sh/"`
    - 其中c语言实现c++继承那段代码可以学习
    - 看完了, 还没调试过
    - `make`
    - `gdb -x init.gdb sh` 调试
    - `file sh`
    - `objdump -d sh` 看入口
    - `readelf -h sh` 看入口
    - 几乎完整的unix shell
- [ ] 调试 “小程序”: `wget -r -np -nH --cut-dirs=2 -R "index.html*" "https://jyywiki.cn/os-demos/virtualization/musl-demos/"`
    - 用的musl-gcc(是一段脚本, 调了一段脚本, 让编译器链接自己的libc而不是glibc(GPT也能写这样的脚本))
- [ ] mmap

### 17
- [ ] : ``
- [ ] : ``
- [ ] : ``
- [ ] : ``
