// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

/*
--- Day 1: Not Quite Lisp ---

Santa was hoping for a white Christmas, but his weather machine's "snow"
function is powered by stars, and he's fresh out! To save Christmas, he
needs you to collect fifty stars by December 25th.

Collect stars by helping Santa solve puzzles. Two puzzles will be made
available on each day in the Advent calendar; the second puzzle is unlocked
when you complete the first. Each puzzle grants one star. Good luck!

Here's an easy puzzle to warm you up.

Santa is trying to deliver presents in a large apartment building, but he
can't find the right floor - the directions he got are a little confusing.
He starts on the ground floor (floor 0) and then follows the instructions
one character at a time.

An opening parenthesis, (, means he should go up one floor, and a closing
parenthesis, ), means he should go down one floor.

The apartment building is very tall, and the basement is very deep; he will
never find the top or bottom floors.

For example:
    (()) and ()() both result in floor 0.
    ((( and (()(()( both result in floor 3.
    ))((((( also results in floor 3.
    ()) and ))( both result in floor -1 (the first basement level).
    ))) and )())()) both result in floor -3.

To what floor do the instructions take Santa?

--- Part Two ---

Now, given the same instructions, find the position of the first character
that causes him to enter the basement (floor -1). The first character in
the instructions has position 1, the second character has position 2, and
so on.

For example:
    ) causes him to enter the basement at character position 1.
    ()()) causes him to enter the basement at character position 5.

What is the position of the character that causes Santa to first enter the
basement?
*/

void
Part_One(const char *data)
{
    i64 floor = 0;

    while (*data != '\0') {
        if (*data == '(') {
            ++floor;
        } else if (*data == ')') {
            --floor;
        } else {
            Quit(2, "%s: invalid input %c (%.02x).", NAME, *data, *data);
        }

        ++data;
    }

    printf("Part one: floor #%li\n", floor);
}

void
Part_Two(const char *data)
{
    i64 floor = 0;
    i64 position = 1;

    while (*data != '\0') {
        if (*data == '(') {
            ++floor;
        } else if (*data == ')') {
            --floor;
        } else {
            Quit(3, "%s: invalid input %c (%.02x).", NAME, *data, *data);
        }

        if (floor == -1) {
            break;
        }

        ++position;
        ++data;
    }

    if (floor != -1) {
        Quit(4, "%s: basement not reached.", NAME);
    }

    printf("Part two: position %li\n", position);
}

int
main(void)
{
    Slice input = Slice_ReadStdIn();

    if (input.data == NULL || *input.data == '\0') {
        Quit(1, "%s: empty input.", NAME);
    }

    Part_One(input.data);
    Part_Two(input.data);

    return 0;
}

