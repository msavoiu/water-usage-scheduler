# compiler
CXX = clang++

# compiler flags
CXXFLAGS = -std=c++17 -I/opt/homebrew/include

# target executable
TARGET = main
TARGET_DEL = main.exe

# source files
SRCS = main.cpp classes/appliance.cpp classes/household.cpp classes/scheduler.cpp classes/task.cpp classes/water_system.cpp

# object files
OBJS = $(SRCS:.cpp=.o)

# default rule to build and run the executable
all: $(TARGET) run

# rule to link object files into target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# rule to run the executable
run: $(TARGET)
	./$(TARGET)

# clean rule to remove generated files
clean:
	rm -f $(TARGET) $(OBJS)

# clang++ main.cpp -std=c++17 -I/opt/homebrew/include