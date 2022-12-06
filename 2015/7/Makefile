LIB_PATH=../../lib

CC=gcc
CFLAGS=-std=gnu17 -Wall -Wextra -Wpedantic -Wconversion -Wunused -I$(LIB_PATH)
IFLAGS = \
	-fsanitize=address \
	-fsanitize=leak \
	-fsanitize=undefined \
	-fsanitize=bounds \
	-fsanitize=bounds-strict \
	-fsanitize=alignment \
	-fsanitize=object-size \
	-fsanitize=float-cast-overflow \
	-fsanitize=nonnull-attribute \
	-fsanitize=returns-nonnull-attribute \
	-fsanitize=enum \
	-fsanitize=vptr \
	-fsanitize=pointer-overflow \
	-fsanitize=builtin \
	-fstack-protector-all \
	-fstack-protector-strong

ADVENT_YEAR = 2015
ADVENT_NR = 7

CFLAGS += -DNAME=\"Advent_$(ADVENT_YEAR)_$(ADVENT_NR)\"

SRCS = \
	main.c \

.PHONY: all release debug run test clean

all: debug

release: CFLAGS += -O2 -Wl,-s
release: advent_$(ADVENT_NR)

debug: CFLAGS += -O0 -g3
debug: CFLAGS += $(IFLAGS)
debug: advent_$(ADVENT_NR)

advent_$(ADVENT_NR) : $(SRCS) $(LIB_PATH)/lib.a
	@$(CC) $(CFLAGS) -o $(@) $(^) -include $(LIB_PATH)/defs.h

run: advent_$(ADVENT_NR)
	@./$(<) < input

test: advent_$(ADVENT_NR)
	@./$(<) < sample

clean:
	@-$(RM) -rf advent_$(ADVENT_NR) *.d
