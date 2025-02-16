# Makefile for compiling the 2SAT program

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

# Source files
SRCS = main_x_rays.cpp \
       modules/find_first_char.cpp \
       modules/make_clauses.cpp \
       modules/Two_SAT_Algorithm.cpp

# Output executable
TARGET = 2sat

# Default rule to build the target
all: $(TARGET)

# Rule to link the object files and create the executable
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Run rule to execute the program after building
run: $(TARGET)
	./$(TARGET)
# Clean rule to remove the executable and any object files
clean:
	rm -f $(TARGET) *.o

# Phony targets
.PHONY: all clean
