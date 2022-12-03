// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

#ifndef SUPPORT_H
#define SUPPORT_H

extern const char *DebugFile;
extern const char *DebugFunction;
extern u64 DebugLine;

#define GOTO(tag)             \
{                             \
    DebugFile = __FILE__;     \
    DebugFunction = __func__; \
    DebugLine = __LINE__;     \
                              \
    goto tag;                 \
}

Slice StdIn_ReadAll(void);
const char *StdIn_ReadLine(void);

_Noreturn void Quit(int code, const char *msg, ...);

#endif // SUPPORT_H

