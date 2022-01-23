#==============================================================================
# Escape colors for printing messages
BLUE 	=\033[0;34m
GREEN	=\033[0;32m
CYAN	=\033[0;36m
RESET	=\033[0m
# Default print messages
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
# Device Drivers directory
DDR_DIR=ddrivers
# Doxxygen directory
DOX_DIR=./doc/doxygen
#------------------------------------------------------------------------------
CXX 	=arm-buildroot-linux-gnueabihf-g++
LIBS	=-lpthread -lbcm2835 -lrt

DEBUG	=-D DEBUG #-g
INCLDS	=-I $(INC_DIR)
CXXFLAGS=$(INCLDS) -Wall $(LIBS) $(DEBUG)
#------------------------------------------------------------------------------
# Select all source files: *.c and *.cpp files
SRC= $(wildcard $(SRC_DIR)/*.c*)
# Set object files with the name from source file to BLD_DIR/*.o
OBJS=$(filter %.o,$(patsubst $(SRC_DIR)/%.c,$(BLD_DIR)/%.o,$(SRC)))
OBJS+=$(filter %.o,$(patsubst $(SRC_DIR)/%.cpp,$(BLD_DIR)/%.o,$(SRC)))
# Set dependency files with the name from the objects to BLD_DIR/*.d
DEPS= $(patsubst $(BLD_DIR)/%.o,$(BLD_DIR)/%.d,$(OBJS))
#------------------------------------------------------------------------------
# PROGS: find *.cpp in current dir and use their name in format BIN_DIR/*.elf
PROGS= $(patsubst ./%.cpp, $(BIN_DIR)/%.elf, $(wildcard ./*.cpp))
SUBDIRS=$(TST_DIR) $(DDR_DIR)
DOXYFILE=$(DOX_DIR)/Doxyfile
#==============================================================================
vpath %.c $(SRC_DIR) ./
vpath %.cpp $(SRC_DIR) ./

.DEFAULT_GOAL = build
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
BLD_SUBDIRS=$(addprefix build-,$(SUBDIRS))

$(BLD_SUBDIRS): build-%:
	@$(MAKE) -s -C $* build

.PHONY: build $(BLD_SUBDIRS)
build: .setup $(PROGS) ## Compile the binary program
build-all: build $(BLD_SUBDIRS) ## Compile all

#------------------------------------------------------------------------------
IP=10.42.0.254
DIR=/etc
# Export this variables for use in makefiles called recursively
export IP
export DIR

TRANSF_SUBDIRS=$(addprefix transfer-,$(SUBDIRS))
.PHONY: $(TRANSF_SUBDIRS)

# call 'make transfer' in subdirectories
$(TRANSF_SUBDIRS): transfer-%:
	@$(MAKE) -s -C $* transfer $(IP) $(DIR)

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
	@echo "${CYAN}Cleaning ... $(RESET)"
	@rm -rf $(BLD_DIR) $(BIN_DIR)

# call 'make clean' in subdirectories
$(CLEAN_SUBDIRS): clean-%:
	@$(MAKE) -s -C $* clean

clean-all: clean $(CLEAN_SUBDIRS) ## Delete all built artifacts

clean-doc: ## Delete Doxygen built artifacts
	@echo "$(CYAN)Cleaning Doxyfile docs ...$(RESET)"
	@rm -rf $(DOX_DIR)/html $(DOX_DIR)/latex $(DOX_DIR)/index.html

#------------------------------------------------------------------------------
.PHONY: doc
doc: ## Generate Doxygen documentation
	@echo "$(GREEN)Generating documentation ...$(RESET)"
	@doxygen $(DOXYFILE)
	@cp $(DOX_DIR)/html/index.html $(DOX_DIR)/
	@echo "$(CYAN)Copy of generated index.html in $(DOX_DIR)."

#------------------------------------------------------------------------------
.setup:
	@mkdir -p $(BLD_DIR)
	@mkdir -p $(BIN_DIR)

help: ## Generate list of targets with descriptions
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "$(CYAN)%-15s $(RESET)%s\n", $$1, $$2}'

.PHONY: build help
# .PRECIOUS: $(BLD_DIR)/%.o $(BLD_DIR)/%.d
#==============================================================================