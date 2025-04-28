CC = g++
CCFLAGS = -g -std=c++11
INCLUDES =
# LIBRARIES = -lboost_system -lboost_thread -lpthread -lrt 
LIBRARIES = -lpthread
EXECUTABLES = producer-consumer

# Command for creating an executable file from a .cpp file
pipegrep: main.o boundedBuffer.o pipelineGrepStages.o
	$(CC) $(CCFLAGS) $(INCLUDES) -o pipegrep main.o boundedBuffer.o pipelineGrepStages.o $(LIBRARIES)

# Rule for generating .o file from .cpp file
%.o: %.cpp
	$(CC) $(CCFLAGS) $(INCLUDES) -c $^

# All files to be generated
all: $(EXECUTABLES)

# Clean the directory
clean:
	rm -rf $(EXECUTABLES) *.o