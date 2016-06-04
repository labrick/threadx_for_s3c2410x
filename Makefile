#begin
TOPDIR    := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
export TOPDIR
export OUTOBJ:=$(TOPDIR)/out/
INCLUDEDIRS =$(TOPDIR)/include 
THREADXINC = $(TOPDIR)/threadx/threadx/os_kern
export THREADXINC
export INCLUDEDIRS
include $(TOPDIR)/config.mk


SUBDIRS :=arch lib driver app exception_handle threadx


LIBS = $(wildcard out/*.a)
OBJS = $(wildcard out/*.o)
#OBJS = $(OUTOBJ)*.o
.PHONY : $(SUBDIRS)

#all: .depend $(SUBDIRS)
all:objs link
	@echo "$(OBJS),$(LIBS)"
link:
	@echo "$(OBJS),$(LIBS)"
#	cd $(OUTOBJ) && LIBS = $(OUTOBJ)$(wildcard *.a) && OBJS = $(OUTOBJ)$(wildcard *.o)
	$(LD) $(LDFLAGS) $(wildcard out/*.o) $(wildcard out/*.a) -o zmOS --start-group -Map zmOS.map
	${OBJCOPY} -O binary -S zmOS zmOS.bin
	${OBJDUMP} -D zmOS > zmOS.dis

#	cd $(TOPDIR)
objs:
	
	@echo "start enter dir"
	for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir all; done

	
.PHONY : clean makeobjs
clean:
#	rm zmOS
	for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir clean ; done

