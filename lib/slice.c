// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

#define SLICE_BUFFER_SIZE 64 * 1024

static char SliceBuffer[SLICE_BUFFER_SIZE];
static const Slice NullSlice = {NULL, 0};

bool
Slice_Equals(const Slice lhs, const Slice rhs)
{
    if (
        lhs.data == NULL ||
        rhs.data == NULL ||
        lhs.size != rhs.size
    ) {
        return false;
    }

    return memcmp(lhs.data, rhs.data, lhs.size) == 0;
}

bool
Slice_StartWith(const Slice slice, const Slice subslice)
{
    if (
        slice.data == NULL ||
        subslice.data == NULL ||
        subslice.size > slice.size
    ) {
        return false;
    }

    return memcmp(slice.data, subslice.data, subslice.size) == 0;
}

bool
Slice_EndWith(const Slice slice, const Slice subslice)
{
    if (
        slice.data == NULL ||
        subslice.data == NULL ||
        subslice.size > slice.size
    ) {
        return false;
    }

    return memcmp(slice.data + (slice.size - subslice.size), subslice.data, subslice.size) == 0;
}

bool
Slice_Contains(const Slice slice, const Slice subslice)
{
    if (
        slice.data == NULL ||
        subslice.data == NULL ||
        subslice.size > slice.size
    ) {
        return false;
    }

    return strstr(slice.data, subslice.data) != NULL;
}

bool
Slice_EqualsStr(const Slice slice, const char *data)
{
    return Slice_Equals(slice, (Slice){data, strlen(data)});
}

bool
Slice_StartWithStr(const Slice slice, const char *data)
{
    return Slice_StartWith(slice, (Slice){data, strlen(data)});
}

bool
Slice_EndWithStr(const Slice slice, const char *data)
{
    return Slice_EndWith(slice, (Slice){data, strlen(data)});
}

bool
Slice_ContainsStr(const Slice slice, const char *data)
{
    return Slice_Contains(slice, (Slice){data, strlen(data)});
}

int
Slice_Compare(const Slice lhs, const Slice rhs)
{
    return strncmp(lhs.data, rhs.data, (lhs.size < rhs.size) ? lhs.size : rhs.size);
}

int
Slice_CompareStr(const Slice lhs, const char *data)
{
    return Slice_Compare(lhs, (Slice){data, strlen(data)});
}

Slice
Slice_Find(const Slice slice, const Slice subslice)
{
    if (
        slice.data == NULL ||
        subslice.data == NULL ||
        subslice.size > slice.size
    ) {
        return NullSlice;
    }

    const char *cursor = strstr(slice.data, subslice.data);

    if (cursor == NULL) {
        return NullSlice;
    }

    return (Slice){cursor, slice.size - ((u64) (cursor - slice.data))};
}

Slice
Slice_FindStr(const Slice slice, const char *data)
{
    return Slice_Find(slice, (Slice){data, strlen(data)});
}

Slice
Slice_ReadLine(Slice *slice)
{
    if (slice == NULL || slice->data == NULL || slice->size == 0) {
        return NullSlice;
    }

    char *cursor = SliceBuffer + (slice->data - SliceBuffer);
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

Slice
Slice_ReadStdIn(void)
{
    u64 bytes_read = fread(SliceBuffer, 1, SLICE_BUFFER_SIZE, stdin);

    if (bytes_read == SLICE_BUFFER_SIZE) {
        Quit(-1, "StdIn_ReadLine: buffer overflow.\n");
    }

    if (bytes_read == 0) {
        return NullSlice;
    }

    SliceBuffer[bytes_read] = '\0';

    return (Slice){SliceBuffer, bytes_read};
}

Slice
Slice_Token(Slice *slice, const char *delimiter)
{
    if (
        slice == NULL ||
        delimiter == NULL ||
        slice->data == NULL ||
        slice->size == 0
    ) {
        return NullSlice;
    }

    u64 delimiter_length = strlen(delimiter);

    if (delimiter_length > slice->size) {
        return NullSlice;
    }

    const char *cursor = strstr(slice->data, delimiter);

    Slice result = {.data = slice->data};

    if (cursor) {
        result.size = (u64) (cursor - slice->data);

        slice->data = cursor + delimiter_length;
        slice->size = slice->size - delimiter_length - result.size;
    } else {
        result.size = slice->size;

        slice->data = NULL;
        slice->size = 0;
    }

    return result;
}

void
Slice_Print(Slice slice)
{
    if (slice.data == NULL || slice.size == 0) {
        printf("(NULL)");
    } else {
        printf("%.*s", (int) slice.size, slice.data);
    }
}

