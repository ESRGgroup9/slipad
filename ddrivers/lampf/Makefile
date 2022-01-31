#==============================================================================
# build module lampf.ko using the lampf.o object file
obj-m 	:= lampf.o
# module objects
lampf-objs:=lampfdriver.o utils.o
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