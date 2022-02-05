#==============================================================================
# Export the following variables for use in makefiles called recursively
BLDROOT_OUT=$(HOME)/buildroot/buildroot-2021.02.5/output

# target architecture
export ARCH ?=arm

# path of the kernel build directory
export KDIR :=$(BLDROOT_OUT)/build/linux-custom/

# host/ contains the tools built for the host
# export CROSS_COMPILE:=$(BLDROOT_OUT)/host/bin/arm-buildroot-linux-gnueabihf-
# export CROSS_COMPILE:=arm-buildroot-linux-gnueabihf-
export CROSS_COMPILE:=arm-linux-

# compiler in use
export COMPILE=g++
#------------------------------------------------------------------------------
# Variables specifying destination of 'make transfer'
# Destination IP
export IP=10.42.0.254
# Destination user
export USR=root
# Destination directory in IP connection
export DIR=/etc/slipad
#------------------------------------------------------------------------------
# Project directory
export PROJ_DIR=$(shell pwd)
#------------------------------------------------------------------------------
# Project source directories
export SRC_PROJ_DIR=$(PROJ_DIR)/src
export INC_PROJ_DIR=$(PROJ_DIR)/inc
#------------------------------------------------------------------------------
SRC_DIR=$(SRC_PROJ_DIR)
INC_DIR=$(INC_PROJ_DIR)

BLD_ARM_DIR=$(PROJ_DIR)/build_arm
BLD_x86_DIR=$(PROJ_DIR)/build_x86
#------------------------------------------------------------------------------
# Tests directory
TST_DIR=tests
# Doxygen directory
DOX_DIR=doc/doxygen
# Local system directory
LS_DIR=localSystem
# Remote system directory
RS_DIR=remoteSystem
# Gateway directory
GAT_DIR=gateway
# Device drivers directory
DDR_DIR=ddrivers
#------------------------------------------------------------------------------
# Identify the subdirectories in order to execute its makefiles
SUBDIRS=$(LS_DIR) $(GAT_DIR) $(RS_DIR) $(DDR_DIR)
# Doxygen configuration file
DOXYFILE=$(DOX_DIR)/Doxyfile
#------------------------------------------------------------------------------
CXX 	=$(CROSS_COMPILE)$(COMPILE)
LIBS	=-lpthread -lbcm2835 -lrt

