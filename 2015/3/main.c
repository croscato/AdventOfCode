// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

/*
--- Day 3: Perfectly Spherical MapKeys in a Vacuum ---

Santa is delivering presents to an infinite two-dimensional grid of keys.

He begins by delivering a present to the key at his starting location,
and then an elf at the North Pole calls him via radio and tells him where
to move next. Moves are always exactly one key to the north (^), south
(v), east (>), or west (<). After each move, he delivers another present to
the key at his new location.

However, the elf back at the north pole has had a little too much eggnog,
and so his directions are a little off, and Santa ends up visiting some
keys more than once. How many keys receive at least one present?

For example:
    - > delivers presents to 2 keys: one at the starting location, and
      one to the east.
    - ^>v< delivers presents to 4 keys in a square, including twice to
      the key at his starting/ending location.
    - ^v^v^v^v^v delivers a bunch of presents to some very lucky children
      at only 2 keys.

--- Part Two ---

The next year, to speed up the process, Santa creates a robot version of
himself, Robo-Santa, to deliver presents with him.

Santa and Robo-Santa start at the same location (delivering two presents to
the same starting house), then take turns moving based on instructions from
the elf, who is eggnoggedly reading from the same script as the previous
year.

This year, how many houses receive at least one present?

For example:
    - ^v delivers presents to 3 houses, because Santa goes north, and then
      Robo-Santa goes south.
    - ^>v< now delivers presents to 3 houses, and Santa and Robo-Santa end
      up back where they started.
    - ^v^v^v^v^v now delivers presents to 11 houses, with Santa going one
      direction and Robo-Santa going the other.
*/

// North ^
// South v
// East  >
// West  <

#include "map.h"

#define MAP_SIZE 2048
#define MAP_HASH_PRIME_1 251
#define MAP_HASH_PRIME_2 457

typedef struct MapKey {
    i32 x;
    i32 y;
} MapKey;

typedef u64 MapValue;

typedef struct MapData {
    MapKey *key;
    MapValue *value;
} MapData;

static MapData
Map_CreateData(MapKey key, MapValue value)
{
    MapData result;

    result.key = malloc(sizeof(MapKey));
    result.value = malloc(sizeof(MapValue));

    if (result.key == NULL || result.value == NULL) {
        Quit(3, "%s: out of memory.", NAME);
    }

    *result.key = key;
    *result.value = value;

    return result;
}

static u64
Map_Hash(void *key)
{
    MapKey *map_key = (MapKey *) key;

    u64 hash_value = (u64) (
        (map_key->x * MAP_HASH_PRIME_1) +
        (map_key->y * MAP_HASH_PRIME_2)
    ) % (MAP_SIZE - 1);

    if (hash_value >= MAP_SIZE) {
        Quit(2, "%s: invalid hash: %lu\n", NAME, hash_value);
    }

    return hash_value;
}

static bool
Map_Compare(void *left, void *right)
{
    MapKey *key_left = (MapKey *) left;
    MapKey *key_right = (MapKey *) right;

    return key_left->x == key_right->x &&
           key_left->y == key_right->y;
}

static void
Map_Move(MapKey *key, char movement)
{
    if (movement == '^') {
        key->y--;
    } else if (movement == 'v') {
        key->y++;
    } else if (movement == '>') {
        key->x++;
    } else if (movement == '<') {
        key->x--;
    }
}

void
Part_One(const char *data)
{
    u64 houses = 0;

    MapKey santa = {0,0};

    Map *map = NULL;

    Map_Create(&map, MAP_SIZE, Map_Hash, Map_Compare);

    MapData map_data = Map_CreateData(santa, 0);

    Map_Insert(map, map_data.key, map_data.value);

    while (*data != '\n' && *data != '\0') {
        Map_Move(&santa, *data);

        map_data = Map_CreateData(santa, 0);

        if (Map_Insert(map, map_data.key, map_data.value)) {
            houses++;
        }

        data++;
    }

    Map_Destroy(&map);

    printf("Part one: %lu houses\n", houses + 1);
}

void
Part_Two(const char *data)
{
    u64 houses_santa = 0;
    u64 houses_robot = 0;

    MapKey santa = {0,0};
    MapKey robot = {0,0};

    Map *map = NULL;

    Map_Create(&map, MAP_SIZE, Map_Hash, Map_Compare);

    MapData map_data = Map_CreateData(santa, 0);
    Map_Insert(map, map_data.key, map_data.value);

    u8 santa_turn = true;

    while (*data != '\0') {
        if (santa_turn) {
            Map_Move(&santa, *data);

            map_data = Map_CreateData(santa, 0);

            if (Map_Insert(map, map_data.key, map_data.value)) {
                houses_santa++;
            }
        } else {
            Map_Move(&robot, *data);

            map_data = Map_CreateData(robot, 0);

            if (Map_Insert(map, map_data.key, map_data.value)) {
                houses_robot++;
            }
        }

        santa_turn ^= true;
        data++;
    }

    Map_Destroy(&map);

    printf("Part two: %lu houses\n", houses_santa + houses_robot + 1);
}

int
main(void)
{
    Slice input = Slice_ReadStdIn();

    if (input.data == NULL || *input.data == '\0') {
        Quit(1, "%s: empty input.", NAME);
    }

    Part_One(input.data);
    Part_Two(input.data);

    return 0;
}

