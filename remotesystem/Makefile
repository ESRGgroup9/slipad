#==============================================================================
# Escape colors for printing messages
export BLUE 	=\033[0;34m
export GREEN	=\033[0;32m
export CYAN		=\033[0;36m
export RESET	=\033[0m
# Default print messages
PRINT_GENERATING="${CYAN}Generating $(shell basename $@) ...$(RESET)"
PRINT_BUILDING	="${BLUE}Building $(shell basename $@) ...$(RESET)"
PRINT_COMPILING	="${GREEN}Compiling $(shell basename $@) ...$(RESET)"
#------------------------------------------------------------------------------
# Project directories
BLD_DIR=./build
BIN_DIR=./bin
SRC_DIR=./src
INC_DIR=./inc
#------------------------------------------------------------------------------
# Tests directory
TST_DIR=tests
# Device Drivers directory
DDR_DIR=ddrivers
# Doxygen directory
DOX_DIR=./doc/doxygen
#------------------------------------------------------------------------------
# Export the following variables for use in makefiles called recursively
BLDROOT_OUT=$(HOME)/buildroot/buildroot-2021.02.5/output

# target architecture
export ARCH ?=arm

# path of the kernel build directory
export KDIR :=$(BLDROOT_OUT)/build/linux-custom/

# host/ contains the tools built for the host
# export CROSS_COMPILE:=$(BLDROOT_OUT)/host/bin/arm-buildroot-linux-gnueabihf-
# export CROSS_COMPILE:=arm-buildroot-linux-gnueabihf-

# compiler in use
COMPILE=g++
#------------------------------------------------------------------------------
export CXX 	=$(COMPILE)
export LIBS	=-lpthread -lrt -lmysqlclient

export DEBUG=-D DEBUG #-g
INCLDS		=-I $(INC_DIR)
CXXFLAGS	=$(INCLDS) -Wall $(LIBS) $(DEBUG)
#------------------------------------------------------------------------------
# Variables specifying destination of 'make transfer'
# Destination IP
export IP=10.42.0.254
# Destination directory in IP connection
export DIR=/etc
#------------------------------------------------------------------------------
# Select all source files: *.c and *.cpp files
SRC=$(wildcard $(SRC_DIR)/*.c*)
# Set object files with the name from source file to BLD_DIR/*.o
OBJS=$(filter %.o,$(patsubst $(SRC_DIR)/%.c,$(BLD_DIR)/%.o,$(SRC)))
OBJS+=$(filter %.o,$(patsubst $(SRC_DIR)/%.cpp,$(BLD_DIR)/%.o,$(SRC)))
# Set dependency files with the name from the objects to BLD_DIR/*.d
DEPS=$(patsubst $(BLD_DIR)/%.o,$(BLD_DIR)/%.d,$(OBJS))
#------------------------------------------------------------------------------
# PROGS: find *.cpp in current dir and use their name in format BIN_DIR/*.elf
PROGS= $(patsubst ./%.cpp, $(BIN_DIR)/%.elf, $(wildcard ./*.cpp))
SUBDIRS=$(TST_DIR) $(DDR_DIR)
DOXYFILE=$(DOX_DIR)/Doxyfile
#==============================================================================
# Specify list of directories that make should search for *.c and *.cpp
vpath %.c $(SRC_DIR) .
vpath %.cpp $(SRC_DIR) .
vpath %.h $(INC_DIR)

# Default rule
.DEFAULT_GOAL = build
#------------------------------------------------------------------------------
# Generate dependencies

# -M flag looks at the #include lines in the source files
$(BLD_DIR)/%.d: %.c
	@echo $(PRINT_GENERATING)
	@$(CXX) -M $< -o $@ $(CXXFLAGS)

$(BLD_DIR)/%.d: %.cpp
	@echo $(PRINT_GENERATING)
	@$(CXX) -M $< -o $@ $(CXXFLAGS)

#------------------------------------------------------------------------------
# Build object files

$(BLD_DIR)/%.o: %.c
	@echo $(PRINT_BUILDING)
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BLD_DIR)/%.o: %.cpp
	@echo $(PRINT_BUILDING)
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

#------------------------------------------------------------------------------
# Compile executables

# Add PROG build files .o and .d to the prerequesites
$(PROGS): $(BIN_DIR)/%.elf: $(BLD_DIR)/%.o $(BLD_DIR)/%.d $(DEPS) $(OBJS)
	@echo $(PRINT_COMPILING)
	@$(CXX) -o $@ $< $(OBJS) $(CXXFLAGS)

#------------------------------------------------------------------------------
# Build sub directories
.PHONY: build $(BLD_SUBDIRS)
BLD_SUBDIRS=$(addprefix build-,$(SUBDIRS))

# call 'make build' in subdirectories
# Despite is not shown in make help, user can execute 'make build-<subdir>'
# Ex: $ make build-ddrivers
$(BLD_SUBDIRS): build-%:
	@$(MAKE) -s -C $* build

build: .setup $(PROGS) ## Compile the binary program
build-all: build $(BLD_SUBDIRS) ## Compile all

#------------------------------------------------------------------------------
# Transfer sub directories
TRANSF_SUBDIRS=$(addprefix transfer-,$(SUBDIRS))
.PHONY: $(TRANSF_SUBDIRS)

# call 'make transfer' in subdirectories
# Despite is not shown in make help, user can execute 'make transfer-<subdir>'
# Ex: $ make transfer-ddrivers
$(TRANSF_SUBDIRS): transfer-%:
	@$(MAKE) -s -C $* transfer

# stuff just to make transferring files print prettier
.PHONY: .print_transfer transfer $(PRINT_TARGET)	
TARGET=$(BIN_DIR)/*
PRINT_TARGET=$(addprefix print-, $(wildcard $(TARGET)))

# print targets, one by one, in different lines
$(PRINT_TARGET): print-$(BIN_DIR)/%:
	@echo "$(CYAN)"$*"$(RESET)"

# print "transfering ..." message
.print_transfer:
	@echo "Transfering to $(IP) into $(DIR)..."

# call print rules and do transfer using scp
transfer: .print_transfer $(PRINT_TARGET) ## Transfer executables
	@scp $(TARGET) root@$(IP):$(DIR)

transfer-all: transfer $(TRANSF_SUBDIRS) ## Transfer all

#------------------------------------------------------------------------------
CLEAN_SUBDIRS=$(addprefix clean-,$(SUBDIRS))
.PHONY: clean $(CLEAN_SUBDIRS)

clean: ## Delete main artifacts
	@echo "${CYAN}Cleaning main ... $(RESET)"
	@rm -rf $(BLD_DIR) $(BIN_DIR)

# call 'make clean' in subdirectories
# Despite is not shown in make help, user can execute 'make clean-<subdir>'
# Ex: $ make clean-ddrivers
$(CLEAN_SUBDIRS): clean-%:
	@$(MAKE) -s -C $* clean

clean-all: clean $(CLEAN_SUBDIRS) ## Delete all built artifacts

clean-doc: ## Delete Doxygen built artifacts
	@echo "$(CYAN)Cleaning Doxyfile docs ...$(RESET)"
	@rm -rf $(DOX_DIR)/html $(DOX_DIR)/latex

#------------------------------------------------------------------------------
.PHONY: doc
doc: ## Generate Doxygen documentation
	@echo "$(GREEN)Generating documentation ...$(RESET)"
	@doxygen $(DOXYFILE)

#------------------------------------------------------------------------------
# Make output (bin, build) directories
.setup:
	@mkdir -p $(BLD_DIR)
	@mkdir -p $(BIN_DIR)

help: ## Generate list of targets with descriptions
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "$(CYAN)%-15s $(RESET)%s\n", $$1, $$2}'

.PHONY: .setup help
#------------------------------------------------------------------------------
# On the first pass this should fail, but it should discover how 
# to build these dependencies and then build them and load them in.
# once they are loaded in, it should know how to build the object files.
# -include $(DEPS) # DOESNT WORK
#==============================================================================
