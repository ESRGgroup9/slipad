#==============================================================================
# build module pir.ko using the pir.o object file
obj-m 	:=pir.o
# module objects
pir-objs:=pirdriver.o utils.o
#------------------------------------------------------------------------------
# Make use of CROSS_COMPILE and KDIR, exported in main makefile
export ARCH CROSS_COMPILE
#------------------------------------------------------------------------------
KBUILD_TARGETS	:= clean help modules modules_install
#==============================================================================

.PHONY: all $(KBUILD_TARGETS)

all: modules
$(KBUILD_TARGETS):
	@$(MAKE) -C $(KDIR) M=$(shell pwd) $@

#==============================================================================