.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

export TARGET  := cfb
export TOPDIR  := $(CURDIR)

include $(DEVKITARM)/ds_rules

.PHONY: checkarm9 clean

ARM7_BIN := $(DEVKITPRO)/calico/bin/ds7_maine.elf

all: checkarm9 $(TARGET).nds

checkarm9:
	$(MAKE) -C arm9

$(TARGET).nds: arm9/$(TARGET).elf
	ndstool -c $(TARGET).nds -7 $(ARM7_BIN) -9 arm9/$(TARGET).elf

arm9/$(TARGET).elf:
	$(MAKE) -C arm9

clean:
	$(MAKE) -C arm9 clean
	rm -f $(TARGET).nds
