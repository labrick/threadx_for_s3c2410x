
include $(TOPDIR)/config.mk

SUBDIRS := uart timer

all:
	@echo "start enter driver dir"
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir all; done

clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done