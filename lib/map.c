// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

typedef struct MapItem {
    void *key;
    void *value;

    struct MapItem *next;
} MapItem;

struct Map {
    MapItem **data;

    u64 size;

    MapKeyHash hash;
    MapKeyCompare compare;
};

void
Map_Create(Map **map, u64 size, MapKeyHash hash, MapKeyCompare compare)
{
    *map = malloc(sizeof(struct Map));

    if (*map == NULL) {
        goto out_of_memory;
    }

    (*map)->data = malloc(sizeof(MapItem) * size);

    if ((*map)->data == NULL) {
        goto out_of_memory;
    }

    (*map)->size = size;
    (*map)->hash = hash;
    (*map)->compare = compare;

    for (u64 i = 0; i < (*map)->size; ++i) {
        (*map)->data[i] = NULL;
    }

    return;

out_of_memory:
    Quit(-1, "%s: out of memory in %s at line %d.", __FILE__, __func__, __LINE__);
}

void
Map_Destroy(Map **map)
{
    if (map == NULL || *map == NULL) {
        return;
    }

    for (u64 i = 0; i < (*map)->size; ++i) {
        MapItem *item = (*map)->data[i];

        if (item == NULL) {
            continue;
        }

        MapItem *parent;

        while (item != NULL) {
            parent = item;
            item = item->next;

            free(parent->key);
            free(parent->value);
            free(parent);
        }
    }

    free((*map)->data);
    free(*map);

    *map = NULL;
}

bool
Map_Insert(Map *map, void *key, void *value)
{
    u64 hash = map->hash(key);

    MapItem *item = map->data[hash];
    MapItem *parent = NULL;

    if (item != NULL) {
        do {
            if (map->compare(item->key, key)) {
                free(key);
                free(value);

                return false;
            }

            parent = item;
            item = item->next;
        } while (item != NULL);
    }

    item = malloc(sizeof(MapItem));

    if (item == NULL) {
        Quit(-1, "%s: out of memory in %s at line %d.", __FILE__, __func__, __LINE__);
    }

    item->key = key;
    item->value = value;
    item->next = NULL;

    if (parent) {
        parent->next = item;
    } else {
        map->data[hash] = item;
    }

    return true;
}


