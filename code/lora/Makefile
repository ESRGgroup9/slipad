CC=g++
#CC=arm-buildroot-linux-gnueabihf-g++

CFLAGS= -g -I.

DEPS=LoRa.h
OBJS=LoRa.cpp test.cpp
TARGET=loratest

all: $(TARGET)
$(TARGET) : $(OBJS)
	$(CC) -pthread -o $(TARGET) $(OBJS)

%.o: %.cpp $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(TARGET) *.o *~
