#==============================================================================
BLUE 	=\033[0;34m
GREEN	=\033[0;32m
CYAN	=\033[0;36m
RESET	=\033[0m
#------------------------------------------------------------------------------
obj-m 	:= lampf.o
lampf-objs:=lampfdriver.o utils.o
KDIR 	:=$(HOME)/buildroot/buildroot-2021.02.5/output/build/linux-custom/
ARCH 	?=arm
#------------------------------------------------------------------------------
CROSS_COMPILE :=arm-buildroot-linux-gnueabihf-
export ARCH CROSS_COMPILE
#------------------------------------------------------------------------------
KBUILD_TARGETS	:= clean help modules modules_install
#==============================================================================
.PHONY: all $(KBUILD_TARGETS)

all: modules
$(KBUILD_TARGETS):
	@$(MAKE) -C $(KDIR) M=$(shell pwd) $@

#------------------------------------------------------------------------------
IP=10.42.0.254
DIR=/etc
TARGET=*.ko

transfer:
	@echo "Transfering:"
	@echo "${GREEN}"$(shell ls $(TARGET))"$(RESET)"
	@scp $(TARGET) root@$(IP):$(DIR)
