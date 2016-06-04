include $(TOPDIR)/config.mk


LIB = threadx.o

RM = -rm -rf

OBJ = $(OUTOBJ)$(LIB)

AOBJS    = $(patsubst %.S, %.o, $(wildcard *.S))
COBJS    = $(patsubst %.c, %.o, $(wildcard *.c))
CPPOBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

SRCS :=$(wildcard *.S) $(wildcard *.c) $(wildcard *.cpp)
OBJS    = $(AOBJS) $(COBJS) $(CPPOBJS)

all:$(OBJ)
	@echo "completed threadxlib.a"
$(OBJ):$(OBJS)
	$(LD) -r -o $@ $(OBJS)
#$(OBJS):$(SRCS)
#	$(CC) $(CFLAGS) $(CPPFLAGS) $^
%.o:%.c
	${CC} $(CFLAGS) -c -o $@ $< -I$(TOPDIR)/threadx/threadx/os_kern

%.o:%.S
	${CC} $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(OBJ) *.bak *~
.PHONY: all clean
	
