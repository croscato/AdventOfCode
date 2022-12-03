// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

#ifndef BINARY_TREE_H
#define BINARY_TREE_H 1

typedef struct BinaryTree BinaryTree;

typedef struct BinaryTreeConfig {
    u64 value_size;
} BinaryTreeConfig;

void BinaryTree_Create(BinaryTree **tree, BinaryTreeConfig config);

#endif // BINARY_TREE_H

