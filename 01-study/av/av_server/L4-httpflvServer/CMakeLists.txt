cmake_minimum_required(VERSION 3.10.2)
project(L4-httpflvServer)

# 设置 C++11 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# 调试和发布模式的编译选项
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 查找 jsoncpp 库
find_package(jsoncpp REQUIRED)
if (jsoncpp_FOUND)
	include_directories(/usr/include/jsoncpp) # jsoncpp 头文件路径
	link_directories(/usr/lib/x86_64-linux-gnu) # jsoncpp 库路径
    # include_directories(${jsoncpp_INCLUDE_DIRS})
	# link_directories(${jsoncpp_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "jsoncpp not found")
endif()

# 查找 Boost 库，包含 system 组件
find_package(Boost REQUIRED COMPONENTS system)

# 设置 Boost 头文件和库路径
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# 设置本地的 include 和 library 目录（如果有本地的依赖）
set(INCLUDE_DIR /usr/local/include)
set(LIB_DIR /usr/local/lib)
include_directories(${INCLUDE_DIR})
link_directories(${LIB_DIR})

# 定义可执行文件及其源文件
add_executable(L4-httpflvServer
        main.cpp
		Scheduler.cpp
        Utils/Config.cpp
        Server/BoostServer.cpp
        Server/HttpServerConnection.cpp
)

# 链接库，包括 Boost::system、pthread 和其他依赖
target_link_libraries(L4-httpflvServer ${Boost_LIBRARIES} pthread jsoncpp)
