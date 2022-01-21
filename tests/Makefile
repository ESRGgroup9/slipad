#==============================================================================
BLUE 	=\033[0;34m
GREEN	=\033[0;32m
CYAN	=\033[0;36m
RESET	=\033[0m

PRINT_GENERATING="${CYAN}Generating $(shell basename $@) ...$(RESET)"
PRINT_BUILDING	="${BLUE}Building $(shell basename $@) ...$(RESET)"
PRINT_COMPILING	="${GREEN}Compiling $(shell basename $@) ...$(RESET)"
#------------------------------------------------------------------------------
BLD_DIR=./build
BIN_DIR=./bin
SRC_DIR=../src
INC_DIR=../inc
#------------------------------------------------------------------------------
CXX 	=arm-buildroot-linux-gnueabihf-g++
LIBS	=-lpthread -lbcm2835 -lrt

DEBUG	=-g #-D DEBUG
INCLDS	=-I $(INC_DIR)
CXXFLAGS=$(INCLDS) -Wall $(LIBS)

#------------------------------------------------------------------------------
SRC= $(wildcard $(SRC_DIR)/*.c*)

OBJS=$(filter %.o,$(patsubst $(SRC_DIR)/%.c,../$(BLD_DIR)/%.o,$(SRC)))
OBJS+=$(filter %.o,$(patsubst $(SRC_DIR)/%.cpp,../$(BLD_DIR)/%.o,$(SRC)))

DEPS= $(patsubst ../$(BLD_DIR)/%.o,../$(BLD_DIR)/%.d,$(OBJS))
#------------------------------------------------------------------------------
PROGS=$(patsubst ./%.c, $(BIN_DIR)/%.elf, $(wildcard ./*.c))
PROGS+=$(patsubst ./%.cpp, $(BIN_DIR)/%.elf, $(wildcard ./*.cpp))
#==============================================================================
vpath %.c $(SRC_DIR) ./
vpath %.cpp $(SRC_DIR) ./

.DEFAULT_GOAL = build
build: setup $(PROGS) ## Compile the binary program

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
TEST=
TARGET=$(BIN_DIR)/$(TEST)

transfer: ## Transfer TARGET=<file> to IP=<ip> into DIR=<dir> directory
	@echo "Transfering:"
	@echo "${GREEN}"$(shell ls $(TARGET))
	@echo "$(RESET)To $(IP) into $(DIR)..."
	@scp $(TARGET) root@$(IP):$(DIR)

setup:
	@mkdir -p $(BLD_DIR)
	@mkdir -p $(BIN_DIR)

clean: ## Delete built artifacts
	@echo "Cleaning ... "
	@rm -rf $(BLD_DIR) $(BIN_DIR)

help: ## Generate list of targets with descriptions
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m %s\n", $$1, $$2}'

.PHONY: build clean transfer help
.PRECIOUS: $(BLD_DIR)/%.o $(BLD_DIR)/%.d