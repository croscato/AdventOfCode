// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

#ifndef SLICE_H
#define SLICE_H 1

typedef struct {
    const char *data;
    u64 size;
} Slice;

bool Slice_Equals(const Slice lhs, const Slice rhs);
bool Slice_StartWith(const Slice slice, const Slice subslice);
bool Slice_EndWith(const Slice slice, const Slice subslice);
bool Slice_Contains(const Slice slice, const Slice subslice);
bool Slice_EqualsStr(const Slice slice, const char *data);
bool Slice_StartWithStr(const Slice slice, const char *data);
bool Slice_EndWithStr(const Slice slice, const char *data);
bool Slice_ContainsStr(const Slice slice, const char *data);
int Slice_Compare(const Slice lhs, const Slice rhs);
int Slice_CompareStr(const Slice lhs, const char *data);
Slice Slice_Find(const Slice slice, const Slice subslice);
Slice Slice_FindStr(const Slice slice, const char *data);
Slice Slice_ReadLine(Slice *slice);
Slice Slice_ReadStdIn(void);
Slice Slice_Token(Slice *slice, const char *delimiter);
void Slice_Print(Slice slice);

#endif // SLICE_H

