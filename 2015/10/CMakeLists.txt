# SPDX-License-Identifier: MIT
# Copyright (c) 2023 Gustavo Ribeiro Croscato

cmake_path(GET CMAKE_CURRENT_LIST_DIR STEM LAST_ONLY name)

set(target advent_${year}_${name})
cmake_path(CONVERT "${CMAKE_CURRENT_LIST_DIR}/input" TO_NATIVE_PATH_LIST path_input NORMALIZE)
cmake_path(CONVERT "${CMAKE_CURRENT_LIST_DIR}/sample" TO_NATIVE_PATH_LIST path_sample NORMALIZE)

set(sources
    main.c
)

add_executable(${target} ${sources} ${headers})

target_configure_compiler(${target})

target_compile_definitions(${target} PRIVATE
    NAME=\"Advent_${year}_${name}\"
)

target_link_libraries(${target} PRIVATE Lib::C)

if(EXISTS ${path_input})
    set(run_cmd ${target} < ${path_input})
else()
    set(run_cmd ${target})
endif()

add_custom_target(run_${year}_${name}
    COMMAND ${run_cmd}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

if(EXISTS ${path_sample})
    add_custom_target(test_${year}_${name}
        COMMAND ${target} < ${path_sample}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()
