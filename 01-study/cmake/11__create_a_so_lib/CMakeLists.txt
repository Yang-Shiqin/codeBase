cmake_minimum_required(VERSION 3.10) # 最小版本
project(main CXX) # 项目名

file(GLOB SRC ${PROJECT_SOURCE_DIR}/src/*.cc)      # src下所有cc文件放入SRC中
include_directories(${PROJECT_SOURCE_DIR}/include)  # 引入头文件

# # 生成静态库
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/a)    # 设置输出链接库目录
# add_library(animal STATIC ${SRC}) # 生成静态库

# 生成动态库
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/so)    # 设置输出链接库目录
add_library(animal SHARED ${SRC}) # 生成动态库
