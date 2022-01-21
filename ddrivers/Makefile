#==============================================================================
BLUE 	=\033[0;34m
GREEN	=\033[0;32m
CYAN	=\033[0;36m
RESET	=\033[0m

PRINT_GENERATING="${CYAN}Generating $(shell basename $@) ...$(RESET)"
PRINT_BUILDING	="${BLUE}Building $(shell basename $@) ...$(RESET)"
PRINT_COMPILING	="${GREEN}Compiling $(shell basename $@) ...$(RESET)"
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
PROGS=./pir ./lampf
#==============================================================================

.DEFAULT_GOAL = build
build: $(PROGS) ## Compile the binary program
.PHONY: all $(PROGS)

$(PROGS):
	@echo $(PRINT_COMPILING) 
	@$(MAKE) -C $@

#------------------------------------------------------------------------------
clean: ## Delete built artifacts
	@echo "${CYAN}Cleaning ./pir ...$(RESET)"
	@$(MAKE) clean -C ./pir
	@echo "${CYAN}Cleaning ./lampf ...$(RESET)"
	@$(MAKE) clean -C ./lampf

#------------------------------------------------------------------------------
# IP=10.42.0.254
# DIR=/etc

transfer:
# 	@echo "Transfering to $(IP) into $(DIR)..."
	@$(MAKE) transfer -C ./pir
	@$(MAKE) transfer -C ./lampf

help: ## Generate list of targets with descriptions
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m %s\n", $$1, $$2}'

.PHONY: build clean transfer help
