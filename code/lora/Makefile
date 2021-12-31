# CC=g++
CC=arm-buildroot-linux-gnueabihf-g++

CFLAGS= -Wall -g -I. -lbcm2835

DEPS=LoRa.h
OBJS=LoRa.o test.o
TARGET=loratest.elf

IP=10.42.0.254
DIR=/etc

all: $(TARGET)

$(TARGET) : $(OBJS) 
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c  $< -o $@ $(CFLAGS)

transfer:
	scp $(TARGET) root@$(IP):$(DIR)

.PHONY: clean transfer
clean:
	rm -rf $(TARGET) *.o *~
