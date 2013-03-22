CFLAGS = -Wall -Wextra -pedantic -march=armv6k -O0 -std=c99 -finline-functions -fno-stack-protector -nostdinc -ffreestanding -Wno-unused-function -Wno-unused-parameter -g
CFLAGS := ${CFLAGS} -fshort-wchar
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
ARCH = rpi

FILESYSTEMS  = $(patsubst %.c,%.o,$(wildcard kernel/fs/*.c))
VIDEODRIVERS = $(patsubst %.c,%.o,$(wildcard kernel/video/*.c))
DEVICES      = $(patsubst %.c,%.o,$(wildcard kernel/devices/*.c))
VIRTUALMEM   = $(patsubst %.c,%.o,$(wildcard kernel/mem/*.c))
MISCMODS     = $(patsubst %.c,%.o,$(wildcard kernel/misc/*.c))
SYSTEM       = $(patsubst %.c,%.o,$(wildcard kernel/sys/*.c))
DATASTRUCTS  = $(patsubst %.c,%.o,$(wildcard kernel/ds/*.c))
CPUBITS      = $(patsubst %.c,%.o,$(wildcard kernel/cpu/*.c))
RPIBITS_AS   = $(patsubst %.s,%.o,$(wildcard kernel/arch/rpi/*.s))
RPIBITS_C    = $(patsubst %.c,%.o,$(wildcard kernel/arch/rpi/*.c))

SUBMODULES = ${FILESYSTEMS} ${VIDEODRIVERS} ${DEVICES} ${VIRTUALMEM} ${MISCMODS} ${SYSTEM} ${DATASTRUCTS} ${CPUBITS} ${RPIBITS_AS} ${RPIBITS_C}
EXTRA =  kernel/arch/rpi/libcsud.a

UTILITIES = util/bin/readelf util/bin/typewriter util/bin/bim
EMU = qemu-system-arm
GENEXT = genext2fs
DD = dd conv=notrunc
BEG = util/mk-beg
END = util/mk-end
INFO = util/mk-info
ERRORS = 2>>/tmp/.`whoami`-build-errors || util/mk-error
ERRORSS = >>/tmp/.`whoami`-build-errors || util/mk-error

BEGRM = util/mk-beg-rm
ENDRM = util/mk-end-rm

EMUARGS     = -kernel toaruos-kernel -m 1024 -serial stdio -vga std -hda toaruos-disk.img -k en-us -no-frame -rtc base=localtime
EMUKVM      = -enable-kvm

.PHONY: all system clean clean-once clean-hard clean-soft clean-docs clean-bin clean-aux clean-core install run docs utils
.SECONDARY: 

all: system
system: kernel.img

kernel.img: toaruos-kernel
	@arm-none-eabi-objcopy $< -O binary $@

toaruos-kernel: ${SUBMODULES}
	@${BEG} "LD" "$@"
	@${LD} --no-undefined ${SUBMODULES} ${EXTRA} -T kernel/arch/${ARCH}/link.ld -o toaruos-kernel
	@${END} "LD" "$@"
	@${INFO} "--" "Kernel is ready!"

%.o: %.s
	@${BEG} "AS" "$<"
	@${AS} -I./kernel/include $< -o $@ ${ERRORS}
	@${END} "AS" "$<"

kernel/sys/version.o: kernel/*/*.c kernel/arch/*/*.c

%.o: %.c
	@${BEG} "CC" "$<"
	@${CC} ${CFLAGS} -I./kernel/include -c -o $@ $< ${ERRORS}
	@${END} "CC" "$<"


##############
#    ctags   #
##############
tags: kernel/*/*.c kernel/*.c .userspace-check
	@${BEG} "ctag" "Generating CTags..."
	@ctags -R --c++-kinds=+p --fields=+iaS --extra=+q kernel userspace util
	@${END} "ctag" "Generated CTags."

###############
#    clean    #
###############

clean-soft:
	@${BEGRM} "RM" "Cleaning modules..."
	@-rm -f kernel/*.o
	@-rm -f ${SUBMODULES}
	@${ENDRM} "RM" "Cleaned modules."

clean-docs:
	@${BEGRM} "RM" "Cleaning documentation..."
	@-rm -f docs/*.pdf docs/*.aux docs/*.log docs/*.out
	@-rm -f docs/*.idx docs/*.ind docs/*.toc docs/*.ilg
	@${ENDRM} "RM" "Cleaned documentation"

clean-bin:
	@${BEGRM} "RM" "Cleaning native binaries..."
	@-rm -f hdd/bin/*
	@${ENDRM} "RM" "Cleaned native binaries"

clean-aux:
	@${BEGRM} "RM" "Cleaning auxillary files..."
	@-rm -f loader/*.o
	@-rm -f util/bin/*
	@${ENDRM} "RM" "Cleaned auxillary files"

clean-core:
	@${BEGRM} "RM" "Cleaning final output..."
	@-rm -f toaruos-kernel
	@-rm -f toaruos-initrd
	@-rm -f kernel.img
	@${ENDRM} "RM" "Cleaned final output"

clean: clean-soft clean-core
	@${INFO} "--" "Finished soft cleaning"

clean-hard: clean clean-bin clean-aux clean-docs
	@${INFO} "--" "Finished hard cleaning"

clean-disk:
	@${BEGRM} "RM" "Deleting hard disk image..."
	@-rm -f toaruos-disk.img
	@${ENDRM} "RM" "Deleted hard disk image"

clean-once:
	@${BEGRM} "RM" "Cleaning one-time files..."
	@-rm -f .passed
	@${ENDRM} "RM" "Cleaned one-time files"

# vim:noexpandtab
# vim:tabstop=4
# vim:shiftwidth=4
