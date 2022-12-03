// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

#ifndef DEFS_H
#define DEFS_H 1

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define UNUSED(param) ((void)(param))

typedef struct {
    const char *data;
    u64 size;
} Slice;

#include "support.h"
#include "md5.h"

#endif // DEFS_H

