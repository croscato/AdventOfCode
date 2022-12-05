// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

void
Part_One(Slice input)
{
    while (1) {
        Slice line = Slice_ReadLine(&input);

        if (line.data == NULL) {
            break;
        }
    }
}

void
Part_Two(Slice input)
{
    while (1) {
        Slice line = Slice_ReadLine(&input);

        if (line.data == NULL) {
            break;
        }
    }
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

    return 0;
}

