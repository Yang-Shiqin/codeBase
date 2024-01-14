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

package main

import (
    "lib1"
    "lib2"
)

func main(){
    lib1.Lib1Test()
    lib2.Lib2Test()
}

