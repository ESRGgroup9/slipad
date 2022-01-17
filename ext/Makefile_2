# CC=g++
CC=arm-buildroot-linux-gnueabihf-g++

CFLAGS= -Wall -g -I. -lbcm2835

DEPS=CLdr.h DEV_Config.h TSL2581.h
OBJS=CLdr.o DEV_Config.o TSL2581.o main.o
TARGET=main.elf

DIR_BIN=./bin

IP=10.42.0.30
DIR=/etc

all: $(TARGET)

$(TARGET) : $(OBJS) 
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

#${DIR_BIN}/%.o: %.cpp $(DEPS)
%.o: %.cpp $(DEPS)
	$(CC) -c  $< -o $@ $(CFLAGS)

transfer:
	scp $(TARGET) root@$(IP):$(DIR)

.PHONY: clean transfer
clean:
	rm -rf $(TARGET) *.o *~