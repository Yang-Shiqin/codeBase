# 编译Linux版本(已删除)：make 或 make hls_server_linux 或 make linux
# 编译Windows版本：make hls_server_win 或 make win

# Compiler settings
CXX = g++
CXXFLAGS = -Wall -std=c++11 -I.

# Output binaries
WIN_TARGET = hls_server_win
LINUX_TARGET = hls_server_linux

# Directories
UTILS_DIR = Utils

# Source files
WIN_SRC = main.cpp Connection.cpp
LINUX_SRC = main_linux.cpp	# 因为源码不是实现和win版本一样的功能, 感觉是收流, 所以删除
UTILS_HEADERS = $(UTILS_DIR)/Log.h $(UTILS_DIR)/Utils.h

# Include directories
INCLUDES = -I$(UTILS_DIR)

# Windows specific flags and libs
WIN_LIBS = -lws2_32
WIN_FLAGS = -DWIN32

# Default target
all: $(WIN_TARGET)

win: $(WIN_TARGET)

linux: $(LINUX_TARGET)

# Windows target
$(WIN_TARGET): $(WIN_SRC) $(UTILS_HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(WIN_FLAGS) -o $@ $(WIN_SRC) $(WIN_LIBS)

# Linux target  
$(LINUX_TARGET): $(LINUX_SRC) $(UTILS_HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(LINUX_SRC)

# Clean build files
clean:
	rm -f $(WIN_TARGET) $(LINUX_TARGET)

.PHONY: all win linux clean
