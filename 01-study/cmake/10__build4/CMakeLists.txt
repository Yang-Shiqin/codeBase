cmake_minimum_required(VERSION 3.10) # 最小版本
project(main CXX) # 项目名

add_subdirectory(animal)    # 嵌套的cmakelist目录

add_executable(main main.cc) # 由源文件生成一个可执行的程序

target_link_libraries(main PUBLIC dog cat)    # 链接AnimalLib库

