// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

#ifndef MAP_H
#define MAP_H

typedef struct Map Map;

typedef u64 (*MapKeyHash)(void *key);
typedef bool (*MapKeyCompare)(void *key1, void *key2);

void Map_Create(Map **map, u64 size, MapKeyHash hash, MapKeyCompare compare);
void Map_Destroy(Map **map);
bool Map_Insert(Map *map, void *key, void *value);

#endif // MAP_H
