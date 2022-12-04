// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

/*
--- Day 4: The Ideal Stocking Stuffer ---

Santa needs help mining some AdventCoins (very similar to bitcoins) to use
as gifts for all the economically forward-thinking little girls and boys.

To do this, he needs to find MD5 hashes which, in hexadecimal, start with
at least five zeroes. The input to the MD5 hash is some secret key (your
puzzle input, given below) followed by a number in decimal. To mine
AdventCoins, you must find Santa the lowest positive number (no leading
zeroes: 1, 2, 3, ...) that produces such a hash.

For example:
    - If your secret key is abcdef, the answer is 609043, because the MD5
      hash of abcdef609043 starts with five zeroes (000001dbbfa...), and it
      is the lowest such number to do so.
    - If your secret key is pqrstuv, the lowest number it combines with to
      make an MD5 hash starting with five zeroes is 1048970; that is, the
      MD5 hash of pqrstuv1048970 looks like 000006136ef....


--- Part Two ---

Now find one that starts with six zeroes.
*/

#define STR_SIZE 64

char string[STR_SIZE];

void
PrintDigest(const u8 digest[16])
{
    for (int i = 0; i < 16; ++i) {
        printf("%02x", digest[i]);
    }

    printf("\n");
}

void
Part_One(const char *data)
{
    u64 point = 1;

    MD5_CTX context;
    u8 digest[16];

    while (1) {
        i32 string_length = sprintf(string, "%s%lu", data, point);

        if (string_length > STR_SIZE) {
            Quit(2, "%s: buffer overflow.", NAME);
        }

        MD5Init(&context);
        MD5Update(&context, (unsigned char *) string, (unsigned int) string_length);
        MD5Final(&context, digest);

        if (digest[0] == 0 && digest[1] == 0 && digest[2] <= 0xF) {
            break;
        }

        point++;
    }

    printf("Part one: #%lu, MD5 ", point);
    PrintDigest(digest);
}

void
Part_Two(const char *data)
{
    u64 point = 1;

    MD5_CTX context;
    u8 digest[16];

    while (1) {
        i32 string_length = sprintf(string, "%s%lu", data, point);

        if (string_length > STR_SIZE) {
            Quit(2, "%s: buffer overflow.", NAME);
        }

        MD5Init(&context);
        MD5Update(&context, (unsigned char *) string, (unsigned int) string_length);
        MD5Final(&context, digest);

        if (digest[0] == 0 && digest[1] == 0 && digest[2] == 0) {
            break;
        }

        point++;
    }

    printf("Part one: #%lu, MD5 ", point);
    PrintDigest(digest);

}

int
main(void)
{
    Slice input = StdIn_ReadAll();

    if (input.data == NULL || *input.data == '\0') {
        Quit(1, "%s: empty input.", NAME);
    }

#if 0
    const char *hash = "abcdef609043";

    MD5_CTX context;
    u8 digest[16];
    char str[33];

    MD5Init(&context);
    MD5Update(&context, (unsigned char *) hash, (unsigned int) strlen(hash));
    MD5Final(&context, digest);

    DigestToStr(digest, str);

    printf("Hash: %s\n", str);
#endif

    Part_One(input.data);
    Part_Two(input.data);

    return 0;
}

