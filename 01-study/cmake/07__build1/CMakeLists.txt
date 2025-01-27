cmake_minimum_required(VERSION 3.10) # 最小版本
project(main CXX) # 项目名
add_executable(main main.cc animal/dog.cc) # 由源文件生成一个可执行的程序
