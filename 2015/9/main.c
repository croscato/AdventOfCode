// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

/*
--- Day 9: All in a Single Night ---

Every year, Santa manages to deliver all of his presents in a single night.

This year, however, he has some new locations to visit; his elves have
provided him the distances between every pair of locations. He can start
and end at any two (different) locations he wants, but he must visit each
location exactly once. What is the shortest distance he can travel to
achieve this?

For example, given the following distances:
    - London to Dublin = 464
    - London to Belfast = 518
    - Dublin to Belfast = 141

The possible routes are therefore:
    - Dublin -> London -> Belfast = 982
    - London -> Dublin -> Belfast = 605
    - London -> Belfast -> Dublin = 659
    - Dublin -> Belfast -> London = 659
    - Belfast -> Dublin -> London = 605
    - Belfast -> London -> Dublin = 982

The shortest of these is London -> Dublin -> Belfast = 605, and so the
answer is 605 in this example.

What is the distance of the shortest route?


--- Part Two ---

The next year, just to show off, Santa decides to take the route with the
longest distance instead.

He can still start and end at any two (different) locations he wants, and
he still must visit each location exactly once.

For example, given the distances above, the longest route would be 982 via
(for example) Dublin -> London -> Belfast.

What is the distance of the longest route?
*/

#define TOKEN_CITY_DELIMITER " "
#define TOKEN_DISTANCE_DELIMITER "="
#define CITY_NAME_SIZE 64

static char City_1[CITY_NAME_SIZE] = {0};
static char City_2[CITY_NAME_SIZE] = {0};

typedef struct TreeConnection {
    u64 distance;

    struct TreeNode *node;
    struct TreeConnection *next;
} TreeConnection;

typedef struct TreeNode {
    char name[CITY_NAME_SIZE];
    u64 id;

    struct TreeNode *next;
    TreeConnection *connections;
} TreeNode;

TreeNode *tree = NULL;

typedef TreeConnection * (*Tree_FindFunction)(u64 id, TreeNode **node);

u64
Hash_PJW(const void *data)
{
    const u8 *str = (u8 *) data;

    u64 hash = 0;
    u64 high;

    while (*str) {
        hash = (hash << 4) + *str++;
        if ((high = hash & 0xF0000000)) {
            hash ^= high >> 24;
        }

        hash &= ~high;
    }

    return hash;
}

TreeNode *
Tree_NewNode(void)
{
    TreeNode *node = malloc(sizeof(struct TreeNode));

    if (!node) {
        Quit(0, "%s: out of memory.", NAME);
    }

    node->name[0] = '\0';
    node->id = 0;
    node->next = NULL;
    node->connections = NULL;

    return node;
}

TreeConnection *
Tree_NewConnection(void)
{
    TreeConnection *connection = malloc(sizeof(struct TreeConnection));

    if (!connection) {
        Quit(0, "%s: out of memory.", NAME);
    }

    connection->distance = 0;
    connection->node = NULL;
    connection->next = NULL;

    return connection;
}

TreeNode *
Tree_FindNode(const char *city)
{
    TreeNode *node = tree;

    u64 id = Hash_PJW(city);

    while (node != NULL) {
        if (node->id == id) {
            break;
        }

        node = node->next;
    }

    return node;
}

void
Tree_Insert(const char *city_1_name, const char *city_2_name, u64 distance)
{
    TreeNode *city_1 = Tree_FindNode(city_1_name);

    if (!city_1) {
        city_1 = Tree_NewNode();
        strcpy(city_1->name, city_1_name);
        city_1->id = Hash_PJW(city_1_name);
        city_1->next = tree;

        tree = city_1;
    }

    TreeNode *city_2 = Tree_FindNode(city_2_name);

    if (!city_2) {
        city_2 = Tree_NewNode();
        strcpy(city_2->name, city_2_name);
        city_2->id = Hash_PJW(city_2_name);
        city_2->next = tree;

        tree = city_2;
    }

    TreeConnection *connection = Tree_NewConnection();

    connection->distance = distance;
    connection->node = city_2;
    connection->next = city_1->connections;

    city_1->connections = connection;
}

void
Tree_Print()
{
    TreeNode *cursor = tree;

    while (cursor) {
        printf("%s -> ", cursor->name);

        TreeConnection *connection = cursor->connections;

        while (connection) {
            printf("%s (%lu) | ", connection->node->name, connection->distance);

            connection = connection->next;
        }

        printf("\n");

        cursor = cursor->next;
    }
}

void
Tree_Remove(u64 id)
{
    TreeNode *node = tree;

    while (node) {
        if (node->id != id) {
            TreeConnection *connection = node->connections;
            TreeConnection *parent_connection = NULL;

            while (connection) {
                if (connection->node->id == id) {
                    if (parent_connection) {
                        parent_connection->next = connection->next;
                    } else {
                        node->connections = connection->next;
                    }

                    TreeConnection *connection_to_free = connection;

                    connection = connection->next;

                    free(connection_to_free);

                    continue;
                }

                parent_connection = connection;
                connection = connection->next;
            }
        }

        node = node->next;
    }

    node = tree;
    TreeNode *parent = NULL;

    while (node) {
        if (node->id == id) {
            TreeConnection *connection = node->connections;
            TreeConnection *connection_to_free;

            while (connection) {
                connection_to_free = connection;
                connection = connection->next;

                free(connection_to_free);
            }

            if (parent) {
                parent->next = node->next;
            } else {
                tree = node->next;
            }

            free(node);

            break;
        }

        parent = node;
        node = node->next;
    }
}

TreeConnection *
Tree_FindSmallest(u64 id, TreeNode **node)
{
    if (!tree) {
        return NULL;
    }

    TreeNode *cursor = tree;
    TreeConnection *smallest = NULL;

    while (cursor) {
        TreeConnection *connection = cursor->connections;

        while (connection) {
            if (
                ((connection->node->id == id) || (cursor->id == id)) &&
                (smallest == NULL || connection->distance < smallest->distance)
            ) {
                *node = cursor;
                smallest = connection;
            }

            connection = connection->next;
        }

        cursor = cursor->next;
    }

    return smallest;
}

TreeConnection *
Tree_FindGreatest(u64 id, TreeNode **node)
{
    if (!tree) {
        return NULL;
    }

    TreeNode *cursor = tree;
    TreeConnection *greatest = NULL;

    while (cursor) {
        TreeConnection *connection = cursor->connections;

        while (connection) {
            if (
                ((id == 0) || (connection->node->id == id) || (cursor->id == id)) &&
                (greatest == NULL || connection->distance > greatest->distance)
            ) {
                *node = cursor;
                greatest = connection;
            }

            connection = connection->next;
        }

        cursor = cursor->next;
    }

    return greatest;
}

u64
Tree_FindPath(Tree_FindFunction find_function)
{
    if (!tree) {
        Quit(0, "%s: Invalid tree.");
    }

    u64 path = 0;
    u64 find_id = 0;

    TreeNode *node = tree;

    while (tree) {
        TreeConnection *found = find_function(find_id, &node);

        if (found == NULL) {
            Quit(0, "%s: Can't find found node connected to.", NAME);
        }

        path += found->distance;

        if (node->id == find_id) {
            printf("%s to %s with %lu distance\n", node->name, found->node->name, found->distance);
            find_id = found->node->id;
            Tree_Remove(node->id);
        } else {
            printf("%s to %s with %lu distance\n", found->node->name, node->name, found->distance);
            find_id = node->id;
            Tree_Remove(found->node->id);
        }

        if (tree->next == NULL && tree->connections == NULL) {
            Tree_Remove(tree->id);
        }
    }

    return path;
}

void
Part_One(Slice input)
{
    while (1) {
        Slice line = Slice_ReadLine(&input);

        if (line.data == NULL) {
            break;
        }

        Slice token = Slice_Token(&line, TOKEN_CITY_DELIMITER);
        strncpy(City_1, token.data, token.size);
        City_1[token.size] = '\0';

        Slice_Token(&line, TOKEN_CITY_DELIMITER);

        token = Slice_Token(&line, TOKEN_CITY_DELIMITER);
        strncpy(City_2, token.data, token.size);
        City_2[token.size] = '\0';

        Slice_Token(&line, TOKEN_DISTANCE_DELIMITER);
        token = Slice_Token(&line, TOKEN_DISTANCE_DELIMITER);

        u64 distance = strtoul(token.data, NULL, 10);

        Tree_Insert(City_1, City_2, distance);
    }

    u64 shortest_path = Tree_FindPath(Tree_FindSmallest);

    printf("Part one: shortest path %lu\n", shortest_path);
}

void
Part_Two(Slice input)
{
    while (1) {
        Slice line = Slice_ReadLine(&input);

        if (line.data == NULL) {
            break;
        }

        Slice token = Slice_Token(&line, TOKEN_CITY_DELIMITER);
        strncpy(City_1, token.data, token.size);
        City_1[token.size] = '\0';

        Slice_Token(&line, TOKEN_CITY_DELIMITER);

        token = Slice_Token(&line, TOKEN_CITY_DELIMITER);
        strncpy(City_2, token.data, token.size);
        City_2[token.size] = '\0';

        Slice_Token(&line, TOKEN_DISTANCE_DELIMITER);
        token = Slice_Token(&line, TOKEN_DISTANCE_DELIMITER);

        u64 distance = strtoul(token.data, NULL, 10);

        Tree_Insert(City_1, City_2, distance);
    }

    Tree_Print();
    puts("==================================");

    u64 greatest_path = Tree_FindPath(Tree_FindGreatest);

    printf("Part two: greatest path %lu\n", greatest_path);
}

int
main(void)
{
    Slice input = Slice_ReadStdIn();

    if (input.data == NULL || *input.data == '\0') {
        Quit(1, "%s: empty input.", NAME);
    }

    //Part_One(input);
    //puts("==================================");
    Part_Two(input);

    return 0;
}

