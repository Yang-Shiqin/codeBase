# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

# Include directories
INCLUDES = -I/usr/local/include

# Libraries
LIBS = -lavformat -lavcodec -lavutil -lswscale -lswresample -lSDL2

# Source files
SRCS = main.cc av_processor.cc av_SDL.cc

# Object files
OBJS = $(SRCS:.cc=.o)

# Executable name
TARGET = BasicAVPlayer

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Compile source files to object files
%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ -g

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)