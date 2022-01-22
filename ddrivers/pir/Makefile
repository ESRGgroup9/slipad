#==============================================================================
# build module pir.ko using the pir.o object file
obj-m 	:=pir.o
# module objects
pir-objs:=pirdriver.o utils.o
# path of the kernel build directory
KDIR 	:=$(HOME)/buildroot/buildroot-2021.02.5/output/build/linux-custom/
# target architecture
ARCH	?=arm
#------------------------------------------------------------------------------
# CROSS_COMPILE :=arm-buildroot-linux-gnueabihf-
CROSS_COMPILE :=arm-linux-
export ARCH CROSS_COMPILE
#------------------------------------------------------------------------------
KBUILD_TARGETS	:= clean help modules modules_install
#==============================================================================

.PHONY: all $(KBUILD_TARGETS)

all: modules
$(KBUILD_TARGETS):
	@$(MAKE) -C $(KDIR) M=$(shell pwd) $@

#==============================================================================