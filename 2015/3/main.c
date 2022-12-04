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

#define MAP_SIZE 2048
#define MAP_HASH_PRIME_1 251
#define MAP_HASH_PRIME_2 457

typedef struct MapKey {
    i32 x;
    i32 y;
} MapKey;

typedef struct MapItem {
    MapKey key;
    struct MapItem *next;
} MapItem;

typedef struct Map {
    MapItem **data;
} Map;

Map
Map_Create()
{
    u64 alloc_size = sizeof(MapItem *) * MAP_SIZE;

    MapItem **data = malloc(alloc_size);

    if (data == NULL) {
        Quit(2, "%s: out of memory.");
    }

    for (int i = 0; i < MAP_SIZE - 1; ++i) {
        data[i] = NULL;
    }


    return (Map){data};
}

u64
Map_Hash(MapKey key)
{
    return (
        (key.x * MAP_HASH_PRIME_1) +
        (key.y * MAP_HASH_PRIME_2)
    ) & (MAP_SIZE - 1);
}

bool
Map_Compare(MapKey left, MapKey right)
{
    return left.x == right.x ||
           left.y == right.y;
}

bool
Map_Insert(Map map, MapKey key)
{
    u64 hash = Map_Hash(key);

    MapItem *item = map.data[hash];
    MapItem *parent = NULL;

    if (item != NULL) {
        do {
            if (Map_Compare(item->key, key)) {
                return false;
            }

            parent = item;
            item = item->next;
        } while (item != NULL);
    }

    item = malloc(sizeof(MapItem));

    if (item == NULL) {
        Quit(2, "%s: out of memory.", NAME);
    }

    item->key = key;
    item->next = NULL;

    if (parent) {
        parent->next = item;
    } else {
        map.data[hash] = item;
    }

    return true;
}

void
Map_Destroy(Map *map)
{
    if (map && map->data) {
        MapItem *item;

        for (int i = 0; i < MAP_SIZE; ++i) {
            item = map->data[i];

            if (item == NULL) {
                continue;
            }

            while (item != NULL) {
                MapItem *parent = item;

                item = item->next;

                free(parent);
            }
        }

        free(map->data);

        map->data = NULL;
    }
}

void
Move(MapKey *key, char movement)
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

    MapKey santa = {0, 0};

    Map map = Map_Create();

    Map_Insert(map, santa);

    while (*data != '\n' && *data != '\0') {
        Move(&santa, *data);

        if (Map_Insert(map, santa)) {
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

    MapKey santa = {0, 0};
    MapKey robot = {0, 0};

    Map map_santa = Map_Create();
    Map map_robot = Map_Create();

    Map_Insert(map_santa, santa);
    Map_Insert(map_robot, robot);

    i32 turn = 1;

    while (*data != '\n' && *data != '\0') {
        if (turn % 2 == 1) {
            Move(&santa, *data);

            if (Map_Insert(map_santa, santa)) {
                houses_santa++;
            }
        } else {
            Move(&robot, *data);

            if (Map_Insert(map_robot, robot)) {
                houses_robot++;
            }
        }

        turn++;
        data++;
    }

    Map_Destroy(&map_santa);
    Map_Destroy(&map_robot);

    printf("Part two: %lu houses\n", houses_santa + houses_robot + 1);
}

int
main(void)
{
    Slice input = StdIn_ReadAll();

    if (input.data == NULL || *input.data == '\0') {
        Quit(1, "%s: empty input.", NAME);
    }

    Part_One(input.data);
    Part_Two(input.data);

    return 0;
}

