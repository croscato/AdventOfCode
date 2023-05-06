# SPDX-License-Identifier: MIT
# Copyright (c) 2023 Gustavo Ribeiro Croscato

cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(advent_of_code LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(compiler)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(lib)
add_subdirectory(2015)
