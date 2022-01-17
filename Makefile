BUILD_DIR=./build
BIN_DIR=./bin

SRC_DIR=./src
INC_DIR=./inc
TESTS_DIR=./tests

CXX=arm-buildroot-linux-gnueabihf-g++
LIBS=-lpthread -lbcm2835 -lrt
CXXFLAGS= -Wall -g -I$(INC_DIR) $(LIBS) -D DEBUG

SRC_OBJS=$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(wildcard $(SRC_DIR)/*.c))
SRC_OBJS+=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(wildcard $(SRC_DIR)/*.cpp))

# --------------------  Create localsys.elf --------------------
localsys: $(SRC_OBJS) $(BUILD_DIR)/localsys.o | bin ## Compile local system main process
	$(CXX) -o $(BIN_DIR)/localsys.elf $(BUILD_DIR)/localsys.o $(SRC_OBJS) $(CXXFLAGS)

$(BUILD_DIR)/localsys.o: localsys.cpp | build
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# -----------------------  Create tests ------------------------
test: test-lora test-pwm test-spi ## Compile all tests
# TESTS_OBJS=$(patsubst $(TESTS_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(wildcard $(TESTS_DIR)/*.cpp))
# TESTS_TARGETS=$(patsubst $(TESTS_DIR)/%.cpp,$(TESTS_DIR)/%.elf,$(wildcard $(TESTS_DIR)/*.cpp))

TEST_LORA_OBJS=$(addprefix $(BUILD_DIR)/, LoRa.o testlora.o)
test-lora: $(TEST_LORA_OBJS) | bin ## Compile LoRa test
	$(CXX) -o $(BIN_DIR)/testlora.elf $(TEST_LORA_OBJS) $(CXXFLAGS) 

TEST_PWM_OBJS=$(addprefix $(BUILD_DIR)/, testpwm.o)
test-pwm: $(TEST_PWM_OBJS) | bin ## Compile PWM test
	$(CXX) -o $(BIN_DIR)/testpwm.elf $(TEST_PWM_OBJS) $(CXXFLAGS)

TEST_SPI_OBJS=$(addprefix $(BUILD_DIR)/, testspi.o)
test-spi: $(TEST_SPI_OBJS) | bin ## Compile SPI test
	$(CXX) -o $(BIN_DIR)/testspi.elf $(TEST_SPI_OBJS) $(CXXFLAGS)

TEST_TSL_OBJS=$(addprefix $(BUILD_DIR)/, testtsl.o DEV_Config.o TSL2581.o)
test-tsl: $(TEST_TSL_OBJS) | bin ## Compile TSL test
	$(CXX) -o $(BIN_DIR)/testtsl.elf $(TEST_TSL_OBJS) $(CXXFLAGS)

# -----------------------  Create test objs ------------------------
# ---- cpp objects -----
$(BUILD_DIR)/%.o: $(TESTS_DIR)/%.cpp | build
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# ---- cpp objects -----
$(BUILD_DIR)/%.o: $(TESTS_DIR)/%.c | build
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# ---------------- Create object files from SRC_DIR ------------
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c* | build
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# ------------------ Transfer bins to $(IP):$(DIR) -----------------
IP=10.42.0.254
DIR=/etc

# target can be: localsys.elf
transfer: ## Transfer TARGET=<file> to IP=<ip> into DIR=<dir> directory
	scp $(BIN_DIR)/$(TARGET) root@$(IP):$(DIR)

# ----------------------------- Others -----------------------------
all: localsys test ## Compile all

# make build dir if it doesnt exist
build:
	@mkdir -p $(BUILD_DIR)

# make bin dir if it doesnt exist
bin:
	@mkdir -p $(BIN_DIR)

clean: ## Remove /build and /bin directories
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# help: display all ##comments that are in front of rules
help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m %s\n", $$1, $$2}'

.PHONY: bin build clean localsys transfer tests help