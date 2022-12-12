/*

*/

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define LINE_SIZE 1024

static const char *
Read_Line(void)
{
    static char line[LINE_SIZE];

    char *result = line;
    int ch;

    while ((ch = fgetc(stdin)) != EOF) {
        if (ch == '\n') {
            *result = '\0';
            break;
        }

        *result++ = (char) ch;
    }

    if (ch == EOF) {
        return NULL;
    }

    return line;
}

int
main(void)
{
    printf("Hello, world!\n");

    return 0;
}

