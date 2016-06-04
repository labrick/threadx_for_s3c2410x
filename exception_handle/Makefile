include $(TOPDIR)/config.mk

SRCS :=$(wildcard *.c)
OBJ    = $(patsubst %.c, %.o, $(wildcard *.c))
OBJS	= $(addprefix $(OUTOBJ),$(patsubst %.c,%.o,$(wildcard *.c)))
all:$(OBJ)
	@echo "exception_handle build over"	
#$(OBJS):$(SRCS)
#	$(CC) $(CFLAGS) -c -o $@ $(notdir $(patsubst %.o,%.c,$@))
%.o:%.c
	@echo "$@,$<"
	${CC} -nostdinc $(CFLAGS) -Wall -O2 -fno-builtin -c -o $@ $<
	mv $@ $(OUTOBJ)

clean:
	rm -rf $(OBJS) $(OBJ)
.PHONY: all clean