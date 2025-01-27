cmake_minimum_required(VERSION 3.10)
# set一个值
set(var1 ysq)
message(${var1})
set("var2 with space" lza)
message(${var2\ with\ space})

# set多个值
set(list1 a1 a2)
message(${list1})
set(list2 b1;b2)
message(${list2})

# 访问系统变量中的PATH
message($ENV{PATH})
# 设置系统变量中的CXX为g++
set(ENV{CXX} "g++")
message($ENV{CXX})
# unset
unset(ENV{CXX})
# message($ENV{CXX})
