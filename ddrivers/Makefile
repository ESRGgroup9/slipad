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
IP=10.42.0.254
DIR=/etc
TARGETS=$(shell find $(SUBDIRS) -name "*.ko")

.PHONY: transfer $(TRANSFSUBDIRS)
transfer: ## Transfer *.ko to IP into DIR
	@echo "Transfering to $(IP) into $(DIR)..."
	@echo "$(GREEN)$(TARGETS) $(RESET)"
	@scp $(TARGETS) root@$(IP):$(DIR)

#------------------------------------------------------------------------------
.PHONY: help
help: ## Generate list of targets with descriptions
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m %s\n", $$1, $$2}'

#==============================================================================