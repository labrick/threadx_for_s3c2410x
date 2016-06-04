include $(TOPDIR)/config.mk

SRCS :=$(wildcard *.c)
OBJ    = $(patsubst %.c, %.o, $(wildcard *.c))
OBJS	= $(addprefix $(OUTOBJ),$(patsubst %.c,%.o,$(wildcard *.c)))
all:$(OBJ)
	@echo "completed main.o"	
#$(OBJS):$(SRCS)
#	$(CC) $(CFLAGS) -c -o $@ $(notdir $(patsubst %.o,%.c,$@))
%.o:%.c
	@echo "$@,$<"
	${CC} -nostdinc -I$(THREADXINC) $(CFLAGS)  -Wall -O2 -fno-builtin -c -o  $@ $<
	mv $@ $(OUTOBJ)

clean:
	rm -rf $(OBJS) $(OBJ)
.PHONY: all clean