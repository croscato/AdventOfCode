// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

#include "binary_tree.h"

typedef struct BinaryTreeNode {
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
    u8 value[];
} BinaryTreeNode;

struct BinaryTree {
    BinaryTreeNode *root;
};

void
BinaryTree_Create(BinaryTree **tree, BinaryTreeConfig config)
{
}

