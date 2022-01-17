BUILD_DIR=./build
BIN_DIR=./bin

SRC_DIR=./src
INC_DIR=./inc
TESTS_DIR=./tests

CXX=arm-buildroot-linux-gnueabihf-g++
LIBS=-lpthread -lbcm2835
CXXFLAGS= -Wall -g -I$(INC_DIR) $(LIBS) #-D DEBUG

SRC_OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(wildcard $(SRC_DIR)/*.cpp))

all: localsys tests

# --------------------  Create localsys.elf --------------------
localsys: $(SRC_OBJS) $(BUILD_DIR)/localsys.o | bin
	$(CXX) -o $(BIN_DIR)/localsys.elf $(BUILD_DIR)/localsys.o $(SRC_OBJS) $(CXXFLAGS) 

$(BUILD_DIR)/localsys.o: localsys.cpp | build
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# -----------------------  Create tests ------------------------
tests: testlora testpwm testspi
# TESTS_OBJS=$(patsubst $(TESTS_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(wildcard $(TESTS_DIR)/*.cpp))
# TESTS_TARGETS=$(patsubst $(TESTS_DIR)/%.cpp,$(TESTS_DIR)/%.elf,$(wildcard $(TESTS_DIR)/*.cpp))

TEST_LORA_OBJS=$(addprefix $(BUILD_DIR)/, LoRa.o testlora.o)
testlora: $(TEST_LORA_OBJS) | bin
	$(CXX) -o $(BIN_DIR)/testlora.elf $(TEST_LORA_OBJS) $(CXXFLAGS) 

TEST_PWM_OBJS=$(addprefix $(BUILD_DIR)/, testpwm.o)
testpwm: $(TEST_PWM_OBJS) | bin
	$(CXX) -o $(BIN_DIR)/testpwm.elf $(TEST_PWM_OBJS) $(CXXFLAGS)

TEST_SPI_OBJS=$(addprefix $(BUILD_DIR)/, testspi.o)
testspi: $(TEST_SPI_OBJS) | bin
	$(CXX) -o $(BIN_DIR)/testspi.elf $(TEST_SPI_OBJS) $(CXXFLAGS)

# -----------------------  Create test objs ------------------------
$(BUILD_DIR)/%.o: $(TESTS_DIR)/%.cpp | build
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# ---------------- Create object files from SRC_DIR ------------
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp | build
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# ------------------ Transfer bins to $(IP):$(DIR) -----------------
IP=10.42.0.254
DIR=/etc

# target can be: localsys.elf
transfer:
	scp $(BIN_DIR)/$(TARGET) root@$(IP):$(DIR)

# ---------------------- Create directories --------------------
# make build dir if it doesnt exist
build:
	@mkdir -p $(BUILD_DIR)

# make bin dir if it doesnt exist
bin:
	@mkdir -p $(BIN_DIR)

# --------------------------- Make clean ----------------------
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: bin build clean localsys transfer tests