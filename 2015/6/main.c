// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

/*
--- Day 6: Probably a Fire Hazard ---

Because your neighbors keep defeating you in the holiday house decorating
contest year after year, you've decided to deploy one million lights in a
1000x1000 grid.

Furthermore, because you've been especially nice this year, Santa has
mailed you instructions on how to display the ideal lighting configuration.

Lights in your grid are numbered from 0 to 999 in each direction; the
lights at each corner are at 0,0, 0,999, 999,999, and 999,0. The
instructions include whether to turn on, turn off, or toggle various
inclusive ranges given as coordinate pairs. Each coordinate pair represents
opposite corners of a rectangle, inclusive; a coordinate pair like 0,0
through 2,2 therefore refers to 9 lights in a 3x3 square. The lights all
start turned off.

To defeat your neighbors this year, all you have to do is set up your
lights by doing the instructions Santa sent you in order.

For example:
    - turn on 0,0 through 999,999 would turn on (or leave on) every light.
    - toggle 0,0 through 999,0 would toggle the first line of 1000 lights,
      turning off the ones that were on, and turning on the ones that were
      off.
    - turn off 499,499 through 500,500 would turn off (or leave off) the
      middle four lights.

After following the instructions, how many lights are lit?


--- Part Two ---

You just finish implementing your winning light pattern when you realize
you mistranslated Santa's message from Ancient Nordic Elvish.

The light grid you bought actually has individual brightness controls; each
light can have a brightness of zero or more. The lights all start at zero.

The phrase turn on actually means that you should increase the brightness
of those lights by 1.

The phrase turn off actually means that you should decrease the brightness
of those lights by 1, to a minimum of zero.

The phrase toggle actually means that you should increase the brightness of
those lights by 2.

What is the total brightness of all lights combined after following Santa's
instructions?

For example:
    - turn on 0,0 through 0,0 would increase the total brightness by 1.
    - toggle 0,0 through 999,999 would increase the total brightness by
      2000000.
*/

#define GRID_ROWS 1000
#define GRID_COLS 1000

typedef enum Action {
      Invalid
    , On
    , Off
    , Toggle
} Action;

typedef struct Coordinate {
    u16 row;
    u16 col;
} Coordinate;

typedef struct Range {
    Coordinate start;
    Coordinate end;
} Range;

typedef struct Input {
    Action action;
    Range range;
} Input;

u16 grid[GRID_ROWS][GRID_COLS] = {0};

typedef void (* Grid_Apply)(Coordinate coordinate);

static const Slice ActionOn = { "turn on", 7 };
static const Slice ActionOff = { "turn off", 8 };
static const Slice ActionToggle = { "toggle", 6 };
static const Slice DataDivision = { "through", 7 };

static void
Grid_CheckRange(Range range)
{
    if (
        (range.end.col < range.start.col || range.end.row < range.start.row) ||
        (range.start.col > GRID_COLS || range.start.row > GRID_ROWS) ||
        (range.end.col > GRID_COLS || range.end.row > GRID_ROWS)
    ) {
        Quit(2, "%s: Invalid range (%u,%u:%u,%u).\n", NAME,
            range.start.row, range.start.col, range.end.row, range.end.col
        );
    }
}

static void
Grid_Reset()
{
    for (u16 row = 0; row < GRID_ROWS; ++row) {
        for (u16 col = 0; col < GRID_COLS; ++col) {
            grid[row][col] = 0;
        }
    }
}

static void
Grid_OnOne(Coordinate coordinate)
{
    grid[coordinate.row][coordinate.col] = true;
}

static void
Grid_OffOne(Coordinate coordinate)
{
    grid[coordinate.row][coordinate.col] = false;
}

static void
Grid_ToggleOne(Coordinate coordinate)
{
    grid[coordinate.row][coordinate.col] ^= true;
}

static void
Grid_OnTwo(Coordinate coordinate)
{
    grid[coordinate.row][coordinate.col]++;
}

static void
Grid_OffTwo(Coordinate coordinate)
{
    if (grid[coordinate.row][coordinate.col] > 0) {
        grid[coordinate.row][coordinate.col]--;
    }
}

static void
Grid_ToggleTwo(Coordinate coordinate)
{
    grid[coordinate.row][coordinate.col] += 2;
}

