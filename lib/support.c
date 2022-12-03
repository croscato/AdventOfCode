// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

#define LINE_SIZE 32 * 1024

const char *DebugFile = NULL;
const char *DebugFunction = NULL;
u64 DebugLine = 0;

static char line[LINE_SIZE];

static Slice EmptySlice = { NULL, 0 };

Slice
StdIn_ReadAll(void)
{
    u64 bytes_read = fread(line, 1, LINE_SIZE, stdin);

    if (bytes_read == LINE_SIZE) {
        Quit(-1, "StdIn_ReadLine: buffer overflow.\n");
    }

    if (bytes_read == 0) {
        return EmptySlice;
    }

    line[bytes_read] = '\0';

    return (Slice){line, bytes_read};
}

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

Slice
Slice_ReadLine(Slice *slice)
{
    if (slice == NULL || slice->data == NULL || slice->size == 0) {
        return EmptySlice;
    }

    char *cursor = (char *) slice->data;
    u64 available = slice->size;

    u64 new_size = 0;

    while (*cursor != '\n' && *cursor != '\0' && available > 0) {
        cursor++;
        available--;
        new_size++;
    }

    Slice result;

    if (available > 0) {
        *cursor = '\0';

        result.data = slice->data;
        result.size = new_size;

        slice->data = cursor + 1;
        slice->size = slice->size - new_size - 1;
    } else {
        result.data = slice->data;
        result.size = slice->size;

        slice->data = NULL;
        slice->size = 0;
    }

    return result;
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

