#==============================================================================
BLD_DIR=./build
BIN_DIR=./bin
SRC_DIR=./src
INC_DIR=./inc
TST_DIR=./tests
#------------------------------------------------------------------------------
CXX=arm-buildroot-linux-gnueabihf-g++
LIBS=-lpthread -lbcm2835 -lrt
CXXFLAGS= -Wall -g -I$(INC_DIR) $(LIBS) -D DEBUG
#------------------------------------------------------------------------------
HDRS=$(wildcard $(INC_DIR)/*.h)
SRC_OBJS=$(patsubst $(SRC_DIR)/%.c,$(BLD_DIR)/%.o,$(wildcard $(SRC_DIR)/*.c))
SRC_OBJS+=$(patsubst $(SRC_DIR)/%.cpp,$(BLD_DIR)/%.o,$(wildcard $(SRC_DIR)/*.cpp))
#------------------------------------------------------------------------------
BLUE=`tput setaf 4`
GREEN=`tput setaf 2`
CYAN=`tput setaf 6`

PRINT_GENERATING="${CYAN}Generating $(shell basename $@.elf) ... "
PRINT_BUILDING="${BLUE}Building $(shell basename $@) ... "

#==============================================================================

# --------------------  Create localsys.elf --------------------
localsys: $(SRC_OBJS) $(BLD_DIR)/localsys.o | bin ## Compile local system main process
	@echo $(PRINT_GENERATING)
	@$(CXX) -o $(BIN_DIR)/localsys.elf $(BLD_DIR)/localsys.o $(SRC_OBJS) $(CXXFLAGS)

# --------------------  Create dSensors.elf --------------------
dsensors: $(SRC_OBJS) $(BLD_DIR)/dSensors.o | bin ## Compile local system daemon
	@echo $(PRINT_GENERATING)
	@$(CXX) -o $(BIN_DIR)/dSensors.elf $(BLD_DIR)/dSensors.o $(SRC_OBJS) $(CXXFLAGS)

# -----------------------  Create tests ------------------------
test: test-lora test-pwm test-spi test-timer## Compile all tests
# TESTS_OBJS=$(patsubst $(TESTS_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(wildcard $(TESTS_DIR)/*.cpp))
# TESTS_TARGETS=$(patsubst $(TESTS_DIR)/%.cpp,$(TESTS_DIR)/%.elf,$(wildcard $(TESTS_DIR)/*.cpp))

TEST_LORA_OBJS=$(addprefix $(BLD_DIR)/, LoRa.o testlora.o)
test-lora: $(TEST_LORA_OBJS) | bin ## Compile LoRa test
	@echo $(PRINT_GENERATING)
	@$(CXX) -o $(BIN_DIR)/testlora.elf $(TEST_LORA_OBJS) $(CXXFLAGS) 

TEST_PWM_OBJS=$(addprefix $(BLD_DIR)/, testpwm.o)
test-pwm: $(TEST_PWM_OBJS) | bin ## Compile PWM test
	@echo $(PRINT_GENERATING)
	@$(CXX) -o $(BIN_DIR)/testpwm.elf $(TEST_PWM_OBJS) $(CXXFLAGS)

TEST_SPI_OBJS=$(addprefix $(BLD_DIR)/, testspi.o)
test-spi: $(TEST_SPI_OBJS) | bin ## Compile SPI test
	@echo $(PRINT_GENERATING)
	@$(CXX) -o $(BIN_DIR)/testspi.elf $(TEST_SPI_OBJS) $(CXXFLAGS)

TEST_TSL_OBJS=$(addprefix $(BLD_DIR)/, testtsl.o DEV_Config.o TSL2581.o)
test-tsl: $(TEST_TSL_OBJS) | bin ## Compile TSL test
	@echo $(PRINT_GENERATING)
	@$(CXX) -o $(BIN_DIR)/testtsl.elf $(TEST_TSL_OBJS) $(CXXFLAGS)

TEST_TIM_OBJS=$(addprefix $(BLD_DIR)/, timer.o testtimer.o)
test-timer: $(TEST_TIM_OBJS) | bin ## Compile timer test
	@echo $(PRINT_GENERATING)
	@$(CXX) -o $(BIN_DIR)/testtimer.elf $(TEST_TIM_OBJS) $(CXXFLAGS)

TEST_PIR_OBJS=$(addprefix $(BLD_DIR)/, testpir.o CPir.o)
test-pir: $(TEST_PIR_OBJS) | bin ## Compile pir test
	@echo $(PRINT_GENERATING)
	@$(CXX) -o $(BIN_DIR)/testpir.elf $(TEST_PIR_OBJS) $(CXXFLAGS)

TEST_LAMPF_OBJS=$(addprefix $(BLD_DIR)/, testlampf.o CFailureDetector.o)
test-lampf: $(TEST_LAMPF_OBJS) | bin ## Compile failure detector test
	@echo $(PRINT_GENERATING)
	@$(CXX) -o $(BIN_DIR)/testlampf.elf $(TEST_LAMPF_OBJS) $(CXXFLAGS)

# -----------------------  Create test objs ------------------------
# ---- cpp objects -----
$(BLD_DIR)/%.o: $(TST_DIR)/%.cpp | build
	@echo $(PRINT_BUILDING)
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

# ---- c objects -----
$(BLD_DIR)/%.o: $(TST_DIR)/%.c | build
	@echo $(PRINT_BUILDING)
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

# ---------------- Create object files from SRC_DIR ------------
$(BLD_DIR)/%.o : $(SRC_DIR)/%.c* | build
	@echo $(PRINT_BUILDING)
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

# -------------------- Create main object files  ------------
$(BLD_DIR)/%.o: %.cpp | build
	@echo $(PRINT_BUILDING)
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

# ------------------ Transfer bins to $(IP):$(DIR) -----------------
IP=10.42.0.254
DIR=/etc
TARGET=*

transfer: # Transfer TARGET=<file> to IP=<ip> into DIR=<dir> directory
	@echo "Transfering:"
	@echo $(shell ls $(BIN_DIR)/$(TARGET))
	@echo "To $(IP) into $(DIR)..."
	@scp $(BIN_DIR)/$(TARGET) root@$(IP):$(DIR)

# ----------------------------- Others -----------------------------
all: localsys dsensors## Compile all

# make build dir if it doesnt exist
build:
	@mkdir -p $(BLD_DIR)

# make bin dir if it doesnt exist
bin:
	@mkdir -p $(BIN_DIR)

clean: ## Remove /build and /bin directories
	@echo "Cleaning ... "
	@rm -rf $(BLD_DIR) $(BIN_DIR)

# help: display all ##comments that are in front of rules
help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m %s\n", $$1, $$2}'

.PHONY: bin build clean localsys transfer tests help