static void
Grid_ForRange(Range range, Grid_Apply apply)
{
    Grid_CheckRange(range);

    for (u16 row = range.start.row; row <= range.end.row; ++row) {
        for (u16 col = range.start.col; col <= range.end.col; ++col) {
            apply((Coordinate){row, col});
        }
    }
}

u64
Grid_Count(Action action)
{
    u64 count = 0;

    if (action != On && action != Off) {
        Quit(4, "%s: invalid action (%d).", action);
    }

    for (u16 row = 0; row < GRID_ROWS; ++row) {
        for (u16 col = 0; col < GRID_COLS; ++col) {
            u16 data = grid[row][col];

            while (data) {
                if (action == On && (data & 0x01)) {
                    count++;
                } else if (action == Off && ((data & 0x01) == 0)) {
                    count++;
                }

                data >>= 1;
            }
        }
    }

    return count;
}

u64
Grid_Brigthness(void)
{
    u64 brightness = 0;

    for (u16 row = 0; row < GRID_ROWS; ++row) {
        for (u16 col = 0; col < GRID_COLS; ++col) {
            brightness += grid[row][col];
        }
    }

    return brightness;
}

const char *
Skip_Spaces(const char *input)
{
    if (input == NULL) {
        return NULL;
    }

    while (*input == ' ') {
        input++;
    }

    return input;
}

static Input
Parse_Input(const char *input) {
    Input result = { Invalid, {{0,0},{0,0}} };

    const char *cursor = input;

    if (memcmp(cursor, ActionOn.data, ActionOn.size) == 0) {
        result.action = On;
        cursor += ActionOn.size;
    } else if (memcmp(cursor, ActionOff.data, ActionOff.size) == 0) {
        result.action = Off;
        cursor += ActionOff.size;
    } else if (memcmp(cursor, ActionToggle.data, ActionToggle.size) == 0) {
        result.action = Toggle;
        cursor += ActionToggle.size;
    } else {
        goto error;
    }

    char *end;

    result.range.start.row = (u16) strtoul(cursor, &end, 10);

    if (errno == ERANGE || cursor == end) {
        goto error;
    }

    cursor = end + 1;

    result.range.start.col = (u16) strtoul(cursor, &end, 10);

    if (errno == ERANGE || cursor == end) {
        goto error;
    }

    cursor = Skip_Spaces(end);

    if (memcmp(cursor, DataDivision.data, DataDivision.size) != 0) {
        goto error;
    }

    cursor += DataDivision.size;

    result.range.end.row = (u16) strtoul(cursor, &end, 10);

    if (errno == ERANGE || cursor == end) {
        goto error;
    }

    cursor = end + 1;

    result.range.end.col = (u16) strtoul(cursor, &end, 10);

    if (errno == ERANGE || cursor == end) {
        goto error;
    }

    return result;

error:
    Quit(3, "%s: invalid input '%s'.", NAME, input);
}

void
Part_One(Slice data)
{
    while (1) {
        Slice line = Slice_ReadLine(&data);

        if (line.data == NULL) {
            break;
        }

        Input input = Parse_Input(line.data);

        if (input.action == On) {
            Grid_ForRange(input.range, Grid_OnOne);
        } else if (input.action == Off) {
            Grid_ForRange(input.range, Grid_OffOne);
        } else if (input.action == Toggle) {
            Grid_ForRange(input.range, Grid_ToggleOne);
        }
    }

    printf("Part one: %lu lighs on\n", Grid_Count(On));
}

void
Part_Two(Slice data)
{
    while (1) {
        Slice line = Slice_ReadLine(&data);

        if (line.data == NULL) {
            break;
        }

        Input input = Parse_Input(line.data);

        if (input.action == On) {
            Grid_ForRange(input.range, Grid_OnTwo);
        } else if (input.action == Off) {
            Grid_ForRange(input.range, Grid_OffTwo);
        } else if (input.action == Toggle) {
            Grid_ForRange(input.range, Grid_ToggleTwo);
        }
    }

    printf("Part two: %lu brightness\n", Grid_Brigthness());
}

int
main(void)
{
    Slice input = StdIn_ReadAll();

    if (input.data == NULL || *input.data == '\0') {
        Quit(1, "%s: empty input.", NAME);
    }

    Part_One(input);
    Grid_Reset();
    Part_Two(input);

    return 0;
}

