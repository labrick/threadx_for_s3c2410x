

include $(TOPDIR)/config.mk



AOBJS    = $(patsubst %.S, %.o, $(wildcard *.S))
COBJS    = $(patsubst %.c, %.o, $(wildcard *.c))
#CPPOBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

SRCS :=$(wildcard *.S) $(wildcard *.c)
OBJ    = $(AOBJS) $(COBJS)
OBJS    = $(addprefix $(OUTOBJ),$(OBJ))
all: $(OBJ)
	@echo "complete $(OBJ)"

%.o:%.S
	${CC} $(CFLAGS) -c -o $@ $< 
	mv $@ $(OUTOBJ)
%.o:%.c
	${CC} $(CFLAGS) -I../os_kern -I../../target -c -o $@ $<
	mv $@ $(OUTOBJ)

clean:
	rm -rf $(OBJS) $(OBJ)
.PHONY: all clean