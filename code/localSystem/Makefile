# CC=g++
CC=arm-buildroot-linux-gnueabihf-g++

LIBS=-lpthread -lbcm2835
CFLAGS= -Wall -g -I. $(LIBS)

DEPS=CCommunication.h CLoraComm.h utils.h ../lora/LoRa.h
OBJS=CCommunication.o main.o CLoraComm.o ../lora/LoRa.o
TARGET=main.elf

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