include $(TOPDIR)/config.mk

#ARFLAGS = cr
#LIB = libarm.a

RM = -rm -rf

SRCS :=$(wildcard *.c)
OBJ    = $(patsubst %.c, %.o, $(wildcard *.c))
OBJS	= $(addprefix $(OUTOBJ)/,$(patsubst %.c,%.o,$(wildcard *.c)))
all:$(OBJ)
	@echo "uart build over"
	@echo "$(OBJS)"	
#uart.o:uart.c hello.c
#	$(CC) $(CFLAGS) uart.c
#	$(CC) $(CFLAGS) hello.c
#
#$(OBJS):$(SRCS)
#	$(CC) $(CFLAGS) -c -o $@ $(notdir $(patsubst %.o,%.c,$@))
%.o:%.c
	@echo "$@,$<"
	${CC} $(CFLAGS) -c -o $@ $<
	mv $@ $(OUTOBJ)

clean:
	rm -rf $(OBJS) $(OBJ)
.PHONY: all clean


