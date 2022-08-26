# Copyright (C) 2012       Alejandro Mery <amery@geeks.cl>
# Copyright (C) 2012,2013  Henrik Nordstrom <henrik@henriknordstrom.net>
# Copyright (C) 2013       Patrick Wood <patrickhwood@gmail.com>
# Copyright (C) 2013       Pat Wood <Pat.Wood@efi.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Windows predefines OS in the environment (to "Windows_NT"), otherwise use uname
OS ?= $(shell uname)

CC ?= gcc
DEFAULT_CFLAGS := -std=c99
DEFAULT_CFLAGS += -Wall -Wextra -Wno-unused-result

DEFAULT_CFLAGS += -D_POSIX_C_SOURCE=200112L
# Define _BSD_SOURCE, necessary to expose all endian conversions properly.
# See http://linux.die.net/man/3/endian
DEFAULT_CFLAGS += -D_BSD_SOURCE
# glibc 2.20+ also requires _DEFAULT_SOURCE
DEFAULT_CFLAGS += -D_DEFAULT_SOURCE

DEFAULT_CFLAGS += -Iinclude/

PKG_CONFIG ?= pkg-config

MKSUNXIBOOT ?= mksunxiboot
PATH_DIRS := $(shell echo $$PATH | sed -e 's/:/ /g')

FLUTTER_PATH := $(shell echo $$HOME)/flutter
DART_SDK_PATH := $(FLUTTER_PATH)/bin/cache/dart-sdk

# Try to guess a suitable default ARM cross toolchain
CROSS_DEFAULT := arm-none-eabi-
CROSS_COMPILE ?= $(or $(shell ./find-arm-gcc.sh),$(CROSS_DEFAULT))
CROSS_CC := $(CROSS_COMPILE)gcc

LIBUSB = libusb-1.0
LIBUSB_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags $(LIBUSB))

ZLIB = zlib
ZLIB_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags $(ZLIB))

HOST_CFLAGS = $(DEFAULT_CFLAGS) $(CFLAGS) -Wno-unused-parameter -fPIC

SOC_INFO := soc_info.c soc_info.h
FEL_LIB  := fel_lib.c fel_lib.h
SPI_FLASH:= fel-spiflash.c fel-spiflash.h fel-remotefunc-spi-data-transfer.h
PROGRESS := progress.c progress.h

include libs/Makefile

libsunxifel.so: fel.c fit_image.c thunks/fel-to-spl-thunk.h \
				$(PROGRESS) $(SOC_INFO) $(FEL_LIB) $(SPI_FLASH) \
				$(DART_SDK_PATH)/include/dart_api_dl.c yfdart.c
	$(CC) -shared $(HOST_CFLAGS) $(LIBUSB_CFLAGS) $(ZLIB_CFLAGS) $(LDFLAGS) -o $@ \
		$(filter %.c,$^) $(LIBS) \
		$(STATIC_LIBS) \
		-ludev -lpthread -fPIC \
		-I$(DART_SDK_PATH)/include
