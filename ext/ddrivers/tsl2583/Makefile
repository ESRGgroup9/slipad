obj-m := ldr.o
ldr-objs:=tsl2583.o
KDIR:=$(HOME)/buildroot/buildroot-2021.02.5/output/build/linux-custom/
ARCH ?=arm

#CROSS_COMPILE ?= ~/buildroot/buildroot-2020.08/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-
# CROSS_COMPILE ?= ~/buildroot/buildroot-2020.08/output/host/bin/arm-buildroot-linux-gnueabihf-
CROSS_COMPILE :=arm-buildroot-linux-gnueabihf-

export ARCH CROSS_COMPILE

KBUILD_TARGETS	:= clean help modules modules_install

.PHONY: all $(KBUILD_TARGETS)

all: modules
$(KBUILD_TARGETS):
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@

#$(MAKE) -C $(KDIR) M=$(shell pwd) $@ EXTRA_CFLAGS="$(MY_CFLAGS)"