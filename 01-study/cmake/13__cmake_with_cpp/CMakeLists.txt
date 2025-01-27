cmake_minimum_required(VERSION 3.10) # 最小版本
project(main CXX) # 项目名

# 设置c++的标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(config.h.in config.h)

add_subdirectory(animal)    # 嵌套的cmakelist目录

add_executable(main main.cc) # 由源文件生成一个可执行的程序

target_link_libraries(main PUBLIC dog cat)    # 链接AnimalLib库

target_include_directories(main PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/animal")
