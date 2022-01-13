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

##################################### Makefile ###############################
# a Makefile works accordingly to the following syntax:
#
# rule: dependencies
# 	<TAB>	comand1
# 	<TAB>	comand2
#
# all: the name of the rules to be executed
# teste.o: teste.c (can be interpreted as destination_file: origin_file)
# clean: deletes the intermediary files. If one writes in the terminal <make
# clean> the object files are deleted.
# mrproper: deletes everything that must be modified
#
# ----- Defining variables -----------------------------------
#  VAR_NAME=VAL
#  and to use the variable we placed between $(): $(VAR_NAME) 
#
#	#Using wildcards to help define variables
#	SRC=$(wildcard *.c)
#	OBJ=$(SRC:.c=.o)
# -------------------------------------------------------------
# 
# #Silent Mode (no echo in the command line)
# add @ to the beginning of the command. Ex:
# 				@$(CC) -o $@ $^
#
# ------------------- Internal Variables ---------------------
# $@	Name of the rule
# $<	Name of the 1st dependency
# $^	List of dependencies
# $?	List of dependencies more recent than the rule
# $*	Name of the file with suffix
# ------------------------------------------------------------
# 
#	Rules without dependencies, e.g., actions like clean:, must be called
#	explicitly, otherwise they wont be executed.
#			make clean #will remove all objects files from folder, as described below
#
# -------------------- Interference rules -------------------
#  Generic rules called by default
#  - .c.o: make a .o file from a .c file
#  - %o: %.c: the same thing. The line teste.o:teste.c can be modified with this
#  rule
#  - .PHONY: This rule avoids conflicts.
#  			- If one has a clean file in the directory, nothing will happen when one
#  			executes make clean
#  			- So .PHONY says that clean and mrproper should be executed even if
#  			files with that name exist current path
# ----------------------------------------------------------------
# 
# ------------------------- make install -------------------------
#  Automating the program's installation with the install: rule
#   - install: Places the binary or executable inside a given folder, e.g., /bin
#   or /usr/bin in Linux. It can be any other, using the command mv or cp to
#   move or copy
#   - Lets create 2 vars:
#   	- prefix=path/to/proj
#   	- bindir=$(prefix)/bin
#   	- and we add the rule install:all

# PROJ (used to be test)
PROJ=slipad

# File Paths
SRC_DIR=../src
BIN_DIR=${SRC_DIR}/bin

#LIBS=-lform -lncurses
CFLAGS=-Wall #-W -ansi -pedantic # options passed to the compiler
RM=rm -rf

# documentation
# DOXYGEN=/usr/local/bin/doxygen
# DOXYFILE=../Doxyfile

# Databases
# DB = $(wildcard *.db)

SRC := $(wildcard *.c)
CC:=arm-buildroot-linux-gnueabihf-g++

# Debub settings
DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CFLAGS+=-g
else
    CFLAGS+=-DNDEBUG
endif

# CC=gcc #defining the compiler name for C or C++ (here its gcc) // bad practice
# to define it (not portable)
OBJ := $(SRC:.c=.o)

# Building executable (w/ linking)
$(PROJ): $(OBJ)
# the compiler does the linking between the 2 objects
# gcc -o teste teste.o main.o
# $@ = teste:						#Rule name
# $^ = teste.o main.o		#dependency list
	@echo "Creating executable"
	$(CC) -o $@ $^ ${LIBS}

# Building object files from sources
%.o: ${SRC_DIR}/%.c # make a .o file from a .c file
# gcc -o teste.o -c teste.c -W -Wall -ansi -pedantic
# $< Name of the 1st dependency
	@echo "Building object files"
	$(CC) -o $@ -c $^ $(CFLAGS)

all: $(PROJ)
	@echo "Compiling project"

# Install: run make and then make install
install: all clean
	@echo "Installing binaries"
	@mkdir -p $(BIN_DIR)
	@mv $(PROJ) $(BIN_DIR)/
	@mv $(BIN_DIR) ../


.PHONY: clean mrproper clean-all doc pu-seq
clean-all: clean mrproper
clean: 
# @- $(RM) *.o # this does not work	
#	@- $(RM) $(wildcard *.o) # this does not work
#	this works:
	 @- $(RM) $(OBJ)
#	 @- $(RM) $(DB)
mrproper: clean
	@$(RM) $(PROJ)
# Documentation
# doc:
# 	@echo "Generating documentation"
# 	@$(DOXYGEN) $(DOXYFILE)
