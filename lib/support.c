// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

#define LINE_SIZE 32 * 1024

const char *DebugFile = NULL;
const char *DebugFunction = NULL;
u64 DebugLine = 0;

static char line[LINE_SIZE];

const char *
StdIn_ReadLine(void)
{
    char *result = line;
    int ch;
    u64 size = LINE_SIZE;

    while ((ch = fgetc(stdin)) != EOF && size != 0) {
        if (ch == '\n') {
            break;
        }

        *result++ = (char) ch;
        size--;
    }

    if (size == 0) {
        fprintf(stderr, "StdIn_ReadLine: buffer overflow.\n");
        exit(1);
    }

    *result = '\0';

    if (ch == EOF && result == line) {
        return NULL;
    }

    return line;
}

_Noreturn void
Quit(int code, const char *msg, ...)
{
    if (msg != NULL) {
        va_list args;

        va_start(args, msg);
        vfprintf(stderr, msg, args);
        va_end(args);

        puts("\n");
    }

    exit(code);
}