export DEBUG	=#-D DEBUG #-g
export INCLDS	=-I $(INC_DIR)
CXXFLAGS		=$(INCLDS) -Wall $(LIBS) $(DEBUG)
#------------------------------------------------------------------------------
# Select all source files: *.c and *.cpp files
SRC=$(wildcard $(SRC_DIR)/*.c*)
# Set object files with the name from source file to BLD_DIR/*.o
OBJS=$(filter %_arm.o,$(patsubst $(SRC_DIR)/%.c,$(BLD_ARM_DIR)/%_arm.o,$(SRC)))
OBJS+=$(filter %_arm.o,$(patsubst $(SRC_DIR)/%.cpp,$(BLD_ARM_DIR)/%_arm.o,$(SRC)))
# Set dependency files with the name from the objects to BLD_DIR/*.d
DEPS=$(patsubst $(BLD_ARM_DIR)/%_arm.o,$(BLD_ARM_DIR)/%_arm.d,$(OBJS))
#------------------------------------------------------------------------------
# Select all source files: *.c and *.cpp files
# Select files that need to be compiled in x86
SRC2=$(addprefix $(SRC_DIR)/, CCommunication.cpp CTCPcomm.cpp timer.cpp parser.cpp)
# Set object files with the name from source file to BLD_DIR/*.o
OBJS+=$(filter %_x86.o,$(patsubst $(SRC_DIR)/%.c,$(BLD_x86_DIR)/%_x86.o,$(SRC2)))
OBJS+=$(filter %_x86.o,$(patsubst $(SRC_DIR)/%.cpp,$(BLD_x86_DIR)/%_x86.o,$(SRC2)))

# Set dependency files with the name from the objects to BLD_DIR/*.d
DEPS+=$(patsubst $(BLD_x86_DIR)/%_x86.o,$(BLD_x86_DIR)/%_x86.d,$(OBJS))
# #------------------------------------------------------------------------------
# These will be used in makefiles called from this one
export SRC OBJS DEPS
#------------------------------------------------------------------------------
# Escape colors for printing messages
export BLUE 	=\033[0;34m
export GREEN	=\033[0;32m
export CYAN		=\033[0;36m
export RESET	=\033[0m
# Default print messages
PRINT_GENERATING="${CYAN}Generating $(shell basename $@) ...$(RESET)"
PRINT_BUILDING	="${BLUE}Building $(shell basename $@) ...$(RESET)"
PRINT_COMPILING	="${GREEN}Compiling $(shell basename $@) ...$(RESET)"
#==============================================================================
# Specify list of directories that make should search for *.c and *.cpp
vpath %.c $(SRC_DIR) .
vpath %.cpp $(SRC_DIR) .
vpath %.h $(INC_DIR)

# Default rule
.DEFAULT_GOAL = build-all

#------------------------------------------------------------------------------
# Generate dependencies for arm architecture

# -M flag looks at the #include lines in the source files
$(BLD_ARM_DIR)/%_arm.d: %.c
	@echo $(PRINT_GENERATING)
	@$(CXX) -M $< -o $@ $(CXXFLAGS)

$(BLD_ARM_DIR)/%_arm.d: %.cpp
	@echo $(PRINT_GENERATING)
	@$(CXX) -M $< -o $@ $(CXXFLAGS)

#------------------------------------------------------------------------------
# Build object files for arm architecture

$(BLD_ARM_DIR)/%_arm.o: %.c
	@echo $(PRINT_BUILDING)
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BLD_ARM_DIR)/%_arm.o: %.cpp
	@echo $(PRINT_BUILDING)
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

#------------------------------------------------------------------------------
# Generate dependencies for x86 architecture

# -M flag looks at the #include lines in the source files
$(BLD_x86_DIR)/%_x86.d: %.c
	@echo $(PRINT_GENERATING)
	@$(COMPILE) -M $< -o $@ $(CXXFLAGS)

$(BLD_x86_DIR)/%_x86.d: %.cpp
	@echo $(PRINT_GENERATING)
	@$(COMPILE) -M $< -o $@ $(CXXFLAGS)

#------------------------------------------------------------------------------
# Build object files for x86 architecture

$(BLD_x86_DIR)/%_x86.o: %.c
	@echo $(PRINT_BUILDING)
	@$(COMPILE) -c $< -o $@ $(CXXFLAGS)

$(BLD_x86_DIR)/%_x86.o: %.cpp
	@echo $(PRINT_BUILDING)
	@$(COMPILE) -c $< -o $@ $(CXXFLAGS)

#------------------------------------------------------------------------------
# Build sub directories
.PHONY: build $(BLD_SUBDIRS)

print_build_all: 
	@echo "Making main ..."

build: print_build_all .setup $(DEPS) $(OBJS) ## Build the object files

BLD_SUBDIRS=$(addprefix build-,$(SUBDIRS))
build-all: build $(BLD_SUBDIRS) #build-localsys ## Compile all

# call 'make build' in subdirectories
# Despite is not shown in make help, user can execute 'make build-<subdir>'
# Ex: $ make build-ddrivers
$(BLD_SUBDIRS): build-%:
	@echo "Making $* ..."
	@$(MAKE) -s -C $* build

#------------------------------------------------------------------------------
# Generate tests from sub directories
TST_SUBDIRS=$(addprefix tests-,$(SUBDIRS))
tests-all: $(TST_SUBDIRS) ## Generate all tests
.PHONY: tests-all $(TST_SUBDIRS)

# call 'make build-tests' in subdirectories
# Despite is not shown in make help, user can execute 'make tests-<subdir>'
# Ex: $ make tests-remoteSystem
$(TST_SUBDIRS): tests-%:
	@echo "Making $* ..."
	@$(MAKE) -s -C $* build-tests

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
# transfer: .print_transfer $(PRINT_TARGET) ## Transfer executables
# 	@scp $(TARGET) root@$(IP):$(DIR)

# transfer-all: transfer $(TRANSF_SUBDIRS) ## Transfer all
transfer: $(TRANSF_SUBDIRS) ## Transfer all

#------------------------------------------------------------------------------
CLEAN_SUBDIRS=$(addprefix clean-,$(SUBDIRS))
.PHONY: clean $(CLEAN_SUBDIRS)

clean: ## Delete main artifacts
	@echo "${CYAN}Cleaning main ... $(RESET)"
	@rm -rf $(BLD_ARM_DIR) $(BLD_x86_DIR)

# call 'make clean' in subdirectories
# Despite is not shown in make help, user can execute 'make clean-<subdir>'
# Ex: $ make clean-ddrivers
$(CLEAN_SUBDIRS): clean-%:
	@$(MAKE) -s -C $* clean

clean-localsys:
	@echo "${CYAN}Cleaning localSystem ... $(RESET)"
	@rm -rf $(LS_DIR)/CMakeFiles $(LS_DIR)/CMakeCache.txt $(LS_DIR)/cmake_install.cmake $(LS_DIR)/bin
	
clean-all: clean $(CLEAN_SUBDIRS) clean-localsys ## Delete all built artifacts

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
	@mkdir -p $(BLD_ARM_DIR)
	@mkdir -p $(BLD_x86_DIR)

help: ## Generate list of targets with descriptions
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "$(CYAN)%-15s $(RESET)%s\n", $$1, $$2}'

.PHONY: .setup help
#------------------------------------------------------------------------------
# On the first pass this should fail, but it should discover how 
# to build these dependencies and then build them and load them in.
# once they are loaded in, it should know how to build the object files.
# -include $(DEPS) # DOESNT WORK
#==============================================================================
