// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

/*
--- Day 2: I Was Told There Would Be No Math ---

The elves are running low on wrapping paper, and so they need to submit an
order for more. They have a list of the dimensions (length l, width w, and
height h) of each present, and only want to order exactly as much as they
need.

Fortunately, every present is a box (a perfect right rectangular prism),
which makes calculating the required wrapping paper for each gift a little
easier: find the surface area of the box, which is 2*l*w + 2*w*h + 2*h*l.
The elves also need a little extra paper for each present: the area of the
smallest side.

For example:
    - A present with dimensions 2x3x4 requires 2*6 + 2*12 + 2*8 = 52 square
      feet of wrapping paper plus 6 square feet of slack, for a total of 58
      square feet.
    - A present with dimensions 1x1x10 requires 2*1 + 2*10 + 2*10 = 42
      square feet of wrapping paper plus 1 square foot of slack, for a
      total of 43 square feet.

All numbers in the elves' list are in feet. How many total square feet of
wrapping paper should they order?


--- Part Two ---

The elves are also running low on ribbon. Ribbon is all the same width, so
they only have to worry about the length they need to order, which they
would again like to be exact.

The ribbon required to wrap a present is the shortest distance around its
sides, or the smallest perimeter of any one face. Each present also
requires a bow made out of ribbon as well; the feet of ribbon required for
the perfect bow is equal to the cubic feet of volume of the present. Don't
ask how they tie the bow, though; they'll never tell.

For example:
    - A present with dimensions 2x3x4 requires 2+2+3+3 = 10 feet of ribbon
      to wrap the present plus 2*3*4 = 24 feet of ribbon for the bow, for a
      total of 34 feet.
    - A present with dimensions 1x1x10 requires 1+1+1+1 = 4 feet of ribbon
      to wrap the present plus 1*1*10 = 10 feet of ribbon for the bow, for
      a total of 14 feet.

How many total feet of ribbon should they order?
*/

typedef struct Dimension {
    u64 length;
    u64 width;
    u64 height;
} Dimension;

static Dimension
Dimension_Extract(const char *data)
{
    if (data == NULL) {
        goto error;
    }

    Dimension result;

    char *end;

    result.length = strtoul(data, &end, 10);

    if (errno == ERANGE) {
        goto error;
    }

    end++;

    result.width = strtoul(end, &end, 10);

    if (errno == ERANGE) {
        goto error;
    }

    end++;

    result.height = strtoul(end, &end, 10);

    if (errno == ERANGE) {
        goto error;
    }

    return result;

error:
    Quit(2, "%s: invalid input '%s'.", NAME, data);
}

static u64
Dimension_Area(Dimension dimension)
{
    u64 area = 2 * (
        (dimension.length * dimension.width) +
        (dimension.width * dimension.height) +
        (dimension.height * dimension.length)
    );

    u64 small_1;
    u64 small_2;

    if (dimension.length < dimension.width) {
        small_1 = dimension.length;
        small_2 = (dimension.width < dimension.height) ? dimension.width : dimension.height;
    } else {
        small_1 = dimension.width;
        small_2 = (dimension.length < dimension.height) ? dimension.length : dimension.height;
    }

    return area + (small_1 * small_2);
}

static u64
Dimension_Length(Dimension dimension)
{
    u64 small_1;
    u64 small_2;

    if (dimension.length < dimension.width) {
        small_1 = dimension.length;
        small_2 = (dimension.width < dimension.height) ? dimension.width : dimension.height;
    } else {
        small_1 = dimension.width;
        small_2 = (dimension.length < dimension.height) ? dimension.length : dimension.height;
    }

    u64 length = 2 * (small_1 + small_2);

    return length + (dimension.length * dimension.width * dimension.height);
}

void
Part_One(Slice slice)
{
    u64 area = 0;

    while (1) {
        Slice line = Slice_ReadLine(&slice);

        if (line.data == NULL) {
            break;
        }

        Dimension dimension = Dimension_Extract(line.data);

        area += Dimension_Area(dimension);
    }

    printf("Part one: square feet area %lu\n", area);
}

void
Part_Two(Slice slice)
{
    u64 length = 0;

    while (1) {
        Slice line = Slice_ReadLine(&slice);

        if (line.data == NULL) {
            break;
        }

        Dimension dimension = Dimension_Extract(line.data);

        length += Dimension_Length(dimension);
    }

    printf("Part two: ribbon length %lu\n", length);
}

int
main(void)
{
    Slice slice = Slice_ReadStdIn();

    if (slice.data == NULL || *slice.data == '\0') {
        Quit(1, "%s: empty input.", NAME);
    }

    Part_One(slice);
    Part_Two(slice);

    return 0;
}

