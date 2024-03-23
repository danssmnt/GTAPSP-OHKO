.PHONY: clean1

TARGET = build/GTA_OHKO
OBJS = src/exports/exports.o src/utils/patch.o src/utils/utils.o src/main.o
	
CFLAGS = -O2 -Os -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division -Werror
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

PSP_LARGE_MEMORY = 1
BUILD_PRX = 1
PRX_EXPORTS = exports.exp

USE_PSPSDK_LIBC = 1
USE_PSPSDK_LIBS = 1

LIBS = -lpspsystemctrl_user

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak

clean1:
	-rm *.o $(OBJS)