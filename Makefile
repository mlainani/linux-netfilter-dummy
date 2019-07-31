# The present Make file is intended for cross-compiling this kernel module for a
# Raspberry Pi 3 Model B+. It assumes you've obtained and successfully
# cross-compiled the same kernel image (and modules) as the ones running on your
# board. The general process is described in the official documentation for the
# Raspberry Pi but, unless you need to build a custom kernel, you must get the
# sources for the kernel version running on your board. This can be achieved
# with the following steps
#
# 1. Determine the Linux kernel version and build release on your Raspberry Pi
#
# $ uname -r
# 4.19.57-v7+
#
# $ vcgencmd version
# Jul  9 2019 14:40:53
# Copyright (c) 2012 Broadcom
# version 6c3fe3f096a93de3b34252ad98cdccadeb534be2 (clean) (release) (start)
#
# 2. Clone the Raspberry Pi Linux kernel Git repo (about 4GB size)
#
# $ git clone https://github.com/raspberrypi/linux.git
#
# 3. Find the Git tag corresponding to the Linux kernel build release on your
# Raspberry Pi
#
# $ git tag
# ...
# raspberrypi-kernel_1.20190517-1
# raspberrypi-kernel_1.20190620-1
# raspberrypi-kernel_1.20190709-1 <==
# ...
#
# 4. Checkout the corresponding branch and commit into a local branch
#
# $ git checkout raspberrypi-kernel_1.20190709-1 -b mytestbranch
#
# $ head -5 Makefile
# # SPDX-License-Identifier: GPL-2.0
# VERSION = 4
# PATCHLEVEL = 19
# SUBLEVEL = 57
# EXTRAVERSION =

CFLAGS_MODULE += -Werror
CFLAGS_MODULE += -DDEBUG

ARCH := arm
KERNELDIR := /home/godzilla/making/linux
CROSS_COMPILE := arm-linux-gnueabihf-

PWD       := $(shell pwd)

obj-m	:= nf_dummy.o

default:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf .*.cmd .tmp_versions *~ *.ko *.mod.c *.o modules.order Module.symvers
