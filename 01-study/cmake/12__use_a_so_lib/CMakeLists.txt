cmake_minimum_required(VERSION 3.10) # 最小版本
project(main CXX) # 项目名

# # 调用静态库
# include_directories(${PROJECT_SOURCE_DIR}/include)  # 引入头文件
# link_directories(${PROJECT_SOURCE_DIR}/a)
# link_libraries(animal)
# add_executable(app main.cc)

# 调用动态库
include_directories(${PROJECT_SOURCE_DIR}/include)  # 引入头文件
link_directories(${PROJECT_SOURCE_DIR}/so)
add_executable(app main.cc)
target_link_libraries(app PUBLIC animal)

