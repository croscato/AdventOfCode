CC=gcc
CFLAGS=-std=gnu17 -O2 -Wl,-s -Wall -Wextra -Wpedantic -Wconversion -Wunused

ADVENT_NR = 3

.PHONY: run test clean

advent_$(ADVENT_NR) : main.c
	@$(CC) $(CFLAGS) -o $(@) $(^)

run: advent_$(ADVENT_NR)
	@./$(^) < input

test: advent_$(ADVENT_NR)
	@./$(^) < sample

clean:
	@-$(RM) -rf advent_$(ADVENT_NR)
