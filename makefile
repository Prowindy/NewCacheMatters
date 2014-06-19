CC = g++
CFLAGS = -Wall -O -g -std=c++0x

SUBDIR=$(shell ls -d */)
SOURCES=$(shell find $(SUBDIR) -name '*.cpp')


OBJS = $(patsubst %.cpp, %.o, $(patsubst %.cpp, %.o, $(SOURCES)))

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
TARGET = ./main_func

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
	chmod a+x $(TARGET)

clean:
	rm -rf *.o main_func
