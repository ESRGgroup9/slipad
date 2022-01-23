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
SRC_DIR=./src
INC_DIR=./inc
#------------------------------------------------------------------------------
TST_DIR=tests
DDR_DIR=ddrivers
#------------------------------------------------------------------------------
CXX 	=arm-buildroot-linux-gnueabihf-g++
LIBS	=-lpthread -lbcm2835 -lrt

DEBUG	=-D DEBUG #-g
INCLDS	=-I $(INC_DIR)
CXXFLAGS=$(INCLDS) -Wall $(LIBS) $(DEBUG)
#------------------------------------------------------------------------------
SRC= $(wildcard $(SRC_DIR)/*.c*)

OBJS=$(filter %.o,$(patsubst $(SRC_DIR)/%.c,$(BLD_DIR)/%.o,$(SRC)))
OBJS+=$(filter %.o,$(patsubst $(SRC_DIR)/%.cpp,$(BLD_DIR)/%.o,$(SRC)))

DEPS= $(patsubst $(BLD_DIR)/%.o,$(BLD_DIR)/%.d,$(OBJS))
#------------------------------------------------------------------------------
PROGS=$(addprefix $(BIN_DIR)/, localsys.elf dSensors.elf)
SUBDIRS=$(TST_DIR) $(DDR_DIR)
#==============================================================================
vpath %.c $(SRC_DIR) ./
vpath %.cpp $(SRC_DIR) ./

.DEFAULT_GOAL = build
.PHONY: build
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
.PHONY: $(PROGS)
$(PROGS): $(BIN_DIR)/%.elf: $(BLD_DIR)/%.o $(BLD_DIR)/%.d $(DEPS) $(OBJS)
	@echo $(PRINT_COMPILING)
	@$(CXX) -o $@ $< $(OBJS) $(CXXFLAGS)

#------------------------------------------------------------------------------
BLD_SUBDIRS=$(addprefix build-,$(SUBDIRS))
.PHONY: $(BLD_SUBDIRS)
$(BLD_SUBDIRS): build-%:
	@$(MAKE) -s -C $* build

#------------------------------------------------------------------------------
CLEAN_SUBDIRS=$(addprefix clean-,$(SUBDIRS))
.PHONY: clean $(CLEAN_SUBDIRS)

clean: ## Delete built artifacts
	@echo "${CYAN}Cleaning ... $(RESET)"
	@rm -rf $(BLD_DIR) $(BIN_DIR)

$(CLEAN_SUBDIRS): clean-%:
	@$(MAKE) -s -C $* clean

#------------------------------------------------------------------------------
IP=10.42.0.254
DIR=/etc
TARGET=$(BIN_DIR)/*

TRANSF_SUBDIRS=$(addprefix transfer-,$(SUBDIRS))
.PHONY: transfer $(TRANSF_SUBDIRS)

transfer: ## Transfer TARGET=<file> to IP=<ip> into DIR=<dir> directory
	@echo "Transfering:"
	@echo "${GREEN}"$(shell ls $(TARGET))
	@echo "$(RESET)To $(IP) into $(DIR)..."
	@scp $(TARGET) root@$(IP):$(DIR)

$(TRANSF_SUBDIRS): transfer-%: ## transfer objs
	@$(MAKE) -s -C $* transfer

#------------------------------------------------------------------------------
setup:
	@mkdir -p $(BLD_DIR)
	@mkdir -p $(BIN_DIR)

help: ## Generate list of targets with descriptions
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "$(CYAN)%-15s $(RESET)%s\n", $$1, $$2}'

.PHONY: build help
# .PRECIOUS: $(BLD_DIR)/%.o $(BLD_DIR)/%.d
#==============================================================================