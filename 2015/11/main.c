// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

/*
--- Day 11: Corporate Policy ---

Santa's previous password expired, and he needs help choosing a new one.

To help him remember his new password after the old one expires, Santa has
devised a method of coming up with a password based on the previous one.
Corporate policy dictates that passwords must be exactly eight lowercase
letters (for security reasons), so he finds his new password by
incrementing his old password string repeatedly until it is valid.

Incrementing is just like counting with numbers: xx, xy, xz, ya, yb, and so
on. Increase the rightmost letter one step; if it was z, it wraps around to
a, and repeat with the next letter to the left until one doesn't wrap
around.

Unfortunately for Santa, a new Security-Elf recently started, and he has
imposed some additional password requirements:
    - Passwords must include one increasing straight of at least three
      letters, like abc, bcd, cde, and so on, up to xyz. They cannot skip
      letters; abd doesn't count.
    - Passwords may not contain the letters i, o, or l, as these letters
      can be mistaken for other characters and are therefore confusing.
    - Passwords must contain at least two different, non-overlapping pairs
      of letters, like aa, bb, or zz.

For example:
    - hijklmmn meets the first requirement (because it contains the
      straight hij) but fails the second requirement requirement (because
      it contains i and l).
    - abbceffg meets the third requirement (because it repeats bb and ff)
      but fails the first requirement.
    - abbcegjk fails the third requirement, because it only has one double
      letter (bb).
    - The next password after abcdefgh is abcdffaa.
    - The next password after ghijklmn is ghjaabcc, because you eventually
      skip all the passwords that start with ghi..., since i is not
      allowed.

Given Santa's current password (your puzzle input), what should his next
password be?


--- Part Two ---

Santa's password expired again. What's the next one?
*/

#define PASSWORD_SIZE 8
char Password[PASSWORD_SIZE + 1] = {0};

bool
Char_IsRestricted(int character)
{
    return character == 'i' ||
           character == 'o' ||
           character == 'l';
}

static bool
Password_Check(const char *password)
{
    if (password == NULL) {
        return false;
    }

    bool rule_1 = false;
    bool rule_2 = true;
    bool rule_3 = true;

    u32 rule_3_check = 0;

    u64 length = strlen(password);

    for (u64 i = 0; i < length; ++i) {
        int letter = password[i];

        // Rule #1
        if (
            i <= length - 3 &&
            letter == password[i + 1] - 1 &&
            letter == password[i + 2] - 2
        ) {
            rule_1 = true;
        }

        // Rule #2
        if (letter == 'i' || letter == 'o' || letter == 'l') {
            rule_2 = false;

            break;
        }

        // Rule #3
        if (
            i <= length - 2 &&
            letter == password[i + 1]
        ) {
            rule_3_check |= 1u << (letter - 'a');
        }
    }

    // Rule #3 check
    int rule_3_count = 0;

    for (int i = 0; i < 26; ++i) {
        if (rule_3_check & 0x01) {
            rule_3_count++;
        }

        rule_3_check >>= 1;
    }

    rule_3 = rule_3_count >= 2;

    return rule_1 && rule_2 && rule_3;
}

static void
Password_Increment(char *password, int increment)
{
    if (password == NULL) {
        Quit(1, "%s: can't increment a (null) password.");
    }

    u64 length = strlen(password) - 1;
    char *cursor = password + length;
    bool wrapped;

    do {
        int character = *cursor + increment;

        wrapped = false;

        if (character > 'z') {
            character = (character - 'a') % 26;
            character += 'a';

            wrapped = true;
        }

        *cursor-- = (char) character;
    } while (wrapped && cursor >= password);
}

void
Part_One(char *input)
{
    do {
        Password_Increment(input, 1);
    } while (Password_Check(input) == false);

    printf("Part one: new password '%s'\n", input);
}

void
Part_Two(char *input)
{
    do {
        Password_Increment(input, 1);
    } while (Password_Check(input) == false);

    printf("Part two: new password '%s'\n", input);
}

int
main(void)
{
    strcpy(Password, "cqjxjnds");

    Part_One(Password);
    Part_Two(Password);

    return 0;
}

