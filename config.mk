
CROSS_COMPILE:=arm-linux-gnueabi-
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
CC = $(CROSS_COMPILE)gcc
CPP = $(CC) -E
AR = $(CROSS_COMPILE)ar
NM = $(CROSS_COMPILE)nm
STRIP = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB = $(CROSS_COMPILE)RANLIB

#CFLAGS= -c -Wall -Wstrict-prototypes -Wno-trigraphs -O2 -pipe -g -march=armv4t -mtune=arm920t
#CFLAGS = -c -Wall -nostdlib -nostdinc -fno-builtin -g -march=armv4t -mtune=arm920t -I$(INCLUDEDIRS)
CFLAGS =  -Wall -g -fno-builtin -march=armv4t -mtune=arm920t -nostdlib -nostdinc -I$(INCLUDEDIRS) -I$(TOPDIR) -c -o
# -lgcc -nostdlib -nostartfiles 
# -L $(LIBS) 
LIBS=$(TOPDIR)/libs/

LDSCRIPT :=$(TOPDIR)/zmOS.lds
LDFLAGS= -T$(LDSCRIPT)

export CC
export LD
export AR
export LIBS
export INCLUDEDIRS

