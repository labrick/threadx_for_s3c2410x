include $(TOPDIR)/config.mk


LIB = libarm.a

RM = -rm -rf

OBJ = $(OUTOBJ)$(LIB)

AOBJS    = $(patsubst %.S, %.o, $(wildcard *.S))
COBJS    = $(patsubst %.c, %.o, $(wildcard *.c))
CPPOBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

SRCS :=$(wildcard *.S) $(wildcard *.c) $(wildcard *.cpp)
OBJS    = $(AOBJS) $(COBJS) $(CPPOBJS)

all:$(OBJ)
	@echo "completed libarm.a"
$(OBJ):$(OBJS)
	$(AR) -r -o $@ $(OBJS)
#$(OBJS):$(SRCS)
#	$(CC) $(CFLAGS) $(CPPFLAGS) $^
%.o:%.c
	${CC} $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(OBJ) *.bak *~
.PHONY: all clean
	
