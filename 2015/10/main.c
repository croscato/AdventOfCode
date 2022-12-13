// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

/*
--- Day 10: Elves Look, Elves Say ---

Today, the Elves are playing a game called look-and-say. They take turns
making sequences by reading aloud the previous sequence and using that
reading as the next sequence. For example, 211 is read as "one two, two
ones", which becomes 1221 (1 2, 2 1s).

Look-and-say sequences are generated iteratively, using the previous value
as input for the next step. For each step, take the previous value, and
replace each run of digits (like 111) with the number of digits (3)
followed by the digit itself (1).

For example:
    - 1 becomes 11 (1 copy of digit 1).
    - 11 becomes 21 (2 copies of digit 1).
    - 21 becomes 1211 (one 2 followed by one 1).
    - 1211 becomes 111221 (one 1, one 2, and two 1s).
    - 111221 becomes 312211 (three 1s, two 2s, and one 1).

Starting with the digits in your puzzle input, apply this process 40 times.
What is the length of the result?


--- Part Two ---

Neat, right? You might also enjoy hearing John Conway talking about this
sequence (that's Conway of Conway's Game of Life fame).

Now, starting again with the digits in your puzzle input, apply this
process 50 times. What is the length of the new result?
*/

#define BUFFER_SIZE (8 * 1024 * 1024)

static char Buffer[BUFFER_SIZE] = {0};
static char Input[BUFFER_SIZE] = {0};

static u64
Run_Sequence(const char *input, int times)
{
    strcpy(Input, input);

    for (int i = 0; i < times; ++i) {
        const char *cursor = Input;
        char *buffer = Buffer;
        int last_char = *cursor++;
        int count = 1;

        while (*cursor != '\0') {
            if (last_char != *cursor) {
                int bytes_written = sprintf(buffer, "%d%c", count, (char) last_char);

                buffer += bytes_written;

                last_char = *cursor;
                count = 1;
            } else {
                count++;
            }

            cursor++;
        }

        if (count > 0) {
            int bytes_written = sprintf(buffer, "%d%c", count, (char) last_char);

            buffer += bytes_written;
        }

        *buffer = '\0';

        strcpy(Input, Buffer);
    }

    return strlen(Buffer);
}


void
Part_One(const char *input)
{
    printf("Part one: string length %ld\n", Run_Sequence(input, 40));
}


void
Part_Two(const char *input)
{
    printf("Part two: string length %ld\n", Run_Sequence(input, 50));
}

int
main(void)
{
    Part_One("1321131112");
    Part_Two("1321131112");

    return 0;
}

