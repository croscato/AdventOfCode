// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

/*
--- Day 12: JSAbacusFramework.io ---

Santa's Accounting-Elves need help balancing the books after a recent
order. Unfortunately, their accounting software uses a peculiar storage
format. That's where you come in.

They have a JSON document which contains a variety of things: arrays
([1,2,3]), objects ({"a":1, "b":2}), numbers, and strings. Your first job
is to simply find all of the numbers throughout the document and add them
together.

For example:
    - [1,2,3] and {"a":2,"b":4} both have a sum of 6.
    - [[[3]]] and {"a":{"b":4},"c":-1} both have a sum of 3.
    - {"a":[-1,1]} and [-1,{"a":1}] both have a sum of 0.
    - [] and {} both have a sum of 0.

You will not encounter any strings containing numbers.

What is the sum of all numbers in the document?


--- Part Two ---

Uh oh - the Accounting-Elves have realized that they double-counted
everything red.

Ignore any object (and all of its children) which has any property with the
value "red". Do this only for objects ({...}), not arrays ([...]).

    - [1,2,3] still has a sum of 6.
    - [1,{"c":"red","b":2},3] now has a sum of 4, because the middle object
      is ignored.
    - {"d":"red","e":[1,2,3,4],"f":5} now has a sum of 0, because the
      entire structure is ignored.
    - [1,"red",5] has a sum of 6, because "red" in an array has no effect.
*/

static i64
Calculate_SumOne(const char *data)
{
    i64 sum = 0;

    while (*data != '\0') {
        if (*data == '-' || isdigit(*data)) {
            char *cursor = NULL;
            sum += strtol(data, &cursor, 10);
            data += cursor - data;
        } else {
            data++;
        }
    }

    return sum;
}

// 166774 high
// 143653 high

static int red_count = 0;

static const char *
Skip_ObjectWithRed(const char *data)
{
    const char *cursor = data;
    u64 object_scope = 0;
    bool has_red_object = false;

    while (*cursor != '\0') {
        if (*cursor == '{') {
            object_scope++;
        } else if (*cursor == '}') {
            object_scope--;
        } else if (*cursor == ':' && has_red_object == false) {
            cursor++;

            while (*cursor == ' ') {
                cursor++;
            }

            if (memcmp(cursor, "\"red\"", 5) == 0) {
                red_count++;
                has_red_object = true;

                break;
            }
        }

        cursor++;

        if (object_scope == 0) {
            break;
        }
    }

    if (has_red_object) {
        while (*cursor != '\0' && object_scope > 0) {
            if (*cursor == '}') {
                object_scope--;
            }

            cursor++;
        }

        return cursor;
    }

    return data;
}

static i64
Calculate_SumTwo(const char *data)
{
    i64 sum = 0;

    while (*data != '\0') {
        if (*data == '{') {
            data = Skip_ObjectWithRed(data);
        }

        char *cursor = NULL;

        if (*data == '-' || isdigit(*data)) {
            sum += strtol(data, &cursor, 10);
            data += cursor - data;
        } else {
            data++;
        }
    }

    return sum;
}

void
Part_One(Slice input)
{
    i64 sum = 0;

    while (1) {
        Slice line = Slice_ReadLine(&input);

        if (line.data == NULL) {
            break;
        }

        sum += Calculate_SumOne(line.data);
    }

    printf("Part one: sum %ld\n", sum);
}

void
Part_Two(Slice input)
{
    i64 sum = 0;

    while (1) {
        Slice line = Slice_ReadLine(&input);

        if (line.data == NULL) {
            break;
        }

        sum += Calculate_SumTwo(line.data);
    }

    printf("Part two: sum %ld\n", sum);
}

int
main(void)
{
    Slice input = Slice_ReadStdIn();

    if (input.data == NULL || *input.data == '\0') {
        Quit(1, "%s: empty input.", NAME);
    }

    Part_One(input);
    Part_Two(input);
    printf("Red count: %d\n", red_count);

    return 0;
}

