#==============================================================================
BLUE=`tput setaf 4`
GREEN=`tput setaf 2`
CYAN=`tput setaf 6`

PRINT_GENERATING="${CYAN}Generating $(shell basename $@) ... "
PRINT_BUILDING	="${BLUE}Building $(shell basename $@) ... "
PRINT_COMPILING	="${GREEN}Compiling $(shell basename $@) ... "
#------------------------------------------------------------------------------
BLD_DIR=./build
BIN_DIR=./bin
SRC_DIR=./src
INC_DIR=./inc
TST_DIR=./tests
#------------------------------------------------------------------------------
CXX 	=arm-buildroot-linux-gnueabihf-g++
LIBS	=-lpthread -lbcm2835 -lrt

DEBUG	=-g #-D DEBUG
INCLDS	=-I $(INC_DIR)
CXXFLAGS=$(INCLDS) -Wall $(LIBS)

#------------------------------------------------------------------------------
SRC= $(wildcard $(SRC_DIR)/*.c*)

OBJS=$(filter %.o,$(patsubst $(SRC_DIR)/%.c,$(BLD_DIR)/%.o,$(SRC)))
OBJS+=$(filter %.o,$(patsubst $(SRC_DIR)/%.cpp,$(BLD_DIR)/%.o,$(SRC)))

DEPS= $(patsubst $(BLD_DIR)/%.o,$(BLD_DIR)/%.d,$(OBJS))
#------------------------------------------------------------------------------
PROGS=$(addprefix $(BIN_DIR)/, localsys.elf dSensors.elf)
TESTS=$(patsubst $(BIN_DIR)/%.elf,$(TST_DIR)/%.c,$(OBJS))
#==============================================================================

vpath %.c $(SRC_DIR) ./
vpath %.cpp $(SRC_DIR) ./

.DEFAULT_GOAL = build
build: setup $(PROGS) ## Compile the binary program

tests:
	$(MAKE) -C $(TST_DIR)
#------------------------------------------------------------------------------
# Create dependencies

$(BLD_DIR)/%.d: %.c
	@echo $(PRINT_GENERATING)
	@$(CXX) -M $< -o $@ $(CXXFLAGS)

$(BLD_DIR)/%.d: %.cpp
	@echo $(PRINT_GENERATING)
	@$(CXX) -M $< -o $@ $(CXXFLAGS)

#------------------------------------------------------------------------------
# Create object files

$(BLD_DIR)/%.o: %.c
	@echo $(PRINT_BUILDING)
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BLD_DIR)/%.o: %.cpp
	@echo $(PRINT_BUILDING)
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

#------------------------------------------------------------------------------
# Create executables

$(PROGS): $(BIN_DIR)/%.elf: $(BLD_DIR)/%.o $(BLD_DIR)/%.d $(DEPS) $(OBJS)
	@echo $(PRINT_COMPILING)
	@$(CXX) -o $@ $< $(OBJS) $(CXXFLAGS)

#------------------------------------------------------------------------------
IP=10.42.0.254
DIR=/etc
TARGET=*

transfer: ## Transfer TARGET=<file> to IP=<ip> into DIR=<dir> directory
	@echo "Transfering:"
	@echo $(shell ls $(BIN_DIR)/$(TARGET))
	@echo "To $(IP) into $(DIR)..."
	@scp $(BIN_DIR)/$(TARGET) root@$(IP):$(DIR)

setup:
	@mkdir -p $(BLD_DIR)
	@mkdir -p $(BIN_DIR)

clean: ## Delete built artifacts
	@echo "Cleaning ... "
	@rm -rf $(BLD_DIR) $(BIN_DIR)

help: ## Generate list of targets with descriptions
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m %s\n", $$1, $$2}'

.PHONY: build clean transfer help tests
.PRECIOUS: $(BLD_DIR)/%.o $(BLD_DIR)/%.d