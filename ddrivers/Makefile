#==============================================================================
GREEN	=\033[0;32m
CYAN	=\033[0;36m
RESET	=\033[0m

PRINT_BUILDING="${GREEN}Building $(shell basename $@) ...$(RESET)"
#------------------------------------------------------------------------------
SUBDIRS=pir lampf
#==============================================================================

.DEFAULT_GOAL = build
.PHONY: build all $(SUBDIRS)

build: $(SUBDIRS) ## Compile the binary program
$(SUBDIRS):
	@echo $(PRINT_BUILDING)
	@$(MAKE) -s -C $@

#------------------------------------------------------------------------------
CLEAN_SUBDIRS = $(addprefix clean-,$(SUBDIRS))

.PHONY: clean $(CLEAN_SUBDIRS)
clean: $(CLEAN_SUBDIRS) ## Delete built artifacts
$(CLEAN_SUBDIRS): clean-%:
	@echo "${CYAN}Cleaning $* ...$(RESET)"
	@$(MAKE) -s -C $* clean

#------------------------------------------------------------------------------
# stuff just to print transferring files prettier
.PHONY: print_transfer transfer $(PRINT_TARGET)	$(TARGETS)
TARGETS=$(shell find $(SUBDIRS) -name "*.ko")
PRINT_TARGET=$(addprefix print-, $(wildcard $(TARGETS)))

$(PRINT_TARGET): print-%:
	@echo "$(CYAN)"$*"$(RESET)"

print_transfer:
	@echo "Transfering to $(IP) into $(DIR)..."

transfer: print_transfer $(PRINT_TARGET)
	@scp $(TARGETS) root@$(IP):$(DIR)

#------------------------------------------------------------------------------
.PHONY: help
help: ## Generate list of targets with descriptions
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "$(CYAN)%-15s $(RESET)%s\n", $$1, $$2}'

#==============================================================================