# CC=g++
CC=arm-buildroot-linux-gnueabihf-g++

CFLAGS= -Wall -g -I. -pthread -lbcm2835

DEPS=LoRa.h
OBJS=LoRa.o test.o
TARGET=loratest.elf

all: $(TARGET)

$(TARGET) : $(OBJS) 
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c  $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(TARGET) *.o *~
