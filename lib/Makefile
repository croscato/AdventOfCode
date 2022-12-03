CC=gcc
CFLAGS=-std=gnu17 -Wall -Wextra -Wpedantic -Wconversion -Wunused
AR=ar
ARFLAGS=rcs

SRCS= \
	support.c \
	md5.c

OBJS=$(SRCS:.c=.c.o)
DEPS=$(OBJS:=.d)

.PHONY: all release debug run test clean

all: debug

release: CFLAGS += -O2 -g0
release: lib.a

debug: CFLAGS += -O0 -g3
debug: lib.a

lib.a: $(OBJS)
	@$(AR) $(ARFLAGS) $(@) $(^)

%.c.o: %.c
	@$(CC) $(CFLAGS) -MD -MT $(@) -MF $(@).d -o $(@) -c $(<) -include defs.h

clean:
	@-$(RM) -rf *.o *.a *.d

-include $(DEPS)

