# SPDX-License-Identifier: MIT
# Copyright (c) 2023 Gustavo Ribeiro Croscato

set(sources
    binary_tree.c
    map.c
    md5.c
    slice.c
    support.c
)

set(headers
    binary_tree.h
    defs.h
    map.h
    md5.h
    slice.h
    support.h
)

add_library(lib_c OBJECT ${sources} ${headers})

target_configure_compiler(lib_c)

target_include_directories(lib_c PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_precompile_headers(lib_c PUBLIC defs.h)

add_library(Lib::C ALIAS lib_c)
