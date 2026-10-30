# SPDX-License-Identifier: GPL-2.0-only
obj-$(CONFIG_ENCORE_FAS) += encore_fas.o
encore_fas-y := encore_fas_main.o \
                encore_fas_ctx.o \
                encore_fas_evq.o \
                encore_fas_time.o

ifeq ($(KERNELRELEASE),)

# Out-of-tree build: make KDIR=<kernel build dir> ARCH=arm64 CROSS_COMPILE=...
KDIR ?= /lib/modules/$(shell uname -r)/build
HOSTCC ?= cc

all: check_symbol
	$(MAKE) -C $(KDIR) M=$(CURDIR) CONFIG_ENCORE_FAS=m modules
	@if [ -f $(KDIR)/vmlinux ]; then \
		./scripts/tools/check_symbol encore_fas.ko $(KDIR)/vmlinux; \
	else \
		echo "-- no vmlinux in KDIR, skipping symbol check"; \
	fi

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean
	rm -f scripts/tools/check_symbol

check_symbol: scripts/tools/check_symbol.c
	$(HOSTCC) scripts/tools/check_symbol.c -o scripts/tools/check_symbol

.PHONY: all clean

else

fas_srcdir ?= $(srctree)/drivers/encore_fas
ccflags-y += -I$(fas_srcdir)/include

FAS_UPSTREAM_VERSION ?= 9
FAS_UPSTREAM_BRANCH ?= "main"
ccflags-y += -DFAS_VERSION=$(FAS_UPSTREAM_VERSION)
ccflags-y += -DFAS_BRANCH=\"$(FAS_UPSTREAM_BRANCH)\"

# Probe the kernel headers. A vendor kernel can backport an API, so the
# headers are more exact than LINUX_VERSION_CODE.
fas_uprobes_h := $(srctree)/include/linux/uprobes.h
fas_hrtimer_h := $(srctree)/include/linux/hrtimer.h

ifneq ($(shell grep -qs uprobe_unregister_nosync $(fas_uprobes_h) && echo y),)
ifneq ($(shell grep -qs '__u64 \*' $(fas_uprobes_h) && echo y),)
ccflags-y += -DFAS_UPROBE_API=3
else
ccflags-y += -DFAS_UPROBE_API=2
endif
else
ccflags-y += -DFAS_UPROBE_API=1
endif

ifneq ($(shell grep -qs hrtimer_setup $(fas_hrtimer_h) && echo y),)
ccflags-y += -DFAS_HAVE_HRTIMER_SETUP
endif

# The timer modes moved to hrtimer_types.h in v6.10.
ifneq ($(shell cat $(srctree)/include/linux/hrtimer*.h 2>/dev/null | grep -qs HRTIMER_MODE_REL_PINNED_SOFT && echo y),)
ccflags-y += -DFAS_HAVE_HRTIMER_SOFT
endif

endif